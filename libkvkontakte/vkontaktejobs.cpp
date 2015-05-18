/* Copyright 2010, 2011  Thomas McGuire <mcguire@kde.org>
   Copyright 2011  Roeland Jago Douma <unix@rullzer.com>
   Copyright 2011, 2015  Alexander Potashev <aspotashev@gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "vkontaktejobs.moc"

#include <QtCore/QTimer>
#include <qjson/parser.h>
#include <KIO/Job>
#include <KDebug>
#include <KLocale>

namespace Vkontakte
{

bool KJobWithSubjob::doKill()
{
    if (m_job)
        m_job->kill(KJob::Quietly);
    return KJob::doKill();
}

//---------------------------------------------------------------

bool KJobWithSubjobs::doKill()
{
    foreach(KJob *job, m_jobs)
        job->kill(KJob::Quietly);
    m_jobs.clear();

    return KJob::doKill();
}

//---------------------------------------------------------------

/*
 * VkontakteJobs base class
 */
VkontakteJob::VkontakteJob(const QString &accessToken, const QString &method, bool httpPost)
    : m_accessToken(accessToken)
    , m_method(method)
    , m_httpPost(httpPost)
{
    setCapabilities(KJob::Killable);
}

void VkontakteJob::addQueryItem(const QString &key, const QString &value)
{
    QueryItem item;
    item.first = key;
    item.second = value;
    m_queryItems.append(item);
}

bool VkontakteJob::handleError(const QVariant &data)
{
    const QVariantMap errorMap = data.toMap();
    int error_code = errorMap["error_code"].toInt();
    const QString error_msg = errorMap["error_msg"].toString();
    kWarning() << "An error of type" << error_code << "occurred:" << error_msg;

    if (error_code == 6)
    {
        // "Too many requests per second", we will retry after a delay.
        // VK API limit the rate of requests to 3 requests per second,
        // so it should be OK if we wait for 340 ms.
        QTimer::singleShot(340, this, SLOT(slotRetry()));
        return true;
    }
    else
    {
        setError(KJob::UserDefinedError);
        setErrorText(i18n(
            "The VKontakte server returned an error "
            "of type <i>%1</i> in reply to method %2: <i>%3</i>",
            error_code, m_method, error_msg));
        return false;
    }
}

KJob* VkontakteJob::createHttpJob()
{
    KUrl url;
    url.setProtocol("https");
    url.setHost("api.vkontakte.ru");
    url.setPath("/method/" + m_method);

    prepareQueryItems();
    foreach(const QueryItem &item, m_queryItems)
        url.addQueryItem(item.first, item.second);
    url.addQueryItem("access_token", m_accessToken);

    // TODO: Save KUrl to reuse it if we need to retry the HTTP request
//     m_url = url;

    kDebug() << "Starting request" << url;

    if (m_httpPost)
        return KIO::storedHttpPost(QByteArray(), url, KIO::HideProgressInfo);
    else
        return KIO::storedGet(url, KIO::Reload, KIO::HideProgressInfo);
}

void VkontakteJob::start()
{
    m_job = createHttpJob();
    connect(m_job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)));
    m_job->start();
}

void VkontakteJob::slotRetry()
{
    start();
}

void VkontakteJob::jobFinished(KJob *kjob)
{
    KIO::StoredTransferJob *job = dynamic_cast<KIO::StoredTransferJob *>(kjob);
    Q_ASSERT(job);
    if (job && job->error())
    {
        setError(job->error());
        setErrorText(KIO::buildErrorString(error(), job->errorText()));
        kWarning() << "Job error: " << job->errorString();
    }
    else
    {
        kDebug() << "Got data: " << QString::fromAscii(job->data().data());
        QJson::Parser parser;
        bool ok;
        const QVariant data = parser.parse(job->data(), &ok);
        if (ok)
        {
            const QVariant error = data.toMap()["error"];
            if (error.isValid())
            {
                bool willRetry = handleError(error);
                if (willRetry)
                    return;
            }
            else
            {
                handleData(data.toMap()["response"]);
            }
        }
        else
        {
            kWarning() << "Unable to parse JSON data: " << QString::fromAscii(job->data().data());
            setError( KJob::UserDefinedError );
            setErrorText(i18n("Unable to parse data returned by the VKontakte server: %1", parser.errorString()));
        }
    }
    emitResult();
    m_job = 0;
}

} /* namespace Vkontakte */

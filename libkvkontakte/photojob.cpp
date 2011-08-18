/* Copyright 2011 Alexander Potashev <aspotashev@gmail.com>

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
#include "photojob.h"
#include <KIO/Job>
#include <KDebug>

namespace Vkontakte
{

PhotoJob::PhotoJob(const KUrl &url)
    : m_url(url)
    , d(0)
{
}

void PhotoJob::start()
{
    kDebug() << "Starting photo download" << m_url;
    KIO::StoredTransferJob * const job = KIO::storedGet(m_url, KIO::Reload, KIO::HideProgressInfo);
    m_job = job;
    connect(job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)));
    job->start();
}

void PhotoJob::jobFinished(KJob *job)
{
    KIO::StoredTransferJob *transferJob = dynamic_cast<KIO::StoredTransferJob *>(job);
    Q_ASSERT(transferJob);
    if (transferJob->error())
    {
        setError(transferJob->error());
        setErrorText(KIO::buildErrorString(error(), transferJob->errorText()));
        kWarning() << "Job error: " << transferJob->errorString();
    }
    else
        m_photo = QImage::fromData(transferJob->data());

    emitResult();
    m_job = 0;
}

QImage PhotoJob::photo() const
{
    return m_photo;
}

} /* namespace Vkontakte */

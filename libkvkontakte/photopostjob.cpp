/*
 * Copyright (C) 2011, 2015  Alexander Potashev <aspotashev@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "photopostjob.h"

#include <KIO/Job>
#include <KLocalizedString>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeDatabase>
#include <QtCore/QMimeType>
#include <QtCore/QJsonDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttpMultiPart>

namespace Vkontakte
{

PhotoPostJob::PhotoPostJob(Vkontakte::UploadPhotosJob::Dest dest,
                           const QUrl &url, const QStringList &files)
{
    m_url = url;
    m_files = files;
    m_dest = dest;

    setCapabilities(KJob::Killable);

    m_ok = true;
    if (files.size() <= 0 || files.size() > 5)
        m_ok = false;
}

void PhotoPostJob::handleError(const QJsonValue &data)
{
    int error_code = -1;
    QString error_msg;

    if (data.isUndefined())
    {
        qWarning() << "Response from server has unexpected format";
    }
    else
    {
        const QVariantMap errorMap = data.toVariant().toMap();

        error_code = errorMap["error_code"].toInt();
        error_msg = errorMap["error_msg"].toString();

        qWarning() << "An error of type" << error_code << "occurred:" << error_msg;
    }

    setError(KJob::UserDefinedError);

    if (data.isUndefined())
    {
        setErrorText(i18n(
            "Response from the VKontakte server has unexpected format. "
            "Please report this problem against product libkvkontakte "
            "at the <a href=\"%1\">KDE bug tracker</b>.",
            QStringLiteral("http://bugs.kde.org/")));
    }
    else
    {
        setErrorText(i18n(
            "The VKontakte server returned an error "
            "of type <i>%1</i> in reply to uploading to URL %2: <i>%3</i>",
            error_code, m_url.toString(), error_msg));
    }
}

bool PhotoPostJob::appendFile(QHttpMultiPart *multiPart, const QString &header, const QString &path)
{
    QString mime = QMimeDatabase().mimeTypeForUrl(QUrl(path)).name();
    if (mime.isEmpty())
        return false;

    QFileInfo fileInfo(path);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant(QString("form-data; name=\"%1\"; filename=\"%2\"").arg(header).arg(fileInfo.fileName())));
    imagePart.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(fileInfo.size()));
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mime));
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
    {
        delete file;
        return false;
    }

    imagePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    multiPart->append(imagePart);
    return true;
}

void PhotoPostJob::start()
{
    if (!m_ok)
    {
        setError(UserDefinedError);
        setErrorText("Internal error");
        emitResult();
    }

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    switch (m_dest)
    {
        case Vkontakte::UploadPhotosJob::DEST_ALBUM:
            // "file1" .. "file5"
            for (int i = 0; i < m_files.size(); i ++)
                if (!appendFile(multiPart, QString("file%1").arg(i + 1), m_files[i]))
                {
                    m_ok = false;
                    break;
                }
            break;
        case Vkontakte::UploadPhotosJob::DEST_PROFILE:
        case Vkontakte::UploadPhotosJob::DEST_WALL:
            // "photo"
            if (!appendFile(multiPart, QString("photo"), m_files[0]))
                m_ok = false;
            break;
        default:
            m_ok = false;
            break;
    }

    if (!m_ok)
    {
        setError(UserDefinedError);
        setErrorText("Could not attach file");
        emitResult();
    }

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(parseNetworkResponse(QNetworkReply*)));

    qDebug() << "Starting POST request" << m_url;
    QNetworkReply *reply = manager->post(QNetworkRequest(m_url), multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply
}

void PhotoPostJob::parseNetworkResponse(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        // A communication error has occurred
        setError(reply->error());
        setErrorText(KIO::buildErrorString(error(), QString()));
//         kWarning() << "Network error: " << reply->errorString();
    }
    else
    {
        QByteArray ba = reply->readAll();
        qDebug() << "Got data:" << ba;

        QJsonParseError parseError;
        QJsonDocument data = QJsonDocument::fromJson(ba, &parseError);
        if (parseError.error == QJsonParseError::NoError)
        {
            const QJsonObject object = data.object();

            if (!data.isObject())
            {
                // Something went wrong, but there is no valid object "error"
                handleError(QJsonValue::Undefined);
            }
            else if (object.contains("error"))
            {
                handleError(object.value("error"));
            }
            else
            {
                // Handle data
                m_response = object.toVariantMap();
            }
        }
        else
        {
            qWarning() << "Unable to parse JSON data:" << ba;
            setError(KJob::UserDefinedError);
            setErrorText(
                i18n("Unable to parse data returned by the VKontakte server: %1",
                     parseError.errorString()));
        }
    }

    emitResult();
}

QVariantMap PhotoPostJob::response() const
{
    return m_response;
}

} /* namespace Vkontakte */

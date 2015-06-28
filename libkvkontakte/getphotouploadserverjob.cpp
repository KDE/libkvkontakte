/*
 * Copyright (C) 2011, 2012  Alexander Potashev <aspotashev@gmail.com>
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

#include "getphotouploadserverjob.h"
#include "uploadphotosjob.h"

#include <kdebug.h>

#include <QtCore/QVariant>

namespace Vkontakte
{

GetPhotoUploadServerJob::GetPhotoUploadServerJob(const QString &accessToken, Vkontakte::UploadPhotosJob::Dest dest)
    : VkontakteJob(accessToken, getMethod(dest))
{
    m_dest = dest;

    m_aid = -1;
    m_gid = -1;
    m_uid = -1;
}

void GetPhotoUploadServerJob::initUploadAlbum(int aid, int gid)
{
    m_aid = aid;
    m_gid = gid;
}

// static
QString GetPhotoUploadServerJob::getMethod(Vkontakte::UploadPhotosJob::Dest dest)
{
    switch (dest)
    {
        case Vkontakte::UploadPhotosJob::DEST_ALBUM:
            return QLatin1String("photos.getUploadServer");
        case Vkontakte::UploadPhotosJob::DEST_PROFILE:
            return QLatin1String("photos.getProfileUploadServer");
        case Vkontakte::UploadPhotosJob::DEST_WALL:
            return QLatin1String("photos.getWallUploadServer");
        default:
            return QLatin1String("");
    }
}

void GetPhotoUploadServerJob::prepareQueryItems()
{
    switch (m_dest)
    {
        case Vkontakte::UploadPhotosJob::DEST_ALBUM:
            if (m_aid == -1)
            {
                setError(KJob::UserDefinedError);
                setErrorText("m_aid not set.");
                kWarning() << "m_aid not set.";
            }

            addQueryItem("aid", QString::number(m_aid));
            if (m_gid != -1)
                addQueryItem("gid", QString::number(m_gid));
            break;

        case Vkontakte::UploadPhotosJob::DEST_PROFILE:
            // photos.getProfileUploadServer has not parameters
            break;

        case Vkontakte::UploadPhotosJob::DEST_WALL:
            if (m_uid != -1 && m_gid != -1)
            {
                setError(KJob::UserDefinedError);
                setErrorText("Only one parameter m_uid or m_gid should be set.");
                kWarning() << "Only one parameter m_uid or m_gid should be set.";
            }

            if (m_uid != -1)
                addQueryItem("uid", QString::number(m_uid));
            if (m_gid != -1)
                addQueryItem("gid", QString::number(m_gid));
            break;

        default:
            setError(KJob::UserDefinedError);
            setErrorText("Unsupported m_dest.");
            kWarning() << "Unsupported m_dest.";
            break;
    }
}

void GetPhotoUploadServerJob::handleData(const QVariant &data)
{
    m_uploadUrl = data.toMap()["upload_url"].toString();
}

QString GetPhotoUploadServerJob::uploadUrl() const
{
    return m_uploadUrl;
}

} /* namespace Vkontakte */

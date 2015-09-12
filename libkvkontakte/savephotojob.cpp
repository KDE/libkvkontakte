/*
 * Copyright (C) 2011  Alexander Potashev <aspotashev@gmail.com>
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

#include "savephotojob.h"

#include <QtCore/QJsonArray>

namespace Vkontakte
{

SavePhotoJob::SavePhotoJob(const QString &accessToken,
                           UploadPhotosJob::Dest dest,
                           const QVariantMap &photoIdData, int gid)
    : VkontakteJob(accessToken, getMethod(dest), true)
{
    m_dest = dest;

    switch (dest)
    {
        case Vkontakte::UploadPhotosJob::DEST_ALBUM:
            addQueryItem(QStringLiteral("aid"), photoIdData[QStringLiteral("aid")].toString());
            addQueryItem(QStringLiteral("server"), photoIdData[QStringLiteral("server")].toString());
            addQueryItem(QStringLiteral("photos_list"), photoIdData[QStringLiteral("photos_list")].toString());
            addQueryItem(QStringLiteral("hash"), photoIdData[QStringLiteral("hash")].toString());

            // TODO: may be "gid" will also be in "photoIdData", so this argument is unnecessary?
            if (gid != -1)
                addQueryItem(QStringLiteral("gid"), QString::number(gid));
            break;
        case Vkontakte::UploadPhotosJob::DEST_PROFILE:
            addQueryItem(QStringLiteral("server"), photoIdData[QStringLiteral("server")].toString());
            addQueryItem(QStringLiteral("photo"), photoIdData[QStringLiteral("photos")].toString());
            addQueryItem(QStringLiteral("hash"), photoIdData[QStringLiteral("hash")].toString());
            break;
        case Vkontakte::UploadPhotosJob::DEST_WALL:
            // TODO: support optional parameters "uid" and "gid" (for posting to other users' and groups' walls)
            // TODO: for posting onto a wall, we must also call the "wall.post" VK method
            addQueryItem(QStringLiteral("server"), photoIdData[QStringLiteral("server")].toString());
            addQueryItem(QStringLiteral("photo"), photoIdData[QStringLiteral("photo")].toString());
            addQueryItem(QStringLiteral("hash"), photoIdData[QStringLiteral("hash")].toString());
            break;
        default:
            // TODO: handle unknown destination error
            break;
    }
}

// static
QString SavePhotoJob::getMethod(Vkontakte::UploadPhotosJob::Dest dest)
{
    switch (dest)
    {
        case Vkontakte::UploadPhotosJob::DEST_ALBUM:
            return QLatin1String("photos.save");
        case Vkontakte::UploadPhotosJob::DEST_PROFILE:
            return QLatin1String("photos.saveProfilePhoto");
        case Vkontakte::UploadPhotosJob::DEST_WALL:
            return QLatin1String("photos.saveWallPhoto");
        default:
            return QLatin1String("");
    }
}

void SavePhotoJob::handleItem(const QJsonValue &item)
{
    if (!item.isObject())
    {
        // TODO: report error!!!
        m_list.clear();
        return;
    }

    m_list.append(PhotoInfo(item.toObject()));
}

void SavePhotoJob::handleData(const QJsonValue &data)
{
    switch (m_dest)
    {
        case Vkontakte::UploadPhotosJob::DEST_ALBUM:
            if (!data.isArray())
            {
                // TODO: report error!!!
                return;
            }

            foreach (const QJsonValue &item, data.toArray())
            {
                handleItem(item);
            }
            break;
        case Vkontakte::UploadPhotosJob::DEST_PROFILE:
        case Vkontakte::UploadPhotosJob::DEST_WALL:
            handleItem(data);
            break;
        default:
            break;
    }
}

QList<PhotoInfo> SavePhotoJob::list() const
{
    return m_list;
}

} /* namespace Vkontakte */

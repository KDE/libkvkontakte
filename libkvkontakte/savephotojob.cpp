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
#include "savephotojob.h"
#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

SavePhotoJob::SavePhotoJob(const QString &accessToken,
                           const QVariantMap &photoIdData, int gid)
    : VkontakteJob(accessToken, "photos.save", true)
{
    addQueryItem("aid", photoIdData["aid"].toString());
    addQueryItem("server", photoIdData["server"].toString());
    addQueryItem("photos_list", photoIdData["photos_list"].toString());
    addQueryItem("hash", photoIdData["hash"].toString());

    // TODO: may be "gid" will also be in "photoIdData", so this argument is unnecessary?
    if (gid != -1)
        addQueryItem("gid", QString::number(gid));
}

void SavePhotoJob::handleItem(const QVariant &data)
{
    PhotoInfoPtr item(new PhotoInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
    m_list.append(item);
}

void SavePhotoJob::handleData(const QVariant &data)
{
    foreach (const QVariant &item, data.toList())
        handleItem(item);
}

QList<PhotoInfoPtr> SavePhotoJob::list() const
{
    return m_list;
}

} /* namespace Vkontakte */

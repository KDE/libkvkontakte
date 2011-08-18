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
#include "albumlistjob.h"
#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

AlbumListJob::AlbumListJob(const QString &accessToken, int uid, const QIntList &aids)
    : VkontakteJob(accessToken, "photos.getAlbums")
    , d(0)
{
    if (uid != -1)
        addQueryItem("uid", QString::number(uid));
    if (!aids.empty())
        addQueryItem("aids", aids.join());
}

void AlbumListJob::handleItem(const QVariant &data)
{
    AlbumInfoPtr item(new AlbumInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
    m_list.append(item);
}

void AlbumListJob::handleData(const QVariant &data)
{
    foreach (const QVariant &item, data.toList())
        handleItem(item);
}

QList<AlbumInfoPtr> AlbumListJob::list() const
{
    return m_list;
}

} /* namespace Vkontakte */

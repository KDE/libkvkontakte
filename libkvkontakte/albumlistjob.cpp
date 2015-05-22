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

#include "albumlistjob.h"

#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

class AlbumListJob::Private
{
public:
    QList<AlbumInfoPtr> list;
};

AlbumListJob::AlbumListJob(const QString &accessToken, int uid, const QIntList &aids)
    : VkontakteJob(accessToken, "photos.getAlbums")
    , d(new Private)
{
    if (uid != -1)
        addQueryItem("uid", QString::number(uid));
    if (!aids.empty())
        addQueryItem("aids", aids.join());
}

AlbumListJob::~AlbumListJob()
{
    delete d;
}

void AlbumListJob::handleItem(const QVariant &data)
{
    AlbumInfoPtr item(new AlbumInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
    d->list.append(item);
}

void AlbumListJob::handleData(const QVariant &data)
{
    foreach(const QVariant &item, data.toList())
        handleItem(item);
}

QList<AlbumInfoPtr> AlbumListJob::list() const
{
    return d->list;
}

} /* namespace Vkontakte */

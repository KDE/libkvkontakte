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

#include "albumlistjob.h"
#include "util.h"

#include <QJsonArray>
#include <QJsonObject>

namespace Vkontakte
{

class Q_DECL_HIDDEN AlbumListJob::Private
{
public:
    QList<AlbumInfo> list;
};

AlbumListJob::AlbumListJob(const QString &accessToken, int uid, const QList<int> &aids)
    : VkontakteJob(accessToken, QStringLiteral("photos.getAlbums"))
    , d(new Private)
{
    if (uid != -1)
        addQueryItem(QStringLiteral("uid"), QString::number(uid));
    if (!aids.empty())
        addQueryItem(QStringLiteral("aids"), joinIntegers(aids));
}

AlbumListJob::~AlbumListJob()
{
    delete d;
}

void AlbumListJob::handleData(const QJsonValue &data)
{
    if (!data.isArray())
    {
        // TODO: report error!!!
        return;
    }

    foreach (const QJsonValue &item, data.toArray())
    {
        if (!item.isObject())
        {
            // TODO: report error!!!
            d->list.clear();
            return;
        }

        d->list.append(AlbumInfo(item.toObject()));
    }
}

QList<AlbumInfo> AlbumListJob::list() const
{
    return d->list;
}

} /* namespace Vkontakte */

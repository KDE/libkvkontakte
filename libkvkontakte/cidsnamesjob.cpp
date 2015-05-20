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

#include "cidsnamesjob.h"

#include <QtCore/QMap>
#include <QtCore/QVariant>

namespace Vkontakte
{

class CidsNamesJob::Private
{
public:
    QIntList cids;
    QMap<int, QString> names; // cid -> name
};

// https://vk.com/dev/database.getCountriesById
// TODO: access token is not needed for this call
CidsNamesJob::CidsNamesJob(const QString &method,
                           const QString &accessToken,
                           const QIntList &cids)
    : VkontakteJob(accessToken, method)
    , d(new Private)
{
    d->cids = cids;
    addQueryItem("cids", cids.join());
}

CidsNamesJob::~CidsNamesJob()
{
    delete d;
}

void CidsNamesJob::handleData(const QVariant &data)
{
    // We need QMap, because VK server might reorder results
    // (I did not check it, but they do not guarantee the same order as in "cids")
    foreach(const QVariant &item, data.toList())
    {
        QVariantMap item_map = item.toMap();
        d->names[item_map["cid"].toInt()] = item_map["name"].toString();
    }
}

QMap<int, QString> CidsNamesJob::names() const
{
    return d->names;
}

} /* namespace Vkontakte */

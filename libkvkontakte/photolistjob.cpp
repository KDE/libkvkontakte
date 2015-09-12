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

#include "photolistjob.h"
#include "util.h"

#include <QtCore/QJsonArray>

namespace Vkontakte
{

class Q_DECL_HIDDEN PhotoListJob::Private
{
public:
    QList<PhotoInfo> list;
};

// http://vk.com/dev/photos.get
//
// The API also allows to set "limit" and "offset", but that
// does not seem to be useful.
PhotoListJob::PhotoListJob(const QString &accessToken,
                           int uid, int aid, const QList<int> &pids)
    : VkontakteJob(accessToken, QStringLiteral("photos.get"))
    , d(new Private)
{
    addQueryItem(QStringLiteral("uid"), QString::number(uid));
    addQueryItem(QStringLiteral("aid"), QString::number(aid));
    if (!pids.empty())
        addQueryItem(QStringLiteral("pids"), joinIntegers(pids));
}

PhotoListJob::~PhotoListJob()
{
    delete d;
}

void PhotoListJob::handleData(const QJsonValue &data)
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

        d->list.append(PhotoInfo(item.toObject()));
    }
}

QList<PhotoInfo> PhotoListJob::list() const
{
    return d->list;
}

} /* namespace Vkontakte */

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
#include "photolistjob.h"

#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

class PhotoListJob::Private
{
public:
    QList<PhotoInfoPtr> list;
};

// http://vkontakte.ru/developers.php?o=-1&p=photos.get
//
// The API also allows to set "limit" and "offset", but that
// does not seem to be useful.
PhotoListJob::PhotoListJob(const QString &accessToken,
                           int uid, int aid, const QIntList &pids)
    : VkontakteJob(accessToken, "photos.get")
    , d(new Private)
{
    addQueryItem("uid", QString::number(uid));
    addQueryItem("aid", QString::number(aid));
    if (!pids.empty())
        addQueryItem("pids", pids.join());
}

PhotoListJob::~PhotoListJob()
{
    delete d;
}

void PhotoListJob::handleItem(const QVariant &data)
{
    PhotoInfoPtr item(new PhotoInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
    d->list.append(item);
}

void PhotoListJob::handleData(const QVariant &data)
{
    foreach (const QVariant &item, data.toList())
        handleItem(item);
}

QList<PhotoInfoPtr> PhotoListJob::list() const
{
    return d->list;
}

} /* namespace Vkontakte */

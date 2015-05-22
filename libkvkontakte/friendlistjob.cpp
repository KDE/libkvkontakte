/*
 * Copyright (C) 2010  Thomas McGuire <mcguire@kde.org>
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

#include "friendlistjob.h"

#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

class FriendListJob::Private
{
public:
    QList<UserInfoPtr> list;
};

// http://vk.com/dev/friends.get
FriendListJob::FriendListJob(const QString &accessToken, int uid)
    : VkontakteJob(accessToken, "friends.get")
    , d(new Private)
{
    if (uid != -1)
        addQueryItem("uid", QString::number(uid));
    addQueryItem("fields", UserInfo::allQueryFields().join(","));
}

FriendListJob::~FriendListJob()
{
    delete d;
}

QList<UserInfoPtr> FriendListJob::list() const
{
    return d->list;
}

void FriendListJob::handleData(const QVariant &data)
{
    foreach(const QVariant &user, data.toList())
    {
        UserInfoPtr userInfo(new UserInfo());
        QJson::QObjectHelper::qvariant2qobject(user.toMap(), userInfo.data());
        d->list.append(userInfo);
    }
}

} /* namespace Vkontakte */

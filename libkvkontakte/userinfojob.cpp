/*
 * Copyright (C) 2010  Thomas McGuire <mcguire@kde.org>
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

#include "userinfojob.h"

#include <KIO/Job>
#include <KDebug>
#include <KLocale>

#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

class UserInfoJob::Private
{
public:
    QList<UserInfoPtr> userInfo;
    QStringList fields;
};

// http://vk.com/dev/users.get
UserInfoJob::UserInfoJob(const QString &accessToken)
    : VkontakteJob(accessToken, "users.get")
    , d(new Private)
{
    // The complete list of fields
    setFields(UserInfo::allQueryFields()); // TODO: do not pull extra fields by default

    // TODO: support "counters" request (probably in another KJob)
}

UserInfoJob::UserInfoJob(const QString &accessToken, int uid)
    : VkontakteJob(accessToken, "users.get")
    , d(new Private)
{
    setFields(UserInfo::allQueryFields()); // TODO: do not pull extra fields by default
    addQueryItem("user_ids", QString::number(uid));
}

UserInfoJob::UserInfoJob(const QString &accessToken, const QIntList &uids)
    : VkontakteJob(accessToken, "users.get")
    , d(new Private)
{
    setFields(UserInfo::allQueryFields()); // TODO: do not pull extra fields by default
    addQueryItem("user_ids", uids.join());

    // TODO: make this working for more than 1000 uids
    // ("users.get" allows requesting only 1000 users at once)
}

UserInfoJob::~UserInfoJob()
{
    delete d;
}

QList<UserInfoPtr> UserInfoJob::userInfo() const
{
    return d->userInfo;
}

void UserInfoJob::setFields(const QStringList &fields)
{
    d->fields = fields;
}

void UserInfoJob::prepareQueryItems()
{
    if (!d->fields.isEmpty())
        addQueryItem("fields", d->fields.join(","));
}

UserInfoPtr UserInfoJob::handleSingleData(const QVariant &data)
{
     UserInfoPtr userInfo = UserInfoPtr(new UserInfo());
     QJson::QObjectHelper::qvariant2qobject(data.toMap(), userInfo.data());
     return userInfo;
}

void UserInfoJob::handleData(const QVariant &data)
{
    foreach(const QVariant &item, data.toList())
        d->userInfo.append(handleSingleData(item));
}

} /* namespace Vkontakte */

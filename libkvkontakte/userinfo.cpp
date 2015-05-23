/*
 * Copyright (C) 2010, 2011  Thomas McGuire <mcguire@kde.org>
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

#include "userinfo.h"
#include "util.h"

#include <QtCore/QStringList>

namespace Vkontakte
{

class UserInfo::Private : public QSharedData
{
public:
    QJsonObject jsonData;
};

UserInfo::UserInfo()
    : d(new Private)
{
}

UserInfo::UserInfo(const QJsonObject &jsonData)
    : d(new Private)
{
    d->jsonData = jsonData;
}

UserInfo::UserInfo(const UserInfo &other)
{
    d = other.d;
}

UserInfo::~UserInfo()
{
}

UserInfo &UserInfo::operator=(const UserInfo &other)
{
    if (this != &other)
    {
        d = other.d;
    }

    return *this;
}

int UserInfo::userId() const
{
    return d->jsonData.value(QStringLiteral("uid")).toInt(-1);
}

QString UserInfo::firstName() const
{
    return d->jsonData.value(QStringLiteral("first_name")).toString();
}

QString UserInfo::lastName() const
{
    return d->jsonData.value(QStringLiteral("last_name")).toString();
}

QString UserInfo::nickName() const
{
    return d->jsonData.value(QStringLiteral("nick_name")).toString();
}

QString UserInfo::domain() const
{
    return d->jsonData.value(QStringLiteral("domain")).toString();
}

int UserInfo::sex() const
{
    return d->jsonData.value(QStringLiteral("sex")).toInt(-1);
}

bool UserInfo::online() const
{
    return d->jsonData.value(QStringLiteral("online")).toBool(false);
}

// static
QStringList UserInfo::allQueryFields()
{
    QStringList fields;
    fields << "sex"
           << "bdate"
           << "city"
           << "country"
           << "photo_50"
           << "photo_100"
           << "photo_200_orig"
           << "photo_200"
           << "photo_400_orig"
           << "photo_max"
           << "photo_max_orig"
           << "photo_id"
           << "online"
           << "online_mobile"
           << "domain"
           << "has_mobile"
           << "contacts"
           << "connections"
           << "site"
           << "education"
           << "universities"
           << "schools"
           << "can_post"
           << "can_see_all_posts"
           << "can_see_audio"
           << "can_write_private_message"
           << "status"
           << "last_seen"
           << "common_count"
           << "relation"
           << "relatives"
           << "counters"
           << "screen_name"
           << "maiden_name"
           << "timezone"
           << "occupation"
           << "activities"
           << "interests"
           << "music"
           << "movies"
           << "tv"
           << "books"
           << "games"
           << "about"
           << "quotes"
           << "personal"
           << "friends_status";
    return fields;
}

} /* namespace Vkontakte */

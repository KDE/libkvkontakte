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

class Q_DECL_HIDDEN UserInfo::Private : public QSharedData
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
    fields << QStringLiteral("sex")
           << QStringLiteral("bdate")
           << QStringLiteral("city")
           << QStringLiteral("country")
           << QStringLiteral("photo_50")
           << QStringLiteral("photo_100")
           << QStringLiteral("photo_200_orig")
           << QStringLiteral("photo_200")
           << QStringLiteral("photo_400_orig")
           << QStringLiteral("photo_max")
           << QStringLiteral("photo_max_orig")
           << QStringLiteral("photo_id")
           << QStringLiteral("online")
           << QStringLiteral("online_mobile")
           << QStringLiteral("domain")
           << QStringLiteral("has_mobile")
           << QStringLiteral("contacts")
           << QStringLiteral("connections")
           << QStringLiteral("site")
           << QStringLiteral("education")
           << QStringLiteral("universities")
           << QStringLiteral("schools")
           << QStringLiteral("can_post")
           << QStringLiteral("can_see_all_posts")
           << QStringLiteral("can_see_audio")
           << QStringLiteral("can_write_private_message")
           << QStringLiteral("status")
           << QStringLiteral("last_seen")
           << QStringLiteral("common_count")
           << QStringLiteral("relation")
           << QStringLiteral("relatives")
           << QStringLiteral("counters")
           << QStringLiteral("screen_name")
           << QStringLiteral("maiden_name")
           << QStringLiteral("timezone")
           << QStringLiteral("occupation")
           << QStringLiteral("activities")
           << QStringLiteral("interests")
           << QStringLiteral("music")
           << QStringLiteral("movies")
           << QStringLiteral("tv")
           << QStringLiteral("books")
           << QStringLiteral("games")
           << QStringLiteral("about")
           << QStringLiteral("quotes")
           << QStringLiteral("personal")
           << QStringLiteral("friends_status");
    return fields;
}

} /* namespace Vkontakte */

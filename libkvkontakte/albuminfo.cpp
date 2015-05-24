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

#include "albuminfo.h"
#include "util.h"

namespace Vkontakte
{

class AlbumInfo::Private : public QSharedData
{
public:
    QJsonObject jsonData;
};

AlbumInfo::AlbumInfo()
    : d(new Private)
{
}

AlbumInfo::AlbumInfo(const QJsonObject &jsonData)
    : d(new Private)
{
    d->jsonData = jsonData;
}

AlbumInfo::AlbumInfo(const AlbumInfo &other)
{
    d = other.d;
}

AlbumInfo::~AlbumInfo()
{
}

AlbumInfo &AlbumInfo::operator=(const AlbumInfo &other)
{
    if (this != &other)
    {
        d = other.d;
    }

    return *this;
}

int AlbumInfo::aid() const
{
    return d->jsonData.value(QStringLiteral("aid")).toInt(-1);
}

QString AlbumInfo::title() const
{
    return d->jsonData.value(QStringLiteral("title")).toString();
}

QString AlbumInfo::description() const
{
    return d->jsonData.value(QStringLiteral("description")).toString();
}

int AlbumInfo::size() const
{
    return d->jsonData.value(QStringLiteral("size")).toInt(-1);
}

int AlbumInfo::privacy() const
{
    return d->jsonData.value(QStringLiteral("privacy")).toInt(PRIVACY_UNKNOWN);
}

int AlbumInfo::commentPrivacy() const
{
    return d->jsonData.value(QStringLiteral("comment_privacy")).toInt(PRIVACY_UNKNOWN);
}

} /* namespace Vkontakte */

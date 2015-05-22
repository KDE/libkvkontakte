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

#include "albuminfo.h"
#include "util.h"

namespace Vkontakte
{

class AlbumInfo::Private
{
public:
    int aid;
    int thumbId;
    int uid;
    QString title;
    QString description;
    QString dateCreated;
    QString dateUpdated;
    int size; // number of photos in the album
    int privacy;
    int commentPrivacy;
};

AlbumInfo::AlbumInfo()
    : d(new Private)
{
    d->aid = -1;
    d->thumbId = -1;
    d->uid = -1;
    d->size = -1;

    d->privacy = PRIVACY_UNKNOWN;
    d->commentPrivacy = PRIVACY_UNKNOWN;
}

AlbumInfo::~AlbumInfo()
{
    delete d;
}

void AlbumInfo::setAid(int aid)
{
    d->aid = aid;
}

int AlbumInfo::aid() const
{
    return d->aid;
}

void AlbumInfo::setThumbId(int thumbId)
{
    d->thumbId = thumbId;
}

int AlbumInfo::thumbId() const
{
    return d->thumbId;
}

void AlbumInfo::setUid(int uid)
{
    d->uid = uid;
}

int AlbumInfo::uid() const
{
    return d->uid;
}

void AlbumInfo::setTitle(const QString &title)
{
    d->title = title;
}

QString AlbumInfo::title() const
{
    return d->title;
}

void AlbumInfo::setDescription(const QString &description)
{
    d->description = description;
}

QString AlbumInfo::description() const
{
    return d->description;
}

void AlbumInfo::setDateCreatedString(const QString &dateCreatedString)
{
    d->dateCreated = dateCreatedString;
}

QString AlbumInfo::dateCreatedString() const
{
    return d->dateCreated;
}

KDateTime AlbumInfo::dateCreated() const
{
    return unixTimeToKDateTime(d->dateCreated);
}

void AlbumInfo::setDateUpdatedString(const QString &dateUpdatedString)
{
    d->dateUpdated = dateUpdatedString;
}

QString AlbumInfo::dateUpdatedString() const
{
    return d->dateUpdated;
}

KDateTime AlbumInfo::dateUpdated() const
{
    return unixTimeToKDateTime(d->dateUpdated);
}

void AlbumInfo::setSize(int size)
{
    d->size = size;
}

int AlbumInfo::size() const
{
    return d->size;
}

void AlbumInfo::setPrivacy(int privacy)
{
    d->privacy = privacy;
}

int AlbumInfo::privacy() const
{
    return d->privacy;
}

void AlbumInfo::setCommentPrivacy(int commentPrivacy)
{
    d->commentPrivacy = commentPrivacy;
}

int AlbumInfo::commentPrivacy() const
{
    return d->commentPrivacy;
}

} /* namespace Vkontakte */

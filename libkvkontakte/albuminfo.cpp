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
#include "albuminfo.h"

#include "util.h"

#include <KDebug>
#include <KLocalizedString>

namespace Vkontakte
{

AlbumInfo::AlbumInfo()
    : m_aid(-1), m_thumbId(-1), m_uid(-1), m_size(-1)
    , m_privacy(PRIVACY_UNKNOWN), m_commentPrivacy(PRIVACY_UNKNOWN)
    , d(0)
{
}

void AlbumInfo::setAid(int aid)
{
    m_aid = aid;
}

int AlbumInfo::aid() const
{
    return m_aid;
}

void AlbumInfo::setThumbId(int thumbId)
{
    m_thumbId = thumbId;
}

int AlbumInfo::thumbId() const
{
    return m_thumbId;
}

void AlbumInfo::setUid(int uid)
{
    m_uid = uid;
}

int AlbumInfo::uid() const
{
    return m_uid;
}

void AlbumInfo::setTitle(const QString &title)
{
    m_title = title;
}

QString AlbumInfo::title() const
{
    return m_title;
}

void AlbumInfo::setDescription(const QString &description)
{
    m_description = description;
}

QString AlbumInfo::description() const
{
    return m_description;
}

void AlbumInfo::setDateCreatedString(const QString &dateCreatedString)
{
    m_dateCreated = dateCreatedString;
}

QString AlbumInfo::dateCreatedString() const
{
    return m_dateCreated;
}

KDateTime AlbumInfo::dateCreated() const
{
    return unixTimeToKDateTime(m_dateCreated);
}

void AlbumInfo::setDateUpdatedString(const QString &dateUpdatedString)
{
    m_dateUpdated = dateUpdatedString;
}

QString AlbumInfo::dateUpdatedString() const
{
    return m_dateUpdated;
}

KDateTime AlbumInfo::dateUpdated() const
{
    return unixTimeToKDateTime(m_dateUpdated);
}

void AlbumInfo::setSize(int size)
{
    m_size = size;
}

int AlbumInfo::size() const
{
    return m_size;
}

void AlbumInfo::setPrivacy(int privacy)
{
    m_privacy = privacy;
}

int AlbumInfo::privacy() const
{
    return m_privacy;
}

void AlbumInfo::setCommentPrivacy(int commentPrivacy)
{
    m_commentPrivacy = commentPrivacy;
}

int AlbumInfo::commentPrivacy() const
{
    return m_commentPrivacy;
}

} /* namespace Vkontakte */

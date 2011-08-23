/* Copyright 2011 Roeland Jago Douma <unix@rullzer.com>
   Copyright 2011 Alexander Potashev <aspotashev@gmail.com>

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
#include "noteinfo.h"

#include "util.h"

#include <KDebug>
#include <KLocalizedString>

namespace Vkontakte
{

NoteInfo::NoteInfo()
    : m_nid(-1), m_uid(-1), m_ncom(-1), m_readNcom(-1)
    , m_privacy(-1), m_commentPrivacy(-1), m_canComment(false)
    , d(0)
{
}

void NoteInfo::setNid(int nid)
{
    m_nid = nid;
}

int NoteInfo::nid() const
{
    return m_nid;
}

void NoteInfo::setUid(int uid)
{
    m_uid = uid;
}

int NoteInfo::uid() const
{
    return m_uid;
}

void NoteInfo::setTitle(const QString &title)
{
    m_title = title;
}

QString NoteInfo::title() const
{
    return m_title;
}

void NoteInfo::setText(const QString &text)
{
    m_text = text;
}

QString NoteInfo::text() const
{
    return QString(m_text).remove(QChar::ByteOrderMark);
}

void NoteInfo::setTextWiki(const QString &textWiki)
{
    m_textWiki = textWiki;
}

QString NoteInfo::textWiki() const
{
    return m_textWiki;
}

void NoteInfo::setDateString(const QString &createdDate)
{
    m_date = createdDate;
}

QString NoteInfo::dateString() const
{
    return m_date;
}

KDateTime NoteInfo::date() const
{
    return unixTimeToKDateTime(m_date);
}

void NoteInfo::setNcom (int ncom)
{
    m_ncom = ncom;
}

int NoteInfo::ncom() const
{
    return m_ncom;
}

void NoteInfo::setReadNcom (int readNcom)
{
    m_readNcom = readNcom;
}

int NoteInfo::readNcom() const
{
    return m_readNcom;
}

void NoteInfo::setPrivacy(int privacy)
{
    m_privacy = privacy;
}

int NoteInfo::privacy() const
{
    return m_privacy;
}

void NoteInfo::setCommentPrivacy(int commentPrivacy)
{
    m_commentPrivacy = commentPrivacy;
}

int NoteInfo::commentPrivacy() const
{
    return m_commentPrivacy;
}

void NoteInfo::setCanComment(bool canComment)
{
    m_canComment = canComment;
}

bool NoteInfo::canComment() const
{
    return m_canComment;
}

} /* namespace Vkontakte */

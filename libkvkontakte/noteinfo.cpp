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
#include <KPIMUtils/LinkLocator>

KMime::Message::Ptr NoteInfo::asNote() const
{
    KMime::Message * const note = new KMime::Message();

    QString m = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n";
    m += "<html><head></head><body>\n";
    m += text();
    m += "</body>";

    note->setBody( m.toAscii() );
    note->date()->fromUnicodeString( date().toString(KDateTime::RFCDateDay), "utf-8" );
    note->contentType()->fromUnicodeString( "text/html", "utf-8" );
    note->subject()->fromUnicodeString( title(), "utf-8" );
    note->from()->fromUnicodeString( "you@facebook", "utf-8" );

    note->assemble();

    return KMime::Message::Ptr(note);
}

void NoteInfo::setNid(const QString &nid)
{
    m_nid = nid;
}

QString NoteInfo::nid() const
{
    return m_nid;
}

void NoteInfo::setUid(const QString &uid)
{
    m_uid = uid;
}

QString NoteInfo::uid() const
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
    return m_text;
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
    return vkontakteTimeToKDateTime(m_date);
}

int NoteInfo::ncom() const
{
    return m_ncom;
}

int NoteInfo::readNcom() const
{
    return m_readNcom;
}

void NoteInfo::setNcom (int ncom)
{
    m_ncom = ncom;
}

void NoteInfo::setReadNcom (int readNcom)
{
    m_readNcom = readNcom;
}

/* Copyright 2011 Roeland Jago Douma <unix@rullzer.com>

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

#include <dom/html_document.h>
#include <KDebug>
#include <KLocalizedString>
#include <KPIMUtils/LinkLocator>

void NoteInfo::setId( const QString &id)
{
  mId = id;
}

KMime::Message::Ptr NoteInfo::asNote() const
{
  KMime::Message *note = new KMime::Message();

  QString m = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n";
  m += "<html><head></head><body>\n";
  m += message();
  m += "</body>";

  note->setBody( m.toAscii() );
  note->date()->fromUnicodeString( updatedTime().toString(KDateTime::RFCDateDay), "utf-8" );
  note->contentType()->fromUnicodeString( "text/html", "utf-8" );
  note->subject()->fromUnicodeString( subject(), "utf-8" );
  note->from()->fromUnicodeString( "you@facebook", "utf-8" );

  note->assemble();

  return KMime::Message::Ptr(note);
}

QString NoteInfo::id() const
{
  return mId;
}

void NoteInfo::setFrom( const QString &from)
{
  mFrom = from;
}

QString NoteInfo::from() const
{
  return mFrom;
}

void NoteInfo::setSubject( const QString &subject)
{
  mSubject = subject;
}

QString NoteInfo::subject() const
{
  return mSubject;
}

void NoteInfo::setMessage( const QString &message)
{
  mMessage = message;
}

QString NoteInfo::message() const
{
  return mMessage;
}

void NoteInfo::setCreatedTimeString( const QString &createdTime )
{
  mCreatedTime = createdTime;
}

QString NoteInfo::createdTimeString() const
{
  return mCreatedTime;
}

KDateTime NoteInfo::createdTime() const
{
  return facebookTimeToKDateTime(mCreatedTime);
}

void NoteInfo::setUpdatedTimeString( const QString &updatedTime )
{
  mUpdatedTime = updatedTime;
}

QString NoteInfo::updatedTimeString() const
{
  return mUpdatedTime;
}

KDateTime NoteInfo::updatedTime() const
{
  return facebookTimeToKDateTime(mUpdatedTime);
}



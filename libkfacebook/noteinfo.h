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
#ifndef NOTEINFO_H
#define NOTEINFO_H

#include "libkfacebook_export.h"

#include <KMime/Message>
#include <KDateTime>
#include <QObject>

class LIBKFACEBOOK_EXPORT NoteInfo : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString id WRITE setId READ id)
  Q_PROPERTY(QString from WRITE setFrom READ from)
  Q_PROPERTY(QString subject WRITE setSubject READ subject)
  Q_PROPERTY(QString message WRITE setMessage READ message)
  Q_PROPERTY(QString created_time WRITE setCreatedTimeString READ createdTimeString)
  Q_PROPERTY(QString updated_time WRITE setUpdatedTimeString READ updatedTimeString)
  public:
    void setId( const QString &id);
    QString id() const;

    void setFrom( const QString &from);
    QString from() const;

    void setSubject( const QString &subject);
    QString subject() const;

    void setMessage( const QString &message);
    QString message() const;

    void setCreatedTimeString( const QString &createdTime );
    QString createdTimeString() const;
    KDateTime createdTime() const;

    void setUpdatedTimeString( const QString &updatedTime );
    QString updatedTimeString() const;
    KDateTime updatedTime() const;

    KMime::Message::Ptr asNote() const;

  private:
    QString mId;
    QString mFrom;
    QString mSubject;
    QString mMessage;
    QString mCreatedTime;
    QString mUpdatedTime;
};

typedef QSharedPointer<NoteInfo> NoteInfoPtr;

#endif

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

/**
 * Class to represent a facebook note
 */
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
    /**
     * Set the facebook id of this note
     * @param id the facebook id
     */
    void setId( const QString &id);
    /**
     * Returns the facebook id
     */
    QString id() const;

    /**
     * Set who wrote the note
     * @param from the creator of the note
     */
    void setFrom( const QString &from);
    /**
     * Returns the creator of the note
     */
    QString from() const;

    /**
     * Set the subject of the note
     * @param subject the subject
     */
    void setSubject( const QString &subject);
    /**
     * Returns the subject of the note.
     */
    QString subject() const;

    /**
     * Set the actual content of the note
     * @param message The actual content of the note
     */
    void setMessage( const QString &message);
    /**
     * Returns the content of the note.
     */
    QString message() const;

    /**
     * Set the creation time of the note
     * @param createdTime Time in "facebook format"
     */
    void setCreatedTimeString( const QString &createdTime );
    /**
     * Returns the creation time as a string in "facebook format"
     */
    QString createdTimeString() const;
    /**
     * Returns the creation time in KDateTime
     */
    KDateTime createdTime() const;

    /**
     * Set the time of the last update of the note
     * @param updatedTime The time, in "facebook format", of the last update of
     *                    the note.
     */
    void setUpdatedTimeString( const QString &updatedTime );
    /**
     * Returns the time of the last update of the note in "facebook format"
     */
    QString updatedTimeString() const;
    /**
     * Returns the time of the last update of the note as a KDateTime
     */
    KDateTime updatedTime() const;

    /**
     * Generates a KMime::Message from this note and return a
     * KMime::Message::Ptr to it.
     */
    KMime::Message::Ptr asNote() const;

  private:
    QString mId;          /* Facebook id of the note. */
    QString mFrom;        /* Creator of the note. */
    QString mSubject;     /* Subject of the note. */
    QString mMessage;     /* Actual content of the note. */
    QString mCreatedTime; /* Creation time of the note. */
    QString mUpdatedTime; /* Last update time of the note. */
};

typedef QSharedPointer<NoteInfo> NoteInfoPtr;

#endif

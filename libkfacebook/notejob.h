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
#ifndef NOTEJOB_H
#define NOTEJOB_H

#include "facebookjobs.h"
#include "noteinfo.h"

/**
* A job to retrieve one or multiple notes from facebook and convert them into
* noteInfo objects.
*/
class LIBKFACEBOOK_EXPORT NoteJob : public FacebookGetJob
{
  Q_OBJECT
  public:
    /**
    * @brief Construct a notejob to retrieve multiple notes.
    *
    * @param noteIds A list of ids of notes to retrieve.
    * @param accessToken The token to access data on facebook.
    */
    NoteJob( const QStringList &noteIds, const QString &accessToken );
    
    /**
    * @brief Contrust a notejob to retrieve a single note from facebook.
    *
    * @param noteId The id of the note to retrieve.
    * @param accessToken The token to accesss data on facebook.
    */
    NoteJob( const QString &noteId, const QString &accessToken );

    /**
    * @brief Return a list of all the notes that his job has retrieved
    *
    * @return List of pointers to noteinfo objects
    */
    QList<NoteInfoPtr> noteInfo() const;

  protected:
    /**
    * @brief Handle the data that is returned by the FacebookGetJob
    *
    * @param data A JSON string describing one or multiple notes
    */
    virtual void handleData( const QVariant& data );

  private:
    /**
    * @brief Convert a JSON string to a noteInfo object.
    *
    * @param data JSON string that describes a facebook note.
    *
    * @return Pointer to a noteInfo object
    */
    NoteInfoPtr handleSingleNote( const QVariant& data );

    /**
    * @brief List of pointers to noteInfo objects.
    */
    QList<NoteInfoPtr> mNoteInfo;
    /**
    * @brief Boolean value to indicate if we are retrieving one or multple 
    * notes
    */
    bool mMultiQuery;
};

#endif

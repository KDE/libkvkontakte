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
#ifndef NOTESLISTJOB_H
#define NOTESLISTJOB_H

#include "vkontaktejobs.h"
#include "noteinfo.h"

class LIBKVKONTAKTE_EXPORT NotesListJob : public VkontakteJob
{
    Q_OBJECT
public:
    /**
    * @brief Construct a job to retrieve the data of user with the given uid.
    * @param accessToken The access token to access data on vkontakte.
    * @param uid The user ID
    * @param offset Offset in the list of all notes
    * @param count Number of notes to retrieve (limited to 100)
    **/
    NotesListJob( const QString &accessToken, const QString &uid, int offset, int count );

    QList<NoteInfoPtr> notes() const;
    int totalCount() const;

protected:
    /**
    * @brief Handles the data returned by the VkontakteGetJob
    * @param data A JSON string containing the data.
    */
    virtual void handleData( const QVariant& data );

    void handleItem(const QVariant& data);

private:
    int m_totalCount; // number of all notes, not only notes retrieved in this request
    QList<NoteInfoPtr> m_notes;
};

#endif // NOTESLISTJOB_H

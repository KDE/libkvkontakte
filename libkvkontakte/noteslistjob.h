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

#ifndef NOTESLISTJOB_H
#define NOTESLISTJOB_H

#include "vkontaktejobs.h"
#include "noteinfo.h"

namespace Vkontakte
{

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
    NotesListJob(const QString &accessToken, int uid, int offset, int count);
    ~NotesListJob();

    QList<NoteInfoPtr> list() const;
    int totalCount() const;

protected:
    /**
    * @brief Handles the data returned by the VkontakteGetJob
    * @param data A JSON string containing the data.
    */
    virtual void handleData(const QVariant &data);

    void handleItem(const QVariant &data);

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // NOTESLISTJOB_H

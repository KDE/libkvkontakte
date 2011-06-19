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
#include "noteslistjob.h"
#include <QVariant>
#include <qjson/qobjecthelper.h>

NotesListJob::NotesListJob(const QString& accessToken,
                           const QString& uid, int offset, int count)
    : VkontakteJob ("notes.get", accessToken)
{
    // Not passing "nids", because we want all notes.

    addQueryItem("uid", uid);
    addQueryItem("sort", "1"); // ascending by date
    addQueryItem("count", QString::number(count));
    addQueryItem("offset", QString::number(offset));
}

void NotesListJob::handleItem(const QVariant& data)
{
    NoteInfoPtr noteInfo(new NoteInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), noteInfo.data());
    m_notes.append(noteInfo);
}

void NotesListJob::handleData(const QVariant& data)
{
    QVariantList notes = data.toList();
    m_totalCount = notes[0].toInt();
    notes.pop_front();
    foreach (const QVariant &item, notes) {
        handleItem(item);
    }
}

QList<NoteInfoPtr> NotesListJob::notes() const
{
    return m_notes;
}

int NotesListJob::totalCount() const
{
    return m_totalCount;
}

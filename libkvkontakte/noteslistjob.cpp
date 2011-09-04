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

#include <QtCore/QVariant>
#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

class NotesListJob::Private
{
public:
    int totalCount; // number of all notes, not only notes retrieved in this request
    QList<NoteInfoPtr> list;
};

NotesListJob::NotesListJob(const QString &accessToken,
                           int uid, int offset, int count)
    : VkontakteJob(accessToken, "notes.get")
    , d(new Private)
{
    // Not passing "nids", because we want all notes.

    addQueryItem("uid", QString::number(uid));
    addQueryItem("sort", "1"); // ascending by date
    addQueryItem("count", QString::number(count));
    addQueryItem("offset", QString::number(offset));
}

NotesListJob::~NotesListJob()
{
    delete d;
}

void NotesListJob::handleItem(const QVariant &data)
{
    NoteInfoPtr item(new NoteInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
    d->list.append(item);
}

void NotesListJob::handleData(const QVariant &data)
{
    QVariantList list = data.toList();
    d->totalCount = list[0].toInt();
    list.pop_front();
    foreach (const QVariant &item, list)
        handleItem(item);
}

QList<NoteInfoPtr> NotesListJob::list() const
{
    return d->list;
}

int NotesListJob::totalCount() const
{
    return d->totalCount;
}

} /* namespace Vkontakte */

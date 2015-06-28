/*
 * Copyright (C) 2011, 2015  Alexander Potashev <aspotashev@gmail.com>
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

#include "noteslistjob.h"

#include <qjson/qobjecthelper.h>

#include <QtCore/QVariant>

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

    if (uid > 0) {
        addQueryItem("user_id", QString::number(uid));
    }

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
    foreach(const QVariant &item, list)
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

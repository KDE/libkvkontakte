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

#include "notejob.h"

#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

class NoteJob::Private
{
public:
    NoteInfoPtr noteInfo;
};

// http://vk.com/dev/notes.getById
NoteJob::NoteJob(const QString &accessToken, int nid)
    : VkontakteJob(accessToken, "notes.getById")
    , d(new Private)
{
    addQueryItem("nid", QString::number(nid));
    addQueryItem("need_wiki", "1"); // works only for current user's notes
}

NoteJob::~NoteJob()
{
    delete d;
}

void NoteJob::handleData(const QVariant &data)
{
    d->noteInfo = NoteInfoPtr(new NoteInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), d->noteInfo.data());
}

NoteInfoPtr NoteJob::noteInfo()
{
    return d->noteInfo;
}

} /* namespace Vkontakte */

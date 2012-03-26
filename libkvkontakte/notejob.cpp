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
#include "notejob.moc"

#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

class NoteJob::Private
{
public:
    NoteInfoPtr noteInfo;
};

// http://vkontakte.ru/developers.php?o=-1&p=notes.getById
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

/* Copyright 2011 Roeland Jago Douma <unix@rullzer.com>
   Copyright 2011 Alexander Potashev <aspotashev@gmail.com>

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
#include "noteaddjob.h"

#include <QtCore/QVariant>

namespace Vkontakte
{

class NoteAddJob::Private
{
public:
    int nid;
};

NoteAddJob::NoteAddJob(const QString &accessToken, const QString &title, const QString &text)
    : VkontakteJob(accessToken, "notes.add", true)
    , d(new Private)
{
    d->nid = -1;

    addQueryItem("title", title);
    addQueryItem("text", text);
    addQueryItem("privacy", "3");
}

NoteAddJob::~NoteAddJob()
{
    delete d;
}

void NoteAddJob::handleData(const QVariant &data)
{
    d->nid = data.toMap()["nid"].toInt();
}

int NoteAddJob::nid() const
{
    return d->nid;
}

} /* namespace Vkontakte */

#include "noteaddjob.moc"

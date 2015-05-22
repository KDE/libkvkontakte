/*
 * Copyright (C) 2011  Roeland Jago Douma <unix@rullzer.com>
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

#include "noteinfo.h"
#include "util.h"

#include <KLocalizedString>

namespace Vkontakte
{

class NoteInfo::Private
{
public:
    int nid;          /* Vkontakte id of the note. */
    int uid;        /* Creator of the note. */
    QString title;     /* Subject of the note. */
    QString text;     /* Actual content of the note. */
    QString textWiki;
    QString date; /* Creation time of the note. */
    int ncom; /* Number of comments. */
    int readNcom; /* Number of read comments. */
    int privacy;
    int commentPrivacy;
    int canComment;
};

NoteInfo::NoteInfo()
    : d(new Private)
{
    d->nid = -1;
    d->uid = -1;
    d->ncom = -1;
    d->readNcom = -1;
    d->privacy = -1;
    d->commentPrivacy = -1;
    d->canComment = false;
}

NoteInfo::~NoteInfo()
{
    delete d;
}

void NoteInfo::setNid(int nid)
{
    d->nid = nid;
}

int NoteInfo::nid() const
{
    return d->nid;
}

void NoteInfo::setUid(int uid)
{
    d->uid = uid;
}

int NoteInfo::uid() const
{
    return d->uid;
}

void NoteInfo::setTitle(const QString &title)
{
    d->title = title;
}

QString NoteInfo::title() const
{
    return d->title;
}

void NoteInfo::setText(const QString &text)
{
    d->text = text;
}

QString NoteInfo::text() const
{
    return QString(d->text).remove(QChar::ByteOrderMark);
}

void NoteInfo::setTextWiki(const QString &textWiki)
{
    d->textWiki = textWiki;
}

QString NoteInfo::textWiki() const
{
    return d->textWiki;
}

void NoteInfo::setDateString(const QString &createdDate)
{
    d->date = createdDate;
}

QString NoteInfo::dateString() const
{
    return d->date;
}

KDateTime NoteInfo::date() const
{
    return unixTimeToKDateTime(d->date);
}

void NoteInfo::setNcom (int ncom)
{
    d->ncom = ncom;
}

int NoteInfo::ncom() const
{
    return d->ncom;
}

void NoteInfo::setReadNcom (int readNcom)
{
    d->readNcom = readNcom;
}

int NoteInfo::readNcom() const
{
    return d->readNcom;
}

void NoteInfo::setPrivacy(int privacy)
{
    d->privacy = privacy;
}

int NoteInfo::privacy() const
{
    return d->privacy;
}

void NoteInfo::setCommentPrivacy(int commentPrivacy)
{
    d->commentPrivacy = commentPrivacy;
}

int NoteInfo::commentPrivacy() const
{
    return d->commentPrivacy;
}

void NoteInfo::setCanComment(bool canComment)
{
    d->canComment = canComment;
}

bool NoteInfo::canComment() const
{
    return d->canComment;
}

} /* namespace Vkontakte */

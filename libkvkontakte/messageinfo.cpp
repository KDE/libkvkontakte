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
#include "messageinfo.h"
#include "util.h"

#include <QtCore/QRegExp>

namespace Vkontakte
{

class MessageInfo::Private
{
public:
    QString date;
    int uid;
    int mid;
    QString title;
    QString body;
    int readState;
    int out;

    QString chatId;
    QString chatActive;
};

MessageInfo::MessageInfo()
    : d(new Private)
{
    d->uid = -1;
    d->mid = -1;
    d->readState = 0;
    d->out = 0;
}

MessageInfo::~MessageInfo()
{
    delete d;
}

void MessageInfo::setDateString(const QString &createdDate)
{
    d->date = createdDate;
}

QString MessageInfo::dateString() const
{
    return d->date;
}

KDateTime MessageInfo::date() const
{
    return unixTimeToKDateTime(d->date);
}

void MessageInfo::setUid(int uid)
{
    d->uid = uid;
}

int MessageInfo::uid() const
{
    return d->uid;
}

void MessageInfo::setMid(int mid)
{
    d->mid = mid;
}

int MessageInfo::mid() const
{
    return d->mid;
}

void MessageInfo::setTitle(const QString &title)
{
    // vkontakte.ru puts "..." into the title when the subject is not specified
    QRegExp rx("(Re(\\(\\d+\\))?: )?( ?)\\.\\.\\.( ?)");
    if (!rx.exactMatch(title))
        d->title = title;
}

QString MessageInfo::title() const
{
    return d->title;
}

QString MessageInfo::coreTitle() const
{
    QString res = d->title;
    res.remove(QRegExp("^Re(\\(\\d+\\))?: "));
    return res;
}

void MessageInfo::setBody(const QString &body)
{
    d->body = body;

    d->body.remove('\n');
    d->body.replace("<br>", "\n");
    d->body.replace("&lt;", "<");
    d->body.replace("&gt;", ">");
    d->body.replace("&quot;", "\"");
    d->body.replace("&amp;", "&");
}

QString MessageInfo::body() const
{
    return d->body;
}

void MessageInfo::setReadState(int readState)
{
    d->readState = readState;
}

int MessageInfo::readState() const
{
    return d->readState;
}

void MessageInfo::setOut(int out)
{
    d->out = out;
}

int MessageInfo::out() const
{
    return d->out;
}

void MessageInfo::setChatId(const QString &chatId)
{
    d->chatId = chatId;
}

QString MessageInfo::chatId() const
{
    return d->chatId;
}

void MessageInfo::setChatActive(const QString &chatActive)
{
    d->chatActive = chatActive;
}

QString MessageInfo::chatActive() const
{
    return d->chatActive;
}

QString MessageInfo::remoteId() const
{
    return QString("priv_mid%1").arg(mid(), 6, 10, QLatin1Char('0'));
}

MessageInfoPtr::MessageInfoPtr(MessageInfo* ptr)
    : QSharedPointer<MessageInfo>(ptr)
{
}

bool MessageInfoPtr::operator<(const MessageInfoPtr &o) const
{
    return (*this)->mid() < o->mid();
}

} /* namespace Vkontakte */

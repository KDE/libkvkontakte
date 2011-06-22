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
#include <KDebug>

MessageInfo::MessageInfo()
    : m_readState(0), m_out(0)
{
}

void MessageInfo::setDateString(const QString &createdDate)
{
    m_date = createdDate;
}

QString MessageInfo::dateString() const
{
    return m_date;
}

KDateTime MessageInfo::date() const
{
    return unixTimeToKDateTime(m_date);
}

void MessageInfo::setUid(int uid)
{
    m_uid = uid;
}

int MessageInfo::uid() const
{
    return m_uid;
}

void MessageInfo::setMid(int mid)
{
    m_mid = mid;
}

int MessageInfo::mid() const
{
    return m_mid;
}

void MessageInfo::setTitle(const QString &title)
{
    // vkontakte.ru puts "..." into the title when the subject is not specified
    QRegExp rx("(Re(\\(\\d+\\))?: )?( ?)\\.\\.\\.( ?)");
    if (!rx.exactMatch(title))
        m_title = title;
}

QString MessageInfo::title() const
{
    return m_title;
}

void MessageInfo::setBody(const QString &body)
{
    m_body = body;

    m_body.replace("\n", QString());
    m_body.replace("<br>", "\n");
    m_body.replace("&lt;", "<");
    m_body.replace("&gt;", ">");
    m_body.replace("&quot;", "\"");
    m_body.replace("&amp;", "&");
}

QString MessageInfo::body() const
{
    return m_body;
}

void MessageInfo::setReadState(int readState)
{
    m_readState = readState;
}

int MessageInfo::readState() const
{
    return m_readState;
}

void MessageInfo::setOut(int out)
{
    m_out = out;
}

int MessageInfo::out() const
{
    return m_out;
}

void MessageInfo::setChatId(const QString &chatId)
{
    m_chatId = chatId;
}

QString MessageInfo::chatId() const
{
    return m_chatId;
}

void MessageInfo::setChatActive(const QString &chatActive)
{
    m_chatActive = chatActive;
}

QString MessageInfo::chatActive() const
{
    return m_chatActive;
}

QString MessageInfo::remoteId() const
{
    return QString("priv_mid%1").arg(mid(), 6, 10, QLatin1Char('0'));
}

KMime::Message::Ptr MessageInfo::asMessage(QString userAddress, QString ownAddress,
                                           QString messageId, QString inReplyTo) const
{
    if (userAddress.isEmpty())
        userAddress = QString("<unknown@vkontakte>");
    if (ownAddress.isEmpty())
        ownAddress = QString("<you@vkontakte>");

    // http://api.kde.org/4.x-api/kdepimlibs-apidocs/kmime/html/classKMime_1_1Message.html#a5614aa32a42b034f5290d6d7a56cc433
    KMime::Message *mail = new KMime::Message();

    mail->from()->fromUnicodeString( m_out ? ownAddress : userAddress, "utf-8" );
    mail->to()->fromUnicodeString( !m_out ? ownAddress : userAddress, "utf-8" );
    mail->date()->setDateTime( date() );
    mail->subject()->fromUnicodeString( title(), "utf-8" );

    // http://api.kde.org/4.x-api/kdepimlibs-apidocs/kmime/html/index.html
    // This snippet was written by Thomas McGuire
    mail->contentType()->setMimeType( "text/plain" );
    mail->contentType()->setCharset("utf-8");
    mail->fromUnicodeString( body() );
    mail->contentTransferEncoding()->setEncoding(KMime::Headers::CEbase64);

    if (!messageId.isEmpty())
        mail->messageID()->from7BitString(messageId.toAscii());
    if (!inReplyTo.isEmpty()) {
        mail->inReplyTo()->from7BitString(inReplyTo.toAscii());
        mail->references()->from7BitString(inReplyTo.toAscii());
    }

    mail->assemble();

    return KMime::Message::Ptr(mail);
}

MessageInfoPtr::MessageInfoPtr(MessageInfo* ptr)
    : QSharedPointer<MessageInfo>(ptr)
{
}

bool MessageInfoPtr::operator<(const MessageInfoPtr &o) const
{
    return (*this)->mid() < o->mid();
}

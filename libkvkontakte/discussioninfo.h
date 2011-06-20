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
#ifndef DISCUSSIONINFO_H
#define DISCUSSIONINFO_H

#include "libkvkontakte_export.h"

#include <KABC/Addressee>
#include <KDateTime>
#include <QSharedPointer>
#include <QDate>

/**
* Class that describes a discussion on vkontakte.ru
*/
class LIBKVKONTAKTE_EXPORT DiscussionInfo : public QObject
{
    Q_OBJECT

    // date, uid, mid, title, body, read_state, out
    Q_PROPERTY(QString date WRITE setDateString READ dateString)
    Q_PROPERTY(QString uid WRITE setUid READ uid)
    Q_PROPERTY(QString mid WRITE setMid READ mid)
    Q_PROPERTY(QString title WRITE setTitle READ title)
    Q_PROPERTY(QString body WRITE setBody READ body)
    Q_PROPERTY(int read_state WRITE setReadState READ readState)
    Q_PROPERTY(int out WRITE setOut READ out)

    // For chats: chat_id, chat_active
    Q_PROPERTY(QString chat_id WRITE setChatId READ chatId)
    Q_PROPERTY(QString chat_active WRITE setChatActive READ chatActive)

public:
    DiscussionInfo();

    /**
     * Set the creation time of the note
     * @param createdTime Time as UNIX timestamp
     */
    void setDateString(const QString &createdDate);
    /**
     * Returns the creation date/time as a UNIX timestamp
     */
    QString dateString() const;
    /**
     * Returns the creation date/time in KDateTime
     */
    KDateTime date() const;

    /**
    * @brief Sets the facebook id of a person
    * @param id The Vkontakte id
    */
    void setUid(const QString &uid);
    /**
    * @return The vkontakte id of this person
    */
    QString uid() const;

    void setMid(const QString &mid);
    QString mid() const;

    void setTitle(const QString &title);
    QString title() const;

    void setBody(const QString &body);
    QString body() const;

    void setReadState(int readState);
    int readState() const;

    void setOut(int out);
    int out() const;

    void setChatId(const QString &chatId);
    QString chatId() const;

    void setChatActive(const QString &chatActive);
    QString chatActive() const;

private:
    QString m_date;
    QString m_uid;
    QString m_mid;
    QString m_title;
    QString m_body;
    int m_readState;
    int m_out;

    QString m_chatId;
    QString m_chatActive;
};

typedef QSharedPointer<DiscussionInfo> DiscussionInfoPtr;

#endif // DISCUSSIONINFO_H

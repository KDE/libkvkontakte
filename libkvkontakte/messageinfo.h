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

#ifndef MESSAGEINFO_H
#define MESSAGEINFO_H

#include "libkvkontakte_export.h"

#include <KDE/KDateTime>

namespace Vkontakte
{

/**
 * Class that describes a private message on vk.com
 */
class LIBKVKONTAKTE_EXPORT MessageInfo : public QObject
{
    Q_OBJECT

    // date, uid, mid, title, body, read_state, out
    Q_PROPERTY(QString date WRITE setDateString READ dateString)
    Q_PROPERTY(int uid WRITE setUid READ uid)
    Q_PROPERTY(int mid WRITE setMid READ mid)
    Q_PROPERTY(QString title WRITE setTitle READ title)
    Q_PROPERTY(QString body WRITE setBody READ body)
    Q_PROPERTY(int read_state WRITE setReadState READ readState)
    Q_PROPERTY(int out WRITE setOut READ out) // do non-discussion messages have this?

    // For chats: chat_id, chat_active
    Q_PROPERTY(QString chat_id WRITE setChatId READ chatId)
    Q_PROPERTY(QString chat_active WRITE setChatActive READ chatActive)

public:
    MessageInfo();
    ~MessageInfo();

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

    void setUid(int uid);
    int uid() const;

    void setMid(int mid);
    int mid() const;

    void setTitle(const QString &title);
    QString title() const;

    /**
     * @brief Returns the title without "Re(n): " prefix
     **/
    QString coreTitle() const;

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

    QString remoteId() const;

private:
    class Private;
    Private * const d;
};

/**
 * @brief This class is needed for using qSort on QLists of MessageInfoPtrs
 **/
class MessageInfoPtr : public QSharedPointer<MessageInfo>
{
public:
    explicit MessageInfoPtr(MessageInfo *ptr);

    /**
     * @brief Compares by message ID (MessageInfo::m_mid)
     **/
    bool operator<(const MessageInfoPtr &o) const;
};

} /* namespace Vkontakte */

#endif // MESSAGEINFO_H

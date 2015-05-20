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

#ifndef MESSAGESLISTJOB_H
#define MESSAGESLISTJOB_H

#include "vkontaktejobs.h"
#include "messageinfo.h"

namespace Vkontakte
{

// TODO: do not export in KF5 version
class LIBKVKONTAKTE_EXPORT MessagesListJob : public VkontakteJob
{
public:
    /**
     * @brief Constructs a job to retrieve an array of messages from vk.com.
     *
     * @param accessToken vk.com access token.
     * @param out 1 if you want outgoing messages. Defaults to 0.
     * @param offset Offset in the list of all discussions. Defaults to 0.
     * @param count Number of discussions to retrieve. Defaults to 100.
     * @param previewLength 0 means to not truncate messages. Defaults to 0.
     * @param filters ... Defaults to 0.
     * @param timeOffset ... Defaults to 0.
     **/
    explicit MessagesListJob(const QString &accessToken,
                    int out = 0,
                    int offset = 0, int count = 100, int previewLength = 0,
                    int filters = 0, int timeOffset = 0);
    ~MessagesListJob();

    QList<MessageInfoPtr> list() const;
    int totalCount() const;

    /**
     * @brief Used by AllMessagesListJob
     **/
    int out() const;

protected:
    virtual void handleData(const QVariant &data);

    void handleItem(const QVariant &data);

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // MESSAGESLISTJOB_H

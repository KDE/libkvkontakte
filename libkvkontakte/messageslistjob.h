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
#ifndef MESSAGESLISTJOB_H
#define MESSAGESLISTJOB_H

#include <libkvkontakte/vkontaktejobs.h>
#include <libkvkontakte/messageinfo.h>

namespace Vkontakte
{

class LIBKVKONTAKTE_EXPORT MessagesListJob : public VkontakteJob
{
public:
    /**
     * @brief Constructs a job to retrieve an array of messages from vkontakte.ru.
     *
     * @param accessToken vkontakte.ru access token.
     * @param out 1 if you want outgoing messages. Defaults to 0.
     * @param offset Offset in the list of all discussions. Defaults to 0.
     * @param count Number of discussions to retrieve. Defaults to 100.
     * @param previewLength 0 means to not truncate messages. Defaults to 0.
     * @param filters ... Defaults to 0.
     * @param timeOffset ... Defaults to 0.
     **/
    MessagesListJob(const QString &accessToken,
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

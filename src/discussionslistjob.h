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

#ifndef DISCUSSIONSLISTJOB_H
#define DISCUSSIONSLISTJOB_H

#include "vkontaktejobs.h"
#include "messageinfo.h"

namespace Vkontakte
{

class LIBKVKONTAKTE_EXPORT DiscussionsListJob : public VkontakteJob
{
public:
    /**
     * @brief Constructs a job to retrieve an array of discussions from vk.com.
     *
     * @param accessToken vk.com access token.
     * @param offset Offset in the list of all discussions. Defaults to 0.
     * @param count Number of discussions to retrieve. Defaults to 100.
     * @param previewLength 0 means to not truncate messages. Defaults to 0.
     **/
    explicit DiscussionsListJob(const QString &accessToken,
                       int offset = 0, int count = 100, int previewLength = 0);
    ~DiscussionsListJob();

    QList<MessageInfoPtr> list() const;
    int totalCount() const;

protected:
    virtual void handleData(const QVariant &data);

    void handleItem(const QVariant &data);

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // DISCUSSIONSLISTJOB_H

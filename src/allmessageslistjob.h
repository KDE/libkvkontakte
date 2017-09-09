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

#ifndef ALLMESSAGESLISTJOB_H
#define ALLMESSAGESLISTJOB_H

#include "messageslistjob.h"

namespace Vkontakte
{

class LIBKVKONTAKTE_EXPORT AllMessagesListJob : public KJobWithSubjobs
{
    Q_OBJECT
public:
    /**
     * @brief ...
     *
     * @param accessToken ...
     * @param out -1 means to retrieve both incoming and outgoing messages. Defaults to -1.
     * @param previewLength ... Defaults to 0.
     * @param filters ... Defaults to 0.
     * @param timeOffset ... Defaults to 0.
     **/
    explicit AllMessagesListJob(const QString &accessToken,
                       int out = -1, int previewLength = 0,
                       int filters = 0, int timeOffset = 0);
    ~AllMessagesListJob();

    void start() Q_DECL_OVERRIDE;

    QList<MessageInfoPtr> list() const;
    int count() const;

protected:
    void startNewJob(int offset, int count, int out);

private Q_SLOTS:
    void jobFinished(KJob *kjob);

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // ALLMESSAGESLISTJOB_H

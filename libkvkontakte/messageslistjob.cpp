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

#include "messageslistjob.h"

#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

class MessagesListJob::Private
{
public:
    int out;

    int totalCount; // number of all messages, not only messages retrieved in this request
    QList<MessageInfoPtr> list;
};

MessagesListJob::MessagesListJob(const QString &accessToken,
                                 int out,
                                 int offset, int count, int previewLength,
                                 int filters, int timeOffset)
    : VkontakteJob(accessToken, "messages.get")
    , d(new Private)
{
    d->out = out;
    d->totalCount = 0;

    addQueryItem("out", QString::number(out));
    addQueryItem("offset", QString::number(offset));
    addQueryItem("count", QString::number(count));
    addQueryItem("filters", QString::number(filters));
    addQueryItem("preview_length", QString::number(previewLength));
    addQueryItem("time_offset", QString::number(timeOffset));
}

MessagesListJob::~MessagesListJob()
{
    delete d;
}

void MessagesListJob::handleItem(const QVariant &data)
{
    MessageInfoPtr item(new MessageInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
    d->list.append(item);
}

void MessagesListJob::handleData(const QVariant &data)
{
    QVariantList list = data.toList();
    d->totalCount = list[0].toInt();
    list.pop_front();
    foreach(const QVariant &item, list)
        handleItem(item);
}

QList<MessageInfoPtr> MessagesListJob::list() const
{
    return d->list;
}

int MessagesListJob::totalCount() const
{
    return d->totalCount;
}

int MessagesListJob::out() const
{
    return d->out;
}

} /* namespace Vkontakte */

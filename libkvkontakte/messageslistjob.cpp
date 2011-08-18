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
#include "messageslistjob.h"
#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

MessagesListJob::MessagesListJob(const QString &accessToken,
                                 int out,
                                 int offset, int count, int previewLength,
                                 int filters, int timeOffset)
    : VkontakteJob(accessToken, "messages.get")
    , m_out(out)
    , d(0)
{
    addQueryItem("out", QString::number(out));
    addQueryItem("offset", QString::number(offset));
    addQueryItem("count", QString::number(count));
    addQueryItem("filters", QString::number(filters));
    addQueryItem("preview_length", QString::number(previewLength));
    addQueryItem("time_offset", QString::number(timeOffset));
}

void MessagesListJob::handleItem(const QVariant &data)
{
    MessageInfoPtr item(new MessageInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
    m_list.append(item);
}

void MessagesListJob::handleData(const QVariant &data)
{
    QVariantList list = data.toList();
    m_totalCount = list[0].toInt();
    list.pop_front();
    foreach (const QVariant &item, list)
        handleItem(item);
}

QList<MessageInfoPtr> MessagesListJob::list() const
{
    return m_list;
}

int MessagesListJob::totalCount() const
{
    return m_totalCount;
}

int MessagesListJob::out() const
{
    return m_out;
}

} /* namespace Vkontakte */

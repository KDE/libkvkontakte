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
#include "discussionslistjob.h"
#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

DiscussionsListJob::DiscussionsListJob(const QString &accessToken, int offset, int count, int previewLength)
    : VkontakteJob(accessToken, "messages.getDialogs")
    , d(0)
{
    addQueryItem("offset", QString::number(offset));
    addQueryItem("count", QString::number(count));
    addQueryItem("preview_length", QString::number(previewLength));
}

void DiscussionsListJob::handleItem(const QVariant &data)
{
    MessageInfoPtr item(new MessageInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
    m_list.append(item);
}

void DiscussionsListJob::handleData(const QVariant &data)
{
    QVariantList list = data.toList();
    m_totalCount = list[0].toInt();
    list.pop_front();
    foreach (const QVariant &item, list)
        handleItem(item);

    qSort(m_list); // sort by message ID (which should be equivalent to sorting by date)
}

QList<MessageInfoPtr> DiscussionsListJob::list() const
{
    return m_list;
}

int DiscussionsListJob::totalCount() const
{
    return m_totalCount;
}

} /* namespace Vkontakte */

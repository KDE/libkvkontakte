#include "messageslistjob.h"
#include <qjson/qobjecthelper.h>

MessagesListJob::MessagesListJob(const QString &accessToken,
                                 int out,
                                 int offset, int count, int previewLength,
                                 int filters, int timeOffset)
    : VkontakteJob("messages.get", accessToken)
{
    addQueryItem("out", QString::number(out));
    addQueryItem("offset", QString::number(offset));
    addQueryItem("count", QString::number(count));
    addQueryItem("filters", QString::number(filters));
    addQueryItem("preview_length", QString::number(previewLength));
    addQueryItem("time_offset", QString::number(timeOffset));
}

void MessagesListJob::handleItem(const QVariant& data)
{
    MessageInfoPtr item(new MessageInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
    m_list.append(item);
}

void MessagesListJob::handleData(const QVariant& data)
{
    QVariantList list = data.toList();
    m_totalCount = list[0].toInt();
    list.pop_front();
    foreach (const QVariant &item, list) {
        handleItem(item);
    }
}

QList<MessageInfoPtr> MessagesListJob::list() const
{
    return m_list;
}

int MessagesListJob::totalCount() const
{
    return m_totalCount;
}

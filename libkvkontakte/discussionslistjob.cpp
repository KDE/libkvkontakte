#include "discussionslistjob.h"
#include <qjson/qobjecthelper.h>

DiscussionsListJob::DiscussionsListJob(const QString& accessToken, int offset, int count, int previewLength)
    : VkontakteJob("messages.getDialogs", accessToken)
{
    addQueryItem("offset", QString::number(offset));
    addQueryItem("count", QString::number(count));
    addQueryItem("preview_length", QString::number(previewLength));
}

void DiscussionsListJob::handleItem(const QVariant& data)
{
    MessageInfoPtr item(new MessageInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
    m_list.append(item);
}

void DiscussionsListJob::handleData(const QVariant& data)
{
    QVariantList list = data.toList();
    m_totalCount = list[0].toInt();
    list.pop_front();
    foreach (const QVariant &item, list) {
        handleItem(item);
    }

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

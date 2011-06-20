#include <qjson/qobjecthelper.h>
#include "longpollserverjob.h"

LongPollServerJob::LongPollServerJob(const QString& accessToken)
    : VkontakteJob("messages.getLongPollServer", accessToken)
{
}

void LongPollServerJob::handleData(const QVariant& data)
{
     m_serverInfo = LongPollServerInfoPtr(new LongPollServerInfo());
     QJson::QObjectHelper::qvariant2qobject(data.toMap(), m_serverInfo.data());
}

LongPollServerInfoPtr LongPollServerJob::serverInfo()
{
    return m_serverInfo;
}

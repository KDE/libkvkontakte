#ifndef LONGPOLLSERVERJOB_H
#define LONGPOLLSERVERJOB_H

#include <libkvkontakte/vkontaktejobs.h>
#include "longpollserverinfo.h"


class LIBKVKONTAKTE_EXPORT LongPollServerJob : public VkontakteJob
{
public:
    LongPollServerJob(const QString &accessToken);

    LongPollServerInfoPtr serverInfo();

protected:
    virtual void handleData(const QVariant& data);
    
private:
    LongPollServerInfoPtr m_serverInfo;
};

#endif // LONGPOLLSERVERJOB_H

#ifndef MESSAGESLISTJOB_H
#define MESSAGESLISTJOB_H

#include <libkvkontakte/vkontaktejobs.h>
#include <libkvkontakte/messageinfo.h>


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

    QList<MessageInfoPtr> list() const;
    int totalCount() const;

protected:
    virtual void handleData(const QVariant& data);

    void handleItem(const QVariant& data);

private:
    int m_totalCount; // number of all messages, not only messages retrieved in this request
    QList<MessageInfoPtr> m_list;
};

#endif // MESSAGESLISTJOB_H

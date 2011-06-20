#ifndef DISCUSSIONSLISTJOB_H
#define DISCUSSIONSLISTJOB_H

#include <libkvkontakte/vkontaktejobs.h>
#include <libkvkontakte/messageinfo.h>


class LIBKVKONTAKTE_EXPORT DiscussionsListJob : public VkontakteJob
{
public:
    /**
     * @brief Constructs a job to retrieve an array of discussions from vkontakte.ru.
     *
     * @param accessToken vkontakte.ru access token.
     * @param offset Offset in the list of all discussions. Defaults to 0.
     * @param count Number of discussions to retrieve. Defaults to 100.
     * @param previewLength 0 means to not truncate messages. Defaults to 0.
     **/
    DiscussionsListJob(const QString &accessToken,
                       int offset = 0, int count = 100, int previewLength = 0);

    QList<MessageInfoPtr> list() const;
    int totalCount() const;

protected:
    virtual void handleData(const QVariant& data);

    void handleItem(const QVariant& data);

private:
    int m_totalCount; // number of all discussions, not only discussions retrieved in this request
    QList<MessageInfoPtr> m_list;
};

#endif // DISCUSSIONSLISTJOB_H

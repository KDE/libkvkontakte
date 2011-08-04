#ifndef SAVEPHOTOJOB_H
#define SAVEPHOTOJOB_H

#include <QMap>
#include <QVariant>
#include "vkontaktejobs.h"
#include "photoinfo.h"

class LIBKVKONTAKTE_EXPORT SavePhotoJob : public VkontakteJob
{
public:
    SavePhotoJob(const QString &accessToken,
                 const QMap<QString, QVariant> &photoIdData, int gid = -1);

    QList<PhotoInfoPtr> list() const;

protected:
    virtual void handleData(const QVariant &data);
    void handleItem(const QVariant &data);

private:
    QList<PhotoInfoPtr> m_list;
};

#endif // SAVEPHOTOJOB_H

#ifndef SAVEPHOTOJOB_H
#define SAVEPHOTOJOB_H

#include <QMap>
#include <QVariant>
#include "vkontaktejobs.h"

class LIBKVKONTAKTE_EXPORT SavePhotoJob : public VkontakteJob
{
public:
    SavePhotoJob(const QString &accessToken,
                 const QMap<QString, QVariant> &photoIdData, int gid = -1);

protected:
    virtual void handleData(const QVariant &data);

private:
};

#endif // SAVEPHOTOJOB_H

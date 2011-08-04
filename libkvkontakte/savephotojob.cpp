#include "savephotojob.h"

SavePhotoJob::SavePhotoJob(const QString &accessToken,
                           const QMap<QString, QVariant> &photoIdData, int gid)
    : VkontakteJob("photos.save", accessToken, true)
{
    addQueryItem("aid", photoIdData["aid"].toString());
    addQueryItem("server", photoIdData["server"].toString());
    addQueryItem("photos_list", photoIdData["photos_list"].toString());
    addQueryItem("hash", photoIdData["hash"].toString());

    if (gid != -1)
        addQueryItem("gid", QString::number(gid));
}

void SavePhotoJob::handleData(const QVariant &data)
{
    // TODO: class PhotoInfo
    // QJson::QObjectHelper::qvariant2qobject(data.toMap(), item.data());
}

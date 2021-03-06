/*
 * Copyright (C) 2011  Alexander Potashev <aspotashev@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SAVEPHOTOJOB_H
#define SAVEPHOTOJOB_H

#include "vkontaktejobs.h"
#include "photoinfo.h"
#include "uploadphotosjob.h"

#include <QtCore/QVariantMap>
#include <QtCore/QJsonValue>

namespace Vkontakte
{

// This class is not exported, so:
//    - we don't care about ABI of this class (not using Pimpl)
//    - library's users should use class UploadPhotosJob
class SavePhotoJob : public VkontakteJob
{
public:
    SavePhotoJob(const QString &accessToken,
                 UploadPhotosJob::Dest dest,
                 const QVariantMap &photoIdData, int gid = -1);

    QList<PhotoInfo> list() const;

protected:
    QString getMethod(Vkontakte::UploadPhotosJob::Dest dest);
    void handleItem(const QJsonValue &item);
    void handleData(const QJsonValue &data) Q_DECL_OVERRIDE;

private:
    UploadPhotosJob::Dest m_dest;
    QList<PhotoInfo> m_list;
};

} /* namespace Vkontakte */

#endif // SAVEPHOTOJOB_H

/* Copyright 2011, 2012 Alexander Potashev <aspotashev@gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef GETPHOTOUPLOADSERVERJOB_H
#define GETPHOTOUPLOADSERVERJOB_H

#include "vkontaktejobs.h"
#include "uploadphotosjob.h"

namespace Vkontakte
{

// This class is not exported, so:
//    - we don't care about ABI of this class (not using Pimpl)
//    - library's users should use class UploadPhotosJob
class GetPhotoUploadServerJob : public VkontakteJob
{
public:
    GetPhotoUploadServerJob(const QString &accessToken, Vkontakte::UploadPhotosJob::Dest m_dest);

    void initUploadAlbum(int aid, int gid = -1);

    QString uploadUrl() const;

protected:
    static QString getMethod(enum UploadPhotosJob::Dest dest);
    virtual void prepareQueryItems();
    virtual void handleData(const QVariant &data);

private:
    UploadPhotosJob::Dest m_dest;
    QString m_uploadUrl;

    int m_aid;
    int m_gid;
    int m_uid;
};

} /* namespace Vkontakte */

#endif // GETPHOTOUPLOADSERVERJOB_H

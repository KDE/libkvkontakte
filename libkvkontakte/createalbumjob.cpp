/* Copyright 2011 Alexander Potashev <aspotashev@gmail.com>

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
#include "createalbumjob.h"
#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

class CreateAlbumJob::Private
{
public:
    AlbumInfoPtr album;
};

CreateAlbumJob::CreateAlbumJob(const QString &accessToken,
                               const QString &title, const QString &description,
                               int privacy, int comment_privacy)
    : VkontakteJob(accessToken, "photos.createAlbum", true)
    , d(new Private)
{
    addQueryItem("title", title);
    if (!description.isEmpty())
        addQueryItem("description", description);
    if (privacy != AlbumInfo::PRIVACY_UNKNOWN)
        addQueryItem("privacy", QString::number(privacy));
    if (comment_privacy != AlbumInfo::PRIVACY_UNKNOWN)
        addQueryItem("comment_privacy", QString::number(comment_privacy));
}

CreateAlbumJob::~CreateAlbumJob()
{
    delete d;
}

void CreateAlbumJob::handleData(const QVariant &data)
{
    d->album = AlbumInfoPtr(new AlbumInfo());
    QJson::QObjectHelper::qvariant2qobject(data.toMap(), d->album.data());
}

AlbumInfoPtr CreateAlbumJob::album() const
{
    return d->album;
}

} /* namespace Vkontakte */

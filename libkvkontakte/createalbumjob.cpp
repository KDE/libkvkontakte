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

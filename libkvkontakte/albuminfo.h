/*
 * Copyright (C) 2011, 2015  Alexander Potashev <aspotashev@gmail.com>
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

#ifndef ALBUMINFO_H
#define ALBUMINFO_H

#include <QtCore/QJsonObject>
#include <QtCore/QSharedDataPointer>

#include "libkvkontakte_export.h"

namespace Vkontakte
{

// http://vk.com/dev/photos.getAlbums
class LIBKVKONTAKTE_EXPORT AlbumInfo
{
public:
    enum
    {
        PRIVACY_UNKNOWN = -1,
        PRIVACY_PUBLIC = 0,
        PRIVACY_FRIENDS = 1,
        PRIVACY_FRIENDS_OF_FRIENDS = 2,
        PRIVACY_PRIVATE = 3
    };

    AlbumInfo();
    AlbumInfo(const AlbumInfo &other);
    AlbumInfo(const QJsonObject &jsonData);
    ~AlbumInfo();

    AlbumInfo &operator=(const AlbumInfo &other);

    int aid() const;
    QString title() const;
    QString description() const;

    /**
     * @brief Returns number of photos in the album
     */
    int size() const;

    int privacy() const;
    int commentPrivacy() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

} /* namespace Vkontakte */

#endif // ALBUMINFO_H

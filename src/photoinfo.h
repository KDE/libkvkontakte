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

#ifndef PHOTOINFO_H
#define PHOTOINFO_H

#include <QtCore/QJsonObject>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QUrl>

#include "libkvkontakte_export.h"

namespace Vkontakte
{

// http://vk.com/dev/photos.get
class LIBKVKONTAKTE_EXPORT PhotoInfo
{
public:
    PhotoInfo();
    PhotoInfo(const PhotoInfo &other);
    PhotoInfo(const QJsonObject &jsonData);
    ~PhotoInfo();

    PhotoInfo &operator=(const PhotoInfo &other);

    /**
     * @brief Returns the integer ID of the photo
     */
    int pid() const;

    /**
     * @brief Return URL to the image version of maximum resolution
     */
    QUrl urlMaxResolution() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

} /* namespace Vkontakte */

#endif // PHOTOINFO_H

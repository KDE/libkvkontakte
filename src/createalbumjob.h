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

#ifndef CREATEALBUMJOB_H
#define CREATEALBUMJOB_H

#include "vkontaktejobs.h"
#include "albuminfo.h"

namespace Vkontakte
{

// This class is too simple to expand, not using Pimpl here
class LIBKVKONTAKTE_EXPORT CreateAlbumJob : public VkontakteJob
{
public:
    CreateAlbumJob(const QString &accessToken,
                   const QString &title, const QString &description = QString(),
                   int privacy = AlbumInfo::PRIVACY_UNKNOWN,
                   int comment_privacy = AlbumInfo::PRIVACY_UNKNOWN);
    ~CreateAlbumJob();

    AlbumInfo album() const;

protected:
    virtual void handleData(const QJsonValue &data);

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // CREATEALBUMJOB_H
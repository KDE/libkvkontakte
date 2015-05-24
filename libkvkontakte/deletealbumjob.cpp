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

#include "deletealbumjob.h"

#include <QVariant>
#include <KDebug>
#include <KLocalizedString>

namespace Vkontakte
{

DeleteAlbumJob::DeleteAlbumJob(const QString &accessToken, int aid)
    : VkontakteJob(accessToken, "photos.deleteAlbum", true)
    , d(0)
{
    addQueryItem("aid", QString::number(aid));
}

void DeleteAlbumJob::handleData(const QVariant &data)
{
    bool ok = false;
    if (data.toInt(&ok) != 1 || !ok)
    {
        setError(1);
        setErrorText(i18n("Failed to delete album"));
        kWarning() << "Failed to delete album";
    }
}

} /* namespace Vkontakte */

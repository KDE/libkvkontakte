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

#include "getapplicationpermissionsjob.h"

#include <QtCore/QVariant>
#include <QtCore/QJsonValue>

namespace Vkontakte
{

class Q_DECL_HIDDEN GetApplicationPermissionsJob::Private
{
public:
    int permissions;
};

// http://vk.com/dev/account.getAppPermissions
// http://vk.com/dev/permissions
GetApplicationPermissionsJob::GetApplicationPermissionsJob(const QString &accessToken)
    : VkontakteJob(accessToken, QStringLiteral("account.getAppPermissions"))
    , d(new Private)
{
}

GetApplicationPermissionsJob::~GetApplicationPermissionsJob()
{
    delete d;
}

void GetApplicationPermissionsJob::handleData(const QJsonValue &data)
{
    d->permissions = data.toInt(-1);

    // TODO: return error (call handleError?) when "data" is not an integer
}

int GetApplicationPermissionsJob::permissions() const
{
    return d->permissions;
}

} /* namespace Vkontakte */

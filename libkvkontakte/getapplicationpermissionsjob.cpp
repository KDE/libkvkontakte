/* Copyright 2011, 2015  Alexander Potashev <aspotashev@gmail.com>

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
#include "getapplicationpermissionsjob.h"

#include <QtCore/QVariant>

namespace Vkontakte
{

class GetApplicationPermissionsJob::Private
{
public:
    int permissions;
};

// http://vk.com/dev/account.getAppPermissions
// http://vk.com/dev/permissions
GetApplicationPermissionsJob::GetApplicationPermissionsJob(const QString &accessToken)
    : VkontakteJob(accessToken, "account.getAppPermissions")
    , d(new Private)
{
}

GetApplicationPermissionsJob::~GetApplicationPermissionsJob()
{
    delete d;
}

void GetApplicationPermissionsJob::handleData(const QVariant &data)
{
    d->permissions = data.toInt();
}

int GetApplicationPermissionsJob::permissions() const
{
    return d->permissions;
}

} /* namespace Vkontakte */

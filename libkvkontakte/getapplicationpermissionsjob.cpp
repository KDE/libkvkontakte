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
#include "getapplicationpermissionsjob.h"
#include <QVariant>

namespace Vkontakte
{

// http://vkontakte.ru/developers.php?o=-1&p=getUserSettings
// http://vkontakte.ru/developers.php?o=-1&p=%CF%F0%E0%E2%E0%20%EF%F0%E8%EB%EE%E6%E5%ED%E8%E9
GetApplicationPermissionsJob::GetApplicationPermissionsJob(const QString &accessToken)
    : VkontakteJob(accessToken, "getUserSettings")
    , d(0)
{
}

void GetApplicationPermissionsJob::handleData(const QVariant &data)
{
    m_permissions = data.toInt();
}

int GetApplicationPermissionsJob::permissions() const
{
    return m_permissions;
}

} /* namespace Vkontakte */

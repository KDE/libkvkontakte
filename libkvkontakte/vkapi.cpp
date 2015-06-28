/*
 * Copyright (C) 2011, 2012, 2015  Alexander Potashev <aspotashev@gmail.com>
 *
 * This file was a part of kipi-plugins project - http://www.digikam.org
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

#include "vkapi.h"

// LibKvkontakte includes
#include <libkvkontakte/authenticationdialog.h>
#include <libkvkontakte/getapplicationpermissionsjob.h>

// Qt includes
#include <QtCore/QPointer>

namespace Vkontakte
{

class VkApi::Private
{
public:
    QWidget* parent;
    QString appId;
    Vkontakte::AppPermissions::Value requiredPermissions;
    QString accessToken;
    bool authenticated;
};

VkApi::VkApi(QWidget* const parent)
    : d(new Private)
{
    d->parent = parent;
    d->appId = QString();
    d->requiredPermissions = Vkontakte::AppPermissions::NoPermissions;
    d->accessToken = QString();
    d->authenticated = false;
}

VkApi::~VkApi()
{
}

void VkApi::setAppId(const QString &appId)
{
    d->appId = appId;
}

void VkApi::setRequiredPermissions(Vkontakte::AppPermissions::Value permissions)
{
    d->requiredPermissions = permissions;
}

void VkApi::setInitialAccessToken(const QString& accessToken)
{
    // Does nothing if m_accessToken is already set, because this function
    // is only for parameter initialization from a configuration file.
    if (d->accessToken.isEmpty())
        d->accessToken = accessToken;
}

QString VkApi::accessToken() const
{
    return d->accessToken;
}

void VkApi::startAuthentication(bool forceLogout)
{
    if (forceLogout)
        d->accessToken.clear();

    if (!d->accessToken.isEmpty())
    {
        Vkontakte::GetApplicationPermissionsJob* const job = new Vkontakte::GetApplicationPermissionsJob(d->accessToken);

        connect(job, SIGNAL(result(KJob*)),
                this, SLOT(slotApplicationPermissionCheckDone(KJob*)));

        job->start();
    }
    else
    {
        QPointer<Vkontakte::AuthenticationDialog> authDialog = new Vkontakte::AuthenticationDialog(d->parent);
        authDialog->setAppId(d->appId);
        authDialog->setPermissions(d->requiredPermissions);

        connect(authDialog, SIGNAL(authenticated(QString)),
                this, SLOT(slotAuthenticationDialogDone(QString)));

        connect(authDialog, SIGNAL(canceled()),
                this, SIGNAL(canceled()));

        authDialog->start();
    }
}

void VkApi::slotApplicationPermissionCheckDone(KJob* kjob)
{
    Vkontakte::GetApplicationPermissionsJob* const job = dynamic_cast<Vkontakte::GetApplicationPermissionsJob *>(kjob);
    Q_ASSERT(job);

    bool havePermissions = true;
    if (!job || job->error())
    {
        // There was some error enumerating permissions, need to start over for sure
        havePermissions = false;
    }
    else
    {
        Vkontakte::AppPermissions::Value availablePermissions =
            static_cast<Vkontakte::AppPermissions::Value>(job->permissions());

        if ((availablePermissions & d->requiredPermissions) != d->requiredPermissions)
        {
            // Existing permissions are not enough, need to request more permissions
            havePermissions = false;
        }
    }

    if (havePermissions)
    {
        d->authenticated = true;
        emit authenticated();
    }
    else
    {
        startAuthentication(true);
    }
}

void VkApi::slotAuthenticationDialogDone(const QString& accessToken)
{
    d->accessToken   = accessToken;
    d->authenticated = true;
    emit authenticated();
}

bool VkApi::isAuthenticated()
{
    return d->authenticated;
}

} // namespace Vkontakte

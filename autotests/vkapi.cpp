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

#include "vkapi.moc"

// Qt includes

#include <QPointer>

// LibKvkontakte includes

#include <libkvkontakte/authenticationdialog.h>
#include <libkvkontakte/getapplicationpermissionsjob.h>

namespace KIPIVkontaktePlugin
{

VkApi::VkApi(QWidget* const parent)
    : m_parent(parent)
    , m_appId()
    , m_requiredPermissions(Vkontakte::AppPermissions::NoPermissions)
    , m_accessToken()
    , m_authenticated(false)
{
}

VkApi::~VkApi()
{
}

void VkApi::setAppId(const QString &appId)
{
    m_appId = appId;
}

void VkApi::setRequiredPermissions(Vkontakte::AppPermissions::Value permissions)
{
    m_requiredPermissions = permissions;
}

void VkApi::setInitialAccessToken(const QString& accessToken)
{
    // Does nothing if m_accessToken is already set, because this function
    // is only for parameter initialization from a configuration file.
    if (m_accessToken.isEmpty())
        m_accessToken = accessToken;
}

QString VkApi::accessToken() const
{
    return m_accessToken;
}

void VkApi::startAuthentication(bool forceLogout)
{
    if (forceLogout)
        m_accessToken.clear();

    if (!m_accessToken.isEmpty())
    {
        Vkontakte::GetApplicationPermissionsJob* const job = new Vkontakte::GetApplicationPermissionsJob(m_accessToken);

        connect(job, SIGNAL(result(KJob*)),
                this, SLOT(slotApplicationPermissionCheckDone(KJob*)));

        job->start();
    }
    else
    {
        QPointer<Vkontakte::AuthenticationDialog> authDialog = new Vkontakte::AuthenticationDialog(m_parent);
        authDialog->setAppId(m_appId);
        authDialog->setPermissions(m_requiredPermissions);

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

        if ((availablePermissions & m_requiredPermissions) != m_requiredPermissions)
        {
            // Existing permissions are not enough, need to request more permissions
            havePermissions = false;
        }
    }

    if (havePermissions)
    {
        m_authenticated = true;
        emit authenticated();
    }
    else
    {
        startAuthentication(true);
    }
}

void VkApi::slotAuthenticationDialogDone(const QString& accessToken)
{
    m_accessToken   = accessToken;
    m_authenticated = true;
    emit authenticated();
}

bool VkApi::isAuthenticated()
{
    return m_authenticated;
}

} // namespace KIPIVkontaktePlugin

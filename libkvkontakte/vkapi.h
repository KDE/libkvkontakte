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

#ifndef VKAPI_H
#define VKAPI_H

#include "libkvkontakte_export.h"

// LibKvkontakte includes
#include <libkvkontakte/apppermissions.h>

// Qt includes
#include <QObject>

class QWidget;
class KJob;

namespace Vkontakte
{

/**
 * @brief Authentication manager for libkvkontakte
 *
 * This is a handy interface on top of Vkontakte::AuthenticationDialog
 * to request authentication on demand and store the API token for later use.
 */
class LIBKVKONTAKTE_EXPORT VkApi : public QObject
{
    Q_OBJECT

public:
    VkApi(QWidget* const parent);
    ~VkApi();

    /**
     * @brief Initialize app ID.
     *
     * @param appId VK application ID for authentication requests.
     */
    void setAppId(const QString& appId);

    /**
     * @brief Set app permissions to request during authentication.
     *
     * @param permissions Any combination of AppPermissions::*, use bitwise OR to combine them
     */
    void setRequiredPermissions(Vkontakte::AppPermissions::Value permissions);

    /**
     * @brief Set initial access token.
     *
     * If an initial access token was set with this method, then before the
     * normal authentication process VkApi::startAuthentication() will first try
     * to reuse the given token in order to cut down the number of times the
     * authentication dialog is opened.
     */
    void setInitialAccessToken(const QString& accessToken);

    QString accessToken() const;

    void startAuthentication(bool forceLogout);
    bool isAuthenticated();

Q_SIGNALS:
    void authenticated();
    void canceled();

protected Q_SLOTS:
    void slotApplicationPermissionCheckDone(KJob* kjob);
    void slotAuthenticationDialogDone(const QString& accessToken);

private:
    class Private;
    Private * const d;
};

} // namespace Vkontakte

#endif // VKAPI_H

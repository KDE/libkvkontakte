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

#include <libkvkontakte/apppermissions.h>

// Qt includes

#include <QObject>

#include "libkvkontakte_export.h"

class QWidget;
class KJob;

namespace Vkontakte
{

class LIBKVKONTAKTE_EXPORT VkApi : public QObject
{
    Q_OBJECT

public:
    VkApi(QWidget* const parent);
    ~VkApi();

    void setAppId(const QString& appId);
    void setRequiredPermissions(Vkontakte::AppPermissions::Value permissions);

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

/*
 * Copyright (C) 2010  Thomas McGuire <mcguire@kde.org>
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

#ifndef AUTHENTICATIONDIALOG_H
#define AUTHENTICATIONDIALOG_H

#include "libkvkontakte_export.h"

#include <libkvkontakte/apppermissions.h>

#include <KDE/KDialog>

namespace Vkontakte
{

class LIBKVKONTAKTE_EXPORT AuthenticationDialog : public KDialog
{
    Q_OBJECT
public:
    AuthenticationDialog(QWidget *parent);
    ~AuthenticationDialog();

    void setAppId(const QString &appId);
    void Q_DECL_DEPRECATED setPermissions(const QStringList &permissions);
    void setPermissions(Vkontakte::AppPermissions::Value permissions);
    void setDisplayMode(const QString &displayMode);
    void start();

Q_SIGNALS:
    void authenticated(const QString &accessToken);
    void canceled();

private Q_SLOTS:
    void urlChanged(const QUrl &url);
    void loadFinished(bool ok);
    void showErrorDialog();

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // AUTHENTICATIONDIALOG_H

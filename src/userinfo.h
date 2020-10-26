/*
 * Copyright (C) 2010, 2011  Thomas McGuire <mcguire@kde.org>
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

#ifndef USERINFO_H
#define USERINFO_H

#include <QMetaType>
#include <QJsonObject>
#include <QSharedDataPointer>

#include "libkvkontakte_export.h"

namespace Vkontakte
{

// http://vk.com/dev/fields
class LIBKVKONTAKTE_EXPORT UserInfo
{
public:
    enum
    {
        INVALID_TIMEZONE = 42
    };

    UserInfo();
    UserInfo(const UserInfo &other);
    UserInfo(const QJsonObject &jsonData);
    ~UserInfo();

    UserInfo &operator=(const UserInfo &other);

    /**
     * @brief Returns integer user ID at VK.
     *
     * UserInfoJob (the "users.get" method) always returns this, regardless of 
     * the requested additional fields.
     *
     * Returns -1 for uninitialized object or on server error.
     */
    int userId() const;

    /**
     * @brief Returns user's first name in the default language.
     *
     * UserInfoJob (the "users.get" method) always returns this, regardless of 
     * the requested additional fields.
     *
     * Returns empty string for uninitialized object or on server error.
     */
    QString firstName() const;

    /**
     * @brief Returns user's last name in the default language.
     *
     * UserInfoJob (the "users.get" method) always returns this, regardless of 
     * the requested additional fields.
     *
     * Returns empty string for uninitialized object or on server error.
     */
    QString lastName() const;

    QString nickName() const;
    QString domain() const;

    int sex() const;

    bool online() const;

    /**
    * @brief Returns the stringlist of all possible field groups
    * that may be requested in VK API method "users.get".
    *
    * @return The list of strings to pass as "fields" argument to a method.
    **/
    static QStringList allQueryFields();

private:
    class Private;
    QSharedDataPointer<Private> d;
};

} /* namespace Vkontakte */

Q_DECLARE_METATYPE(Vkontakte::UserInfo)

#endif // USERINFO_H

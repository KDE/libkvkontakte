/*
 * Copyright (C) 2011  Thomas McGuire <mcguire@kde.org>
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

#include "util.h"

#include <kdeversion.h>

#if KDE_IS_VERSION(4,6,2)
#include <KDE/KGlobal>
static const KCatalogLoader loader("libkvkontakte");
#endif

KDateTime unixTimeToKDateTime(const QString &unixTime)
{
    KDateTime res;
    res.setTime_t(unixTime.toLongLong());
    return res;
}

static QMap<Vkontakte::AppPermissions::Value, QString> initAppPermissionStrings()
{
    QMap<Vkontakte::AppPermissions::Value, QString> map;

    // http://vk.com/dev/permissions
    map.insert(Vkontakte::AppPermissions::Notify,        "notify");
    map.insert(Vkontakte::AppPermissions::Friends,       "friends");
    map.insert(Vkontakte::AppPermissions::Photos,        "photos");
    map.insert(Vkontakte::AppPermissions::Audio,         "audio");
    map.insert(Vkontakte::AppPermissions::Video,         "video");
    map.insert(Vkontakte::AppPermissions::Offers,        "offers");
    map.insert(Vkontakte::AppPermissions::Questions,     "questions");
    map.insert(Vkontakte::AppPermissions::Pages,         "pages");
    // "applink" permission cannot be requested, I invented
    // the name "applink" myself to somehow identify this permission flag only.
    map.insert(Vkontakte::AppPermissions::AppLink,       "applink");
    map.insert(Vkontakte::AppPermissions::Status,        "status");
    map.insert(Vkontakte::AppPermissions::Notes,         "notes");
    map.insert(Vkontakte::AppPermissions::Messages,      "messages");
    map.insert(Vkontakte::AppPermissions::Wall,          "wall");
    map.insert(Vkontakte::AppPermissions::Ads,           "ads");
    map.insert(Vkontakte::AppPermissions::Offline,       "offline");
    map.insert(Vkontakte::AppPermissions::Docs,          "docs");
    map.insert(Vkontakte::AppPermissions::Groups,        "groups");
    map.insert(Vkontakte::AppPermissions::Notifications, "notifications");
    map.insert(Vkontakte::AppPermissions::Stats,         "stats");
    map.insert(Vkontakte::AppPermissions::Email,         "email");

    return map;
}

// This could be a QList<QPair<A, B> >, but with QMap we can write this shorter
static QMap<Vkontakte::AppPermissions::Value, QString> appPermissionToString = initAppPermissionStrings();

QStringList appPermissionsToStringList(Vkontakte::AppPermissions::Value permissions)
{
    QStringList res;

    foreach (Vkontakte::AppPermissions::Value key, appPermissionToString.keys()) {
        if (permissions & key) {
            res << appPermissionToString.value(key);
        }
    }

    return res;
}

// TODO: remove this function after AuthenticationDialog::setPermissions(QStringList) is removed
Vkontakte::AppPermissions::Value appPermissionsFromStringList(QStringList permissions)
{
    Vkontakte::AppPermissions::Value res = Vkontakte::AppPermissions::NoPermissions;

    foreach (Vkontakte::AppPermissions::Value key, appPermissionToString.keys()) {
        if (permissions.contains(appPermissionToString.value(key))) {
            res |= key;
        }
    }

    return res;
}

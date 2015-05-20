/*
 * Copyright (C) 2015  Alexander Potashev <aspotashev@gmail.com>
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

#ifndef APPPERMISSIONS_H
#define APPPERMISSIONS_H

namespace Vkontakte
{
namespace AppPermissions
{

enum Value
{
    NoPermissions = 0,

    // User allowed to send notifications to him/her
    Notify = 1,
    // Access to friends
    Friends = 2,
    // Access to photos
    Photos = 4,
    // Access to audios
    Audio = 8,
    // Access to videos
    Video = 16,
    // Access to offers (obsolete methods)
    Offers = 32,
    // Access to questions (obsolete methods).
    Questions = 64,
    // Access to wiki pages
    Pages = 128,
    // Addition of link to the application in the left menu
    AppLink = 256,
    // Access to user status
    Status = 1024,
    // Access to user notes
    Notes = 2048,
    // Access to advanced methods for messaging
    Messages = 4096,
    // Access to standard and advanced methods for the wall
    Wall = 8192,
    // Access to advanced methods for Ads API
    Ads = 32768,
    // Access to API at any time from a third party server
    Offline = 65536,
    // Access to documents
    Docs = 131072,
    // Access to user groups
    Groups = 262144,
    // Access to notifications about answers to the user
    Notifications = 524288,
    // Access to statistics of user's own groups and applications
    Stats = 1048576,
    // User e-mail access. Available only for sites.
    Email = 4194304,
}; // enum Value

inline AppPermissions::Value operator|(AppPermissions::Value a, AppPermissions::Value b)
{
    return static_cast<AppPermissions::Value>(static_cast<int>(a) | static_cast<int>(b));
}

inline AppPermissions::Value& operator|=(AppPermissions::Value& a, AppPermissions::Value b)
{
    return a = a | b;
}

} // namespace AppPermissions
} // namespace Vkontakte

#endif // APPPERMISSIONS_H

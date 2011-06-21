/* Copyright 2010 Thomas McGuire <mcguire@kde.org>
   Copyright 2011 Alexander Potashev <aspotashev@gmail.com>

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
#ifndef USERINFOJOB_H
#define USERINFOJOB_H

#include "userinfo.h"
#include "vkontaktejobs.h"
#include "qintlist.h"

/**
 * Get information about multiple users
 *
 * Uses VKontakte API method "getProfiles".
 */
// TODO: support working with more than 1000 users
class LIBKVKONTAKTE_EXPORT UserInfoJob : public VkontakteJob
{
    Q_OBJECT
public:
    /**
    * @brief Construct a job to retrieve the data of the currently authenticaed user.
    * @param accessToken The access token to access data on vkontakte.
    */
    UserInfoJob(const QString &accessToken);

    /**
    * @brief Construct a job to retrieve the data of user with the given uid.
    * @param accessToken The access token to access data on vkontakte.
    * @param uid The user ID
    */
    UserInfoJob(const QString &accessToken, int uid);

    UserInfoJob(const QString &accessToken, const QIntList &uids);

    /**
    * @return A pointer to a userInfo object about the currently authenticated user.
    */
    QList<UserInfoPtr> userInfo() const;

    void setFields(const QStringList &fields);

protected:
    /**
    * @brief Handles the data returned by the VkontakteGetJob
    * @param data A JSON string containing the data.
    */
    virtual void handleData(const QVariant& data);

    UserInfoPtr handleSingleData(const QVariant& data);

    /**
     * @brief Prepares m_queryItems by several addQueryItem calls
     **/
    virtual void prepareQueryItems();

private:
    QList<UserInfoPtr> m_userInfo;
    QIntList m_uids;
    QStringList m_fields;
    bool m_needCityNames;
    bool m_needCountryNames;
};

#endif

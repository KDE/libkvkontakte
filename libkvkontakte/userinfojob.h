/*
 * Copyright (C) 2010  Thomas McGuire <mcguire@kde.org>
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

#ifndef USERINFOJOB_H
#define USERINFOJOB_H

#include "vkontaktejobs.h"
#include "userinfo.h"
#include "qintlist.h"

namespace Vkontakte
{

/**
 * Get information about multiple users
 *
 * Uses VKontakte API method "users.get".
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

    ~UserInfoJob();

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
    virtual void handleData(const QVariant &data);

    UserInfoPtr handleSingleData(const QVariant &data);

    /**
     * @brief Prepares m_queryItems by several addQueryItem calls
     **/
    virtual void prepareQueryItems();

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // USERINFOJOB_H

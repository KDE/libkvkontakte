/* Copyright 2010, 2011 Thomas McGuire <mcguire@kde.org>
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
#ifndef FRIENDJOB_H
#define FRIENDJOB_H

#include "vkontaktejobs.h"
#include "userinfo.h"

/**
 * A job to retrieve the data about one or multiple friends from facebook.
 */
// class LIBKVKONTAKTE_EXPORT FriendJob : public VkontakteJob
// {
//   Q_OBJECT
//   public:
//     /**
//     * @brief Constructor to retrieve multiple friends from facebook.
//     *
//     * @param friendIds A list of ids of the friends you want to retrieve from facebook.
//     * @param accessToken The access token to retrieve the data from facebook.
//     */
//     FriendJob( const QStringList &friendIds, const QString &accessToken );
// 
//     /**
//     * @brief Constructor to retrieve a single friend from facebook.
//     *
//     * @param friendId The id of the friend you want to retrieve data from.
//     * @param accessToken The access token to retrieve the data from facebook.
//     */
//     FriendJob( const QString &friendId, const QString &accessToken );
//     
//     /**
//     * @return A list of pointers to UserInfo objects of all the users (friends)
//     *         that have been retrieved by this job.
//     */
//     QList<UserInfoPtr> friendInfo() const;
// 
//   private:
//     QStringList friendFields() const;
//     void handleSingleData( const QVariant& data );
//     void handlePartner(const UserInfoPtr &userInfo, const QVariant &partner);
//     void handleLocation(const UserInfoPtr &userInfo, const QVariant &data);
//     void handleWork(const UserInfoPtr &userInfo, const QVariant &data);
// 
//     QList<UserInfoPtr> mFriendInfo;
// };

#endif // FRIENDJOB_H

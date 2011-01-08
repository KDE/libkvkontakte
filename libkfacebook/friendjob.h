/* Copyright 2010, 2011 Thomas McGuire <mcguire@kde.org>

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

#include "facebookjob.h"
#include "userinfo.h"

class LIBKFACEBOOK_EXPORT FriendJob : public FacebookJob
{
  Q_OBJECT
  public:
    FriendJob( const QStringList &friendIds, const QString &accessToken );
    FriendJob( const QString &friendId, const QString &accessToken );
    QList<UserInfoPtr> friendInfo() const;

  protected:
    virtual void handleData( const QVariant& data );

  private:
    QStringList friendFields() const;
    UserInfoPtr handleSingleUser( const QVariant& data );
    void handleLocation(const UserInfoPtr &userInfo, const QVariant &data);
    void handleWork(const UserInfoPtr &userInfo, const QVariant &data);

    QList<UserInfoPtr> mFriendInfo;
    bool mMultiQuery;
};

#endif

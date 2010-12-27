/* Copyright 2010 Thomas McGuire <mcguire@kde.org>

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
#include "friendjob.h"

#include <qjson/qobjecthelper.h>

FriendJob::FriendJob( const QString& friendId, const QString& accessToken )
  : FacebookJob( '/' + friendId, accessToken),
    mMultiQuery( false )
{
  QStringList fields;
  fields << "first_name"
         << "last_name"
         << "name"
         << "birthday"
         << "email"
         << "website"
         << "location"
         << "significant_other";
  setFields( fields );
}

FriendJob::FriendJob( const QStringList& friendIds, const QString& accessToken )
  : FacebookJob( accessToken ),
    mMultiQuery( true )
{
  QStringList fields;
  fields << "first_name"
         << "last_name"
         << "name"
         << "birthday"
         << "email"
         << "website"
         << "location"
         << "significant_other";
  setFields( fields );
  setIds( friendIds );
}

QList<UserInfoPtr> FriendJob::friendInfo() const
{
  return mFriendInfo;
}

UserInfoPtr FriendJob::handleSingleUser(const QVariant& data)
{
  UserInfoPtr friendInfo( new UserInfo() );
  QJson::QObjectHelper::qvariant2qobject( data.toMap(), friendInfo.data() );
  const QVariant location = data.toMap()["location"];
  if ( location.isValid() ) {
    const QVariant nameVariant = location.toMap()["name"];
    const QString name = nameVariant.toString();
    if ( !name.isEmpty() ) {
      if ( name.contains( ',' ) && name.count( ',' ) == 1 ) {
        QStringList parts = name.split( ',' );
        friendInfo->setCity( parts[0].simplified() );
        friendInfo->setCountry( parts[1].simplified() );
      } else {
        friendInfo->setCity( name );
      }
    }
  }
  return friendInfo;
}

void FriendJob::handleData( const QVariant& data )
{
  if ( !mMultiQuery ) {
    mFriendInfo.append( handleSingleUser( data ) );
  } else {
    foreach( const QVariant &user, data.toMap() ) {
      mFriendInfo.append( handleSingleUser( user ) );
    }
  }
}

#include "friendjob.moc"
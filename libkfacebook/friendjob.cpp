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
#include "friendjob.h"

#include <KDebug>
#include <qjson/qobjecthelper.h>

FriendJob::FriendJob( const QString& friendId, const QString& accessToken )
  : FacebookGetIdJob(friendId, accessToken)
{
  setFields( friendFields() );
}

FriendJob::FriendJob( const QStringList& friendIds, const QString& accessToken )
  : FacebookGetIdJob( friendIds, accessToken )
{
  setFields( friendFields() );
  setIds( friendIds );
}

QStringList FriendJob::friendFields() const
{
  QStringList fields;
  fields << "first_name"
         << "last_name"
         << "name"
         << "birthday"
         << "email"
         << "website"
         << "location"
         << "work"
         << "significant_other"
         << "timezone"
         << "updated_time";
  return fields;
}

QList<UserInfoPtr> FriendJob::friendInfo() const
{
  return mFriendInfo;
}

struct WorkInfo
{
  QDate start;
  QDate end;
  QString position;
  QString company;
};
typedef QSharedPointer<WorkInfo> WorkInfoPtr;

void FriendJob::handleLocation(const UserInfoPtr& userInfo, const QVariant& data)
{
  if ( data.isValid() ) {
    const QVariant nameVariant = data.toMap()["name"];
    const QString name = nameVariant.toString();
    if ( !name.isEmpty() ) {
      if ( name.contains( ',' ) && name.count( ',' ) == 1 ) {
        QStringList parts = name.split( ',' );
        userInfo->setCity( parts[0].simplified() );
        userInfo->setCountry( parts[1].simplified() );
      } else {
        userInfo->setCity( name );
      }
    }
  }
}

void FriendJob::handleWork(const UserInfoPtr& userInfo, const QVariant& data)
{
  QList<QVariant> work = data.toList();
  QList<WorkInfoPtr> workInfos;
  foreach(const QVariant &workInfo, work) {
    QVariantMap workMap = workInfo.toMap();
    QString startDate = workMap["start_date"].toString();
    QString endDate = workMap["end_date"].toString();
    QVariant employer = workMap["employer"];
    QVariant position = workMap["position"];

    startDate.replace("-00", "-01");
    endDate.replace("-00", "-01");
    WorkInfoPtr newWorkInfo( new WorkInfo );
    newWorkInfo->start = QDate::fromString(startDate + "-01", "yyyy-MM-dd");
    newWorkInfo->end = QDate::fromString(endDate + "-01", "yyyy-MM-dd");
    newWorkInfo->company = employer.toMap()["name"].toString();
    newWorkInfo->position = position.toMap()["name"].toString();

    // Invalid enddate means the user works here -> set that as the current work info
    if (!newWorkInfo->end.isValid()) {
      userInfo->setCompany(newWorkInfo->company);
      userInfo->setProfession(newWorkInfo->position);
      return;
    }
    workInfos.append(newWorkInfo);
  }

  // Ok, non of the jobs is marked as current, simply take the first one
  if (!workInfos.isEmpty()) {
    userInfo->setCompany(workInfos.first()->company);
    userInfo->setProfession(workInfos.first()->position);
  }
}

void FriendJob::handlePartner(const UserInfoPtr& userInfo, const QVariant& partner)
{
  if (partner.isValid()) {
    const QVariantMap partnerMap = partner.toMap();
    userInfo->setPartner(partnerMap["name"].toString());
  }
}

void FriendJob::handleSingleData(const QVariant& data)
{
  UserInfoPtr friendInfo( new UserInfo() );
  QJson::QObjectHelper::qvariant2qobject( data.toMap(), friendInfo.data() );
  const QVariant location = data.toMap()["location"];
  handleLocation(friendInfo, location);
  const QVariant work = data.toMap()["work"];
  handleWork(friendInfo, work);
  const QVariant partner = data.toMap()["significant_other"];
  handlePartner(friendInfo, partner);
  mFriendInfo.append(friendInfo);
}

#include "friendjob.moc"

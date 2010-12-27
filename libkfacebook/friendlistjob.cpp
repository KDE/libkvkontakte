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
#include "friendlistjob.h"

#include <qjson/parser.h>
#include <qjson/qobjecthelper.h>

#include <KIO/Job>
#include <KDebug>
#include <KLocale>

FriendListJob::FriendListJob( const QString& accessToken )
  : mAccessToken( accessToken )
{
}

void FriendListJob::start()
{
  KUrl url;
  url.setProtocol( "https" );
  url.setHost( "graph.facebook.com" );
  url.setPath( "/me/friends" );
  url.addQueryItem( "access_token", mAccessToken );
  url.addQueryItem( "fields", "name" );
  KIO::StoredTransferJob * const job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );
  connect( job, SIGNAL(result(KJob*)), this, SLOT(getJobFinished(KJob*)) );
  job->start();
}

QList< UserInfoPtr > FriendListJob::friends() const
{
  return mFriends;
}

void FriendListJob::getJobFinished( KJob* job )
{
  KIO::StoredTransferJob *transferJob = dynamic_cast<KIO::StoredTransferJob *>( job );
  Q_ASSERT( transferJob );
  if ( transferJob->error() ) {
    setError( transferJob->error() );
    setErrorText( transferJob->errorText() );
    kWarning() << "Job error: " << transferJob->errorText();
  } else {
    kDebug() << "Got data: " << QString::fromAscii( transferJob->data().data() );
    QJson::Parser parser;
    bool ok;
    const QVariant root = parser.parse( transferJob->data(), &ok );
    if ( ok ) {
      const QVariant data = root.toMap()["data"];
      foreach( const QVariant &user, data.toList() ) {
        UserInfoPtr userInfo( new UserInfo() );
        QJson::QObjectHelper::qvariant2qobject( user.toMap(), userInfo.data() );
        mFriends.append( userInfo );
      }
    } else {
      kWarning() << "Unable to parse JSON data: " << QString::fromAscii( transferJob->data().data() );
      setError( KJob::UserDefinedError );
      setErrorText( i18n( "Unable to parse data returned by the Facebook server." ) + "\n\n" + parser.errorString() );
    }
  }
  emitResult();
}

#include "friendlistjob.moc"



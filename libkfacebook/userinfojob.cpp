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
#include "userinfojob.h"

#include <qjson/parser.h>
#include <qjson/qobjecthelper.h>

#include <KIO/Job>
#include <KDebug>
#include <KLocale>

QString UserInfo::name() const
{
  return mName;
}

QString UserInfo::id() const
{
  return mId;
}

void UserInfo::setName(const QString& name)
{
  mName = name;
}

void UserInfo::setId(const QString& id)
{
  mId = id;
}

UserInfoJob::UserInfoJob( const QString& accessToken )
  : mAccessToken( accessToken )
{
}

void UserInfoJob::start()
{
  KUrl url;
  url.setProtocol( "https" );
  url.setHost( "graph.facebook.com" );
  url.setPath( "/me" );
  url.addQueryItem( "access_token", mAccessToken );
  url.addQueryItem( "fields", "name" );
  KIO::StoredTransferJob *job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );
  connect( job, SIGNAL(result(KJob*)), this, SLOT(getJobFinished(KJob*)) );
  job->start();
}

UserInfoPtr UserInfoJob::userInfo() const
{
  return mUserInfo;
}

void UserInfoJob::getJobFinished( KJob* job )
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
    QVariant userInfoVariant = parser.parse( transferJob->data(), &ok );
    if ( ok ) {
      mUserInfo = UserInfoPtr( new UserInfo() );
      QJson::QObjectHelper::qvariant2qobject( userInfoVariant.toMap(), mUserInfo.data() );
    } else {
      kWarning() << "Unable to parse JSON data: " << QString::fromAscii( transferJob->data().data() );
      setError( KJob::UserDefinedError );
      setErrorText( i18n( "Unable to parse data returned by the Facebook server." ) + "\n\n" + parser.errorString() );
    }
  }
  emitResult();
}

#include "userinfojob.moc"


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
#include "facebookjob.h"

#include <qjson/parser.h>

#include <KIO/Job>
#include <KDebug>
#include <KLocale>

FacebookJob::FacebookJob( const QString& path, const QString& accessToken )
  : mAccessToken( accessToken ),
    mPath( path )
{
  Q_ASSERT( mPath.startsWith( '/' ) );
  setCapabilities(KJob::Killable);
}

FacebookJob::FacebookJob( const QString& accessToken )
  : mAccessToken( accessToken )
{
}

void FacebookJob::addQueryItem( const QString& key, const QString& value )
{
  QueryItem item;
  item.first = key;
  item.second = value;
  mQueryItems.append( item );
}

bool FacebookJob::doKill()
{
  if (mJob) {
    mJob->kill(KJob::Quietly);
  }
  return KJob::doKill();
}

void FacebookJob::setIds( const QStringList& ids )
{
  mIds = ids;
}

void FacebookJob::setFields( const QStringList& fields )
{
  mFields = fields;
}

void FacebookJob::start()
{
  Q_ASSERT( mIds.isEmpty() ^ mPath.isEmpty() );
  KUrl url;
  url.setProtocol( "https" );
  url.setHost( "graph.facebook.com" );
  if ( !mPath.isEmpty() ) {
    url.setPath( mPath );
  } else {
    url.setPath( "/" );
    url.addQueryItem( "ids", mIds.join( "," ) );
  }
  url.addQueryItem( "access_token", mAccessToken );
  if ( !mFields.isEmpty() ) {
    url.addQueryItem( "fields", mFields.join( "," ) );
  }
  if ( !mQueryItems.isEmpty() ) {
    foreach( const QueryItem &item, mQueryItems ) {
      url.addQueryItem( item.first, item.second );
    }
  }
  kDebug() << "Starting query" << url;
  KIO::StoredTransferJob * const job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );
  mJob = job;
  connect( job, SIGNAL(result(KJob*)), this, SLOT(getJobFinished(KJob*)) );
  job->start();
}

void FacebookJob::getJobFinished( KJob* job )
{
  KIO::StoredTransferJob *transferJob = dynamic_cast<KIO::StoredTransferJob *>( job );
  Q_ASSERT( transferJob );
  if ( transferJob->error() ) {
    setError( transferJob->error() );
    setErrorText( KIO::buildErrorString( error(), transferJob->errorText() ) );
    kWarning() << "Job error: " << transferJob->errorString();
  } else {
    kDebug() << "Got data: " << QString::fromAscii( transferJob->data().data() );
    QJson::Parser parser;
    bool ok;
    const QVariant data = parser.parse( transferJob->data(), &ok );
    if ( ok ) {
      const QVariant error = data.toMap()["error"];
      if ( error.isValid() ) {
        handleError( error );
      } else {
        handleData( data );
      }
    } else {
      kWarning() << "Unable to parse JSON data: " << QString::fromAscii( transferJob->data().data() );
      setError( KJob::UserDefinedError );
      setErrorText( i18n( "Unable to parse data returned by the Facebook server: %1", parser.errorString() ) );
    }
  }
  emitResult();
  mJob = 0;
}

void FacebookJob::handleError( const QVariant& data )
{
  const QVariantMap errorMap = data.toMap();
  const QString type = errorMap["type"].toString();
  const QString message = errorMap["message"].toString();
  setError( KJob::UserDefinedError );
  setErrorText( i18n( "The Facebook server returned an error of type <i>%1</i>: %2" , type, message ) );
}


#include "facebookjob.moc"

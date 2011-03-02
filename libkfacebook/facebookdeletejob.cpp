/* Copyright 2011 Roeland Jago Douma <unix@rullzer.com>

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
#include "facebookdeletejob.h"

#include <qjson/parser.h>

#include <KIO/Job>
#include <KDebug>
#include <KLocale>

#include <QNetworkReply>
#include <QNetworkAccessManager>

FacebookDeleteJob::FacebookDeleteJob( const QString& id, const QString& accessToken )
  : mAccessToken( accessToken ),
    mId( id )
{
  setCapabilities(KJob::Killable);
}

bool FacebookDeleteJob::doKill()
{
  if (mJob) {
    mJob->kill(KJob::Quietly);
  }
  return KJob::doKill();
}

void FacebookDeleteJob::start()
{
  KUrl url;
  url.setProtocol("https");
  url.setHost("graph.facebook.com");
  url.setPath("/" + mId);

  url.addQueryItem("access_token", mAccessToken);
  url.addQueryItem("method", "delete");

  kDebug() << "Starting delete: " << url;
  KIO::StoredTransferJob * const job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );
  mJob = job;
  connect( job, SIGNAL(result(KJob*)), this, SLOT(deleteJobFinished(KJob*)) );
  job->start();
}

void FacebookDeleteJob::deleteJobFinished( KJob *job )
{
  KIO::StoredTransferJob *transferJob = dynamic_cast<KIO::StoredTransferJob *>( job );
  Q_ASSERT( transferJob );
  if ( transferJob->error() ) { 
    setError( transferJob->error() );
    setErrorText( KIO::buildErrorString( error(), transferJob->errorText() ) );
    kWarning() << "Job error: " << transferJob->errorString();
  } else {
    kDebug() << "Got data: " << QString::fromAscii( transferJob->data().data() );
  }

  emitResult();
  mJob = 0;
}

void FacebookDeleteJob::handleError( const QVariant& data )
{
  const QVariantMap errorMap = data.toMap();
  const QString type = errorMap["type"].toString();
  const QString message = errorMap["message"].toString();
  kWarning() << "An error of type" << type << "occurred:" << message;
  if ( type.toLower() != "oauthexception" ) {
    setError( KJob::UserDefinedError );
    setErrorText( i18n( "The Facebook server returned an error of type <i>%1</i>: <i>%2</i>" , type, message ) );
  } else {
    setError( AuthenticationProblem );
    setErrorText( i18n( "Unable to login to the Facebook server, authentication failure.\nThe server said: <i>%1</i>", message ) );
  }
}


#include "facebookdeletejob.moc"

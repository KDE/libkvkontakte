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
#include "facebookaddjob.h"

#include <qjson/parser.h>

#include <KIO/Job>
#include <KDebug>
#include <KLocale>

FacebookAddJob::FacebookAddJob( const QString& path, const QString& accessToken )
  : mAccessToken( accessToken ),
    mPath( path )
{
  setCapabilities(KJob::Killable);
}

bool FacebookAddJob::doKill()
{
  if (mJob) {
    mJob->kill(KJob::Quietly);
  }
  return KJob::doKill();
}

void FacebookAddJob::addField( const QString& key, const QString& value)
{
  mFields.insert(key, value);
}

void FacebookAddJob::start()
{
  KUrl url;
  url.setProtocol("https");
  url.setHost("graph.facebook.com");
  url.setPath(mPath);

  url.addQueryItem("access_token", mAccessToken);

  QMap<QString, QString>::const_iterator i = mFields.constBegin();
  while (i != mFields.constEnd()) {
    url.addQueryItem(i.key(), i.value());
    ++i;
  }

  kDebug() << "Starting add: " << url;
  KIO::StoredTransferJob * const job = KIO::storedHttpPost( QByteArray(), url, KIO::HideProgressInfo );
  mJob = job;
  connect( job, SIGNAL(result(KJob*)), this, SLOT(addJobFinished(KJob*)) );
  job->start();
}

void FacebookAddJob::addJobFinished( KJob *job )
{
  KIO::StoredTransferJob *addJob = dynamic_cast<KIO::StoredTransferJob *>( job );
  Q_ASSERT( addJob );
  if ( addJob->error() ) { 
    setError( addJob->error() );
    setErrorText( KIO::buildErrorString( error(), addJob->errorText() ) );
    kWarning() << "Job error: " << addJob->errorString();
  } else {
    QJson::Parser parser;
    QVariant result = parser.parse(addJob->data().data());
    const QVariantMap dataMap = result.toMap();
    if ( dataMap.contains("id") ) {
      setProperty("id", dataMap["id"]);
    }
  }

  emitResult();
  mJob = 0;
}

void FacebookAddJob::handleError( const QVariant& data )
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


#include "facebookaddjob.moc"

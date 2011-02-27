/* Copyright 2011 Thomas McGuire <mcguire@kde.org>

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
#include "photojob.h"

#include <KDebug>
#include <KIO/Job>

PhotoJob::PhotoJob(const QString& friendId, const QString& accessToken)
  : mAccessToken(accessToken), mUserId(friendId)
{
  setCapabilities(KJob::Killable);
}

QImage PhotoJob::photo() const
{
  return mImage;
}

bool PhotoJob::doKill()
{
  if (mJob) {
    mJob->kill(KJob::Quietly);
  }
  return KJob::doKill();
}

void PhotoJob::start()
{
  KUrl url;
  url.setProtocol( "https" );
  url.setHost( "graph.facebook.com" );
  url.setPath( "/" + mUserId + "/picture" );
  url.addQueryItem( "access_token", mAccessToken );
  url.addQueryItem( "type", "large" );
  kDebug() << "Starting query" << url;
  KIO::StoredTransferJob * const job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );
  mJob = job;
  connect( job, SIGNAL(result(KJob*)), this, SLOT(getJobFinished(KJob*)) );
  job->start();
}

void PhotoJob::getJobFinished(KJob* job)
{
  KIO::StoredTransferJob * const transferJob = dynamic_cast<KIO::StoredTransferJob *>( job );
  Q_ASSERT( transferJob );
  if ( transferJob->error() ) {
    setError( transferJob->error() );
    setErrorText( KIO::buildErrorString( error(), transferJob->errorText() ) );
    kWarning() << "Job error: " << transferJob->errorText();
  } else {
    kDebug() << "Got picture of" << transferJob->data().length() << "bytes.";
    mImage = QImage::fromData(transferJob->data());
  }
  emitResult();
  mJob = 0;
}

#include "photojob.moc"

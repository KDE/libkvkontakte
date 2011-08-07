/* Copyright 2010, 2011 Thomas McGuire <mcguire@kde.org>
   Copyright 2011 Roeland Jago Douma <unix@rullzer.com>
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
#include "vkontaktejobs.h"

#include <qjson/parser.h>

#include <KIO/Job>
#include <KDebug>
#include <KLocale>

namespace Vkontakte
{

bool KJobWithSubjob::doKill()
{
    if (m_job) {
        m_job->kill(KJob::Quietly);
    }
    return KJob::doKill();
}

//---------------------------------------------------------------

bool KJobWithSubjobs::doKill()
{
    foreach (KJob *job, m_jobs) {
        job->kill(KJob::Quietly);
    }
    m_jobs.clear();

    return KJob::doKill();
}

//---------------------------------------------------------------

/*
 * VkontakteJobs base class
 */
VkontakteJob::VkontakteJob( const QString& method, const QString& accessToken, bool httpPost )
    : m_accessToken( accessToken ),
      m_method( method ),
      m_httpPost( httpPost )
{
    setCapabilities(KJob::Killable);
}

void VkontakteJob::addQueryItem( const QString& key, const QString& value )
{
    QueryItem item;
    item.first = key;
    item.second = value;
    m_queryItems.append( item );
}

void VkontakteJob::handleError( const QVariant& data )
{
    const QVariantMap errorMap = data.toMap();
    int error_code = errorMap["error_code"].toInt();
    const QString error_msg = errorMap["error_msg"].toString();
    kWarning() << "An error of type" << error_code << "occurred:" << error_msg;
//     if ( type.toLower() != "oauthexception" ) {
        setError(KJob::UserDefinedError);
        setErrorText(i18n(
            "The VKontakte server returned an error "
            "of type <i>%1</i> in reply to method %2: <i>%3</i>",
            error_code, m_method, error_msg));
//     } else {
//         setError( AuthenticationProblem );
//         setErrorText( i18n( "Unable to login to the VKontakte server, authentication failure.\nThe server said: <i>%1</i>", message ) );
//     }
}

void VkontakteJob::start()
{
    KUrl url;
    url.setProtocol( "https" );
    url.setHost( "api.vkontakte.ru" );
    url.setPath( "/method/" + m_method );

    prepareQueryItems();
    foreach( const QueryItem &item, m_queryItems ) {
        url.addQueryItem( item.first, item.second );
    }
    url.addQueryItem( "access_token", m_accessToken );

    kDebug() << "Starting request" << url;
    KIO::StoredTransferJob *job;
    if (m_httpPost)
        job = KIO::storedHttpPost( QByteArray(), url, KIO::HideProgressInfo );
    else
        job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );

    m_job = job;
    connect( job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)) );
    job->start();
}

void VkontakteJob::jobFinished(KJob *job)
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
                handleData( data.toMap()["response"] );
            }
        } else {
            kWarning() << "Unable to parse JSON data: " << QString::fromAscii( transferJob->data().data() );
            setError( KJob::UserDefinedError );
            setErrorText( i18n( "Unable to parse data returned by the VKontakte server: %1", parser.errorString() ) );
        }
    }
    emitResult();
    m_job = 0;
}

/*
VkontakteAddJob::VkontakteAddJob( const QString &path, const QString &accessToken)
  : VkontakteJob(path, accessToken)
{
}

void VkontakteAddJob::start()
{
  KUrl url;
  url.setProtocol("https");
  url.setHost("graph.facebook.com");
  url.setPath(mPath);

  url.addQueryItem("access_token", mAccessToken);

  foreach(const QueryItem &item, mQueryItems) {
    url.addQueryItem(item.first, item.second);
  }

  kDebug() << "Starting add: " << url;
  KIO::StoredTransferJob * const job = KIO::storedHttpPost( QByteArray(), url, KIO::HideProgressInfo );
  mJob = job;
  connect( job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)) );
  job->start();
}

void VkontakteAddJob::jobFinished(KJob *job)
{
  KIO::StoredTransferJob *addJob = dynamic_cast<KIO::StoredTransferJob *>( job );
  Q_ASSERT( addJob );
  if ( addJob->error() ) {
    setError( addJob->error() );
    setErrorText( KIO::buildErrorString( error(), addJob->errorText() ) );
    kWarning() << "Job error: " << addJob->errorString();
  } else {
    QJson::Parser parser;
    bool ok;
    const QVariant result = parser.parse(addJob->data(), &ok);
    if (!ok) {
      kWarning() << "Unable to parse JSON data: " << QString::fromAscii( addJob->data().data() );
      setError( KJob::UserDefinedError );
      setErrorText( i18n( "Unable to parse data returned by the Facebook server: %1", parser.errorString() ) );
    } else {
      const QVariant error = result.toMap()["error"];
      if ( error.isValid() ) {
        handleError( error );
      } else {
        const QVariantMap dataMap = result.toMap();
        if ( dataMap.contains("id") ) {
          setProperty("id", dataMap["id"]);
        }
      }
    }
  }

  emitResult();
  mJob = 0;
}

VkontakteDeleteJob::VkontakteDeleteJob( const QString &id, const QString &accessToken)
  : VkontakteJob("/" + id, accessToken)
{
}

void VkontakteDeleteJob::start()
{
  KUrl url;
  url.setProtocol("https");
  url.setHost("graph.facebook.com");
  url.setPath(mPath);

  url.addQueryItem("access_token", mAccessToken);
  url.addQueryItem("method", "delete");

  kDebug() << "Starting delete: " << url;
  KIO::StoredTransferJob * const job = KIO::storedHttpPost( QByteArray(), url, KIO::HideProgressInfo );
  mJob = job;
  connect( job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)) );
  job->start();
}

void VkontakteDeleteJob::jobFinished( KJob *job )
{
  KIO::StoredTransferJob *deleteJob = dynamic_cast<KIO::StoredTransferJob *>( job );
  Q_ASSERT( deleteJob );
  if ( deleteJob->error() ) {
    setError( deleteJob->error() );
    setErrorText( KIO::buildErrorString( error(), deleteJob->errorText() ) );
    kWarning() << "Job error: " << deleteJob->errorString();
  } else {
    // TODO: error handling. Does the server return the error as a JSON string?
    kDebug() << "Got data: " << QString::fromAscii( deleteJob->data().data() );
  }

  emitResult();
  mJob = 0;
}

VkontakteGetJob::VkontakteGetJob( const QString &path, const QString &accessToken )
  : VkontakteJob(path, accessToken)
{
}

VkontakteGetJob::VkontakteGetJob( const QString &accessToken)
  : VkontakteJob(accessToken)
{
}

void VkontakteGetJob::setIds( const QStringList& ids )
{
  mIds = ids;
}

void VkontakteGetJob::setFields( const QStringList& fields )
{
  mFields = fields;
}

void VkontakteGetJob::start()
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

  foreach( const QueryItem &item, mQueryItems ) {
    url.addQueryItem( item.first, item.second );
  }

  kDebug() << "Starting query" << url;
  KIO::StoredTransferJob * const job = KIO::storedGet( url, KIO::Reload, KIO::HideProgressInfo );
  mJob = job;
  connect( job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)) );
  job->start();
}

void VkontakteGetJob::jobFinished(KJob *job)
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

VkontakteGetIdJob::VkontakteGetIdJob(const QStringList &ids, const QString &accessToken)
  : VkontakteGetJob(accessToken),
    mMultiQuery(true)
{
  setIds(ids);
}

VkontakteGetIdJob::VkontakteGetIdJob(const QString &id, const QString &accessToken)
  : VkontakteGetJob("/" + id, accessToken),
    mMultiQuery(false)
{
}

void VkontakteGetIdJob::handleData(const QVariant &data)
{
  if (!mMultiQuery) {
    handleSingleData(data);
  } else {
    foreach(const QVariant &item, data.toMap()) {
      handleSingleData(item);
    }
  }
}*/

} /* namespace Vkontakte */

#include "vkontaktejobs.moc"

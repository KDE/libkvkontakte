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
#include "alleventslistjob.h"

#include "eventslistjob.h"

#include <KDebug>
#include <KUrl>

AllEventsListJob::AllEventsListJob( const QString& accessToken )
  : mAccessToken( accessToken )
{
}

QList< EventInfoPtr > AllEventsListJob::allEvents() const
{
  return mEvents;
}

bool AllEventsListJob::doKill()
{
  if (mCurrentJob) {
    mCurrentJob->kill( KJob::Quietly );
  }
  return KJob::doKill();
}

void AllEventsListJob::setLowerLimit( const KDateTime& lowerLimit )
{
  mLowerLimit = lowerLimit;
}

void AllEventsListJob::start()
{
  Q_ASSERT( mLowerLimit.isValid() );
  Q_ASSERT( !mCurrentJob );
  mCurrentJob = new EventsListJob( mAccessToken );
  connect( mCurrentJob, SIGNAL(result(KJob*)),
           this, SLOT(eventListJobFinished(KJob*)) );
  mCurrentJob->start();
}

void AllEventsListJob::eventListJobFinished( KJob* job )
{
  Q_ASSERT( job == mCurrentJob );
  EventsListJob * const listJob = dynamic_cast<EventsListJob*>( job );
  Q_ASSERT( listJob );
  if ( job->error() ) {
    setError( listJob->error() );
    setErrorText( listJob->errorString() );
    emitResult();
  } else {
    kDebug() << "Got" << listJob->events().size() << "events from our subjob.";
    //kDebug() << "Next: " << listJob->nextEvents();
    //kDebug() << "Prev: " << listJob->previousEvents();

    const KUrl next = KUrl::fromUserInput( listJob->nextEvents() );
    const QString limit = next.queryItem( "limit" );
    const QString until = next.queryItem( "until" );
    const QString since = next.queryItem( "since" );
    if ( until.isEmpty() ) {
      kDebug() << "Aborting events fetching, no date range found in URL!";
    }
    KDateTime untilTime;
    untilTime.setTime_t( until.toLongLong() );
    if ( !untilTime.isValid() ) {
      kDebug() << "Aborting events fetching, invalid date range found in URL!";
    }
    //kDebug() << "Starting new subjob for limit" << limit << ", until" << until << ", since" << since;
    //kDebug() << "Lower limit:" << mLowerLimit.toString();
    //kDebug() << "limit of URL:" << untilTime.toString();

    // Stop when we got all events after a certain dates, or no event at all
    if ( listJob->events().isEmpty() || until.isEmpty() || !untilTime.isValid() ||
         untilTime < mLowerLimit ) {
      kDebug() << "All events fetched.";
      mCurrentJob = 0;
      emitResult();
    } else {
      mEvents.append( listJob->events() );

      mCurrentJob = new EventsListJob( mAccessToken );
      if ( !limit.isEmpty() ) {
        mCurrentJob->addQueryItem( "limit", limit );
      }
      if ( !until.isEmpty() ) {
        mCurrentJob->addQueryItem( "until", until );
      }
      if ( !since.isEmpty() ) {
        mCurrentJob->addQueryItem( "since", since );
      }
      connect( mCurrentJob, SIGNAL(result(KJob*)),
               this, SLOT(eventListJobFinished(KJob*)) );
      mCurrentJob->start();
    }
  }
}

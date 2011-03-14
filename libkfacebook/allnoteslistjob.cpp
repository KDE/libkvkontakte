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
#include "allnoteslistjob.h"

#include "noteslistjob.h"

#include <KDebug>
#include <KUrl>

AllNotesListJob::AllNotesListJob( const QString& accessToken )
  : mAccessToken( accessToken )
{
}

QList< NoteInfoPtr > AllNotesListJob::allNotes() const
{
  return mNotes;
}

bool AllNotesListJob::doKill()
{
  if (mCurrentJob) {
    mCurrentJob->kill( KJob::Quietly );
  }
  return KJob::doKill();
}

void AllNotesListJob::setLowerLimit( const KDateTime& lowerLimit )
{
  mLowerLimit = lowerLimit;
}

void AllNotesListJob::start()
{
  Q_ASSERT( mLowerLimit.isValid() );
  Q_ASSERT( !mCurrentJob );
  mCurrentJob = new NotesListJob( mAccessToken );
  connect( mCurrentJob, SIGNAL(result(KJob*)),
           this, SLOT(noteListJobFinished(KJob*)) );
  mCurrentJob->start();
}

void AllNotesListJob::noteListJobFinished( KJob* job )
{
  Q_ASSERT( job == mCurrentJob );
  NotesListJob * const listJob = dynamic_cast<NotesListJob*>( job );
  Q_ASSERT( listJob );
  if ( job->error() ) {
    mCurrentJob = 0;
    setError( listJob->error() );
    setErrorText( listJob->errorString() );
    emitResult();
  } else {
    kDebug() << "Got" << listJob->notes().size() << "notes from our subjob.";
    //kDebug() << "Next: " << listJob->nextNotes();
    //kDebug() << "Prev: " << listJob->previousNotes();

    const KUrl prev = KUrl::fromUserInput( listJob->previousNotes() );
    const QString limit = prev.queryItem( "limit" );
    const QString until = prev.queryItem( "until" );
    const QString since = prev.queryItem( "since" );
    if ( until.isEmpty() ) {
      kDebug() << "Aborting notes fetching, no date range found in URL!";
    }
    KDateTime sinceTime;
    sinceTime.setTime_t( since.toLongLong() );
    if ( !sinceTime.isValid() ) {
      kDebug() << "Aborting notes fetching, invalid date range found in URL!";
    }
    //kDebug() << "Starting new subjob for limit" << limit << ", until" << until << ", since" << since;
    //kDebug() << "Lower limit:" << mLowerLimit.toString();
    //kDebug() << "limit of URL:" << untilTime.toString();

    // Stop when we got all notes after a certain dates, or no notes at all
    if ( listJob->notes().isEmpty() || since.isEmpty() || !sinceTime.isValid() ||
         sinceTime < mLowerLimit ) {
      kDebug() << "All notes fetched: " << mNotes.size();
      mCurrentJob = 0;
      emitResult();
    } else {
      mNotes.append( listJob->notes() );

      mCurrentJob = new NotesListJob( mAccessToken );
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
               this, SLOT(noteListJobFinished(KJob*)) );
      mCurrentJob->start();
    }
  }
}

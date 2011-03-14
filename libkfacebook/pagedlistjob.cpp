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
#include "pagedlistjob.h"

#include "listjobbase.h"

#include <KDebug>

PagedListJob::PagedListJob( const QString& accessToken )
  : mAccessToken( accessToken )
{
}

bool PagedListJob::doKill()
{
  if (mCurrentJob) {
    mCurrentJob->kill( KJob::Quietly );
  }
  return KJob::doKill();
}

void PagedListJob::setLowerLimit( const KDateTime& lowerLimit )
{
  mLowerLimit = lowerLimit;
}

void PagedListJob::start()
{
  Q_ASSERT( mLowerLimit.isValid() );
  Q_ASSERT( !mCurrentJob );
  mCurrentJob = createJob(KUrl(), KUrl());
  connect( mCurrentJob, SIGNAL(result(KJob*)),
           this, SLOT(listJobFinished(KJob*)) );
  mCurrentJob->start();
}

void PagedListJob::listJobFinished( KJob* job )
{
  Q_ASSERT( job == mCurrentJob );
  ListJobBase * const listJob = dynamic_cast<ListJobBase*>( job );
  Q_ASSERT( listJob );
  if ( job->error() ) {
    mCurrentJob = 0;
    setError( listJob->error() );
    setErrorText( listJob->errorString() );
    emitResult();
  } else {
    kDebug() << "Got" << listJob->numEntries() << "items from our subjob.";

    const KUrl next = KUrl::fromUserInput( listJob->nextItems() );
    const KUrl prev = KUrl::fromUserInput( listJob->previousItems() );

    // Stop when we got all items after a certain dates, or no items at all
    if ( listJob->numEntries() == 0 || !shouldStartNewJob(prev, next)) {
      kDebug() << "All items fetched.";
      mCurrentJob = 0;
      emitResult();
    } else {
      appendItems(mCurrentJob);

      mCurrentJob = createJob(prev, next);
      connect( mCurrentJob, SIGNAL(result(KJob*)),
               this, SLOT(listJobFinished(KJob*)) );
      mCurrentJob->start();
    }
  }
}
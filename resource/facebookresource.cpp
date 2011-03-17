/* Copyright 2010, 2011 Thomas McGuire <mcguire@kde.org>
   Copyright 2011 Roeland Jago Douma <unix@rullzer.com>

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
#include "facebookresource.h"
#include <config.h>
#include "settings.h"
#include "settingsdialog.h"
#include "timestampattribute.h"

#include <libkfacebook/friendlistjob.h>
#include <libkfacebook/friendjob.h>
#include <libkfacebook/photojob.h>
#include <libkfacebook/alleventslistjob.h>
#include <libkfacebook/eventjob.h>
#include <libkfacebook/allnoteslistjob.h>
#include <libkfacebook/notejob.h>
#include <libkfacebook/noteaddjob.h>
#include <libkfacebook/facebookjobs.h>

#include <Akonadi/AttributeFactory>
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <akonadi/changerecorder.h>

using namespace Akonadi;

static const char * friendsRID = "friends";
static const char * eventsRID = "events";
static const char * eventMimeType = "application/x-vnd.akonadi.calendar.event";
static const char * notesRID = "notes";

FacebookResource::FacebookResource( const QString &id )
    : ResourceBase( id )
{
  AttributeFactory::registerAttribute<TimeStampAttribute>();
  setNeedsNetwork( true );
#if KDEPIMLIBS_IS_VERSION( 4, 6, 41 )
  setAutomaticProgressReporting( false );
#endif
  setObjectName( QLatin1String( "FacebookResource" ) );
  resetState();
  Settings::self()->setResourceId( identifier() );

  connect( this, SIGNAL(abortRequested()),
           this, SLOT(slotAbortRequested()) );
  connect( this, SIGNAL(reloadConfiguration()), SLOT(configurationChanged()) );

  changeRecorder()->fetchCollection( true );
  changeRecorder()->itemFetchScope().fetchFullPayload( true );
}

FacebookResource::~FacebookResource()
{
  Settings::self()->writeConfig();
}

void FacebookResource::configurationChanged()
{
  Settings::self()->writeConfig();
}

void FacebookResource::aboutToQuit()
{
  slotAbortRequested();
}

void FacebookResource::abort()
{
  resetState();
  cancelTask();
}

void FacebookResource::abortWithError( const QString& errorMessage, bool authFailure )
{
  resetState();
  cancelTask( errorMessage );

  // This doesn't work, why?
  if ( authFailure ) {
    emit status( Broken, i18n( "Unable to login to Facebook, authentication failure." ) );
  }
}

void FacebookResource::resetState()
{
  mIdle = true;
  mNumFriends = -1;
  mNumPhotosFetched = 0;
  mCurrentJobs.clear();
  mExistingFriends.clear();
  mNewOrChangedFriends.clear();
  mPendingFriends.clear();
}

void FacebookResource::slotAbortRequested()
{
  if (!mIdle) {
    foreach(const QPointer<KJob> &job, mCurrentJobs) {
      kDebug() << "Killing current job:" << job;
      job->kill(KJob::Quietly);
    }
    abort();
  }
}

void FacebookResource::configure( WId windowId )
{
  const QPointer<SettingsDialog> settingsDialog( new SettingsDialog( this, windowId ) );
  if ( settingsDialog->exec() == QDialog::Accepted ) {
    emit configurationDialogAccepted();
  }
  else {
    emit configurationDialogRejected();
  }

  delete settingsDialog;
}

void FacebookResource::retrieveItems( const Akonadi::Collection &collection )
{
  Q_ASSERT(mIdle);

  if ( collection.remoteId() == friendsRID ) {
    mIdle = false;
    emit status( Running, i18n( "Preparing sync of friends list." ) );
    emit percent( 0 );
    ItemFetchJob * const fetchJob = new ItemFetchJob( collection );
    fetchJob->fetchScope().fetchAttribute<TimeStampAttribute>();
    fetchJob->fetchScope().fetchFullPayload( false );
    mCurrentJobs << fetchJob;
    connect( fetchJob, SIGNAL(result(KJob*)), this, SLOT(initialItemFetchFinished(KJob*)) );
  } else if ( collection.remoteId() == eventsRID ) {
    mIdle = false;
    emit status( Running, i18n( "Preparing sync of events list." ) );
    emit percent( 0 );
    AllEventsListJob * const listJob = new AllEventsListJob( Settings::self()->accessToken() );
    listJob->setLowerLimit(KDateTime::fromString( Settings::self()->lowerLimit(), "%Y-%m-%d" ));
    mCurrentJobs << listJob;
    connect( listJob, SIGNAL(result(KJob*)), this, SLOT(eventListFetched(KJob*)) );
    listJob->start();
  } else if ( collection.remoteId() == notesRID ) {
    mIdle = false;
    emit status( Running, i18n( "Preparing sync of notes list." ) );
    emit percent( 0 );
    AllNotesListJob * const notesJob = new AllNotesListJob( Settings::self()->accessToken() );
    notesJob->setLowerLimit(KDateTime::fromString( Settings::self()->lowerLimit(), "%Y-%m-%d" ));
    mCurrentJobs << notesJob;
    connect( notesJob, SIGNAL(result(KJob*)), this, SLOT(noteListFetched(KJob*)) );
    notesJob->start();
  } else {
    // This can not happen
    Q_ASSERT(!"Unknown Collection");
    cancelTask();
  }
}

bool FacebookResource::retrieveItem( const Akonadi::Item &item, const QSet<QByteArray> &parts )
{
  Q_UNUSED( parts );

  kDebug() << item.mimeType();

  if (item.mimeType() == "text/directory") {
    // TODO: Is this ever called??
    mIdle = false;
    FriendJob * const friendJob = new FriendJob( item.remoteId(),
                                               Settings::self()->accessToken() );
    mCurrentJobs << friendJob;
    friendJob->setProperty( "Item", QVariant::fromValue( item ) );
    connect( friendJob, SIGNAL(result(KJob*)), this, SLOT(friendJobFinished(KJob*)) );
    friendJob->start();
  } else if (item.mimeType() == "text/x-vnd.akonadi.note") {
    mIdle = false;
    NoteJob * const noteJob = new NoteJob( item.remoteId(), Settings::self()->accessToken());
    mCurrentJobs << noteJob;
    noteJob->setProperty( "Item", QVariant::fromValue( item ) );
    connect( noteJob, SIGNAL(result(KJob*)), this, SLOT(noteJobFinished(KJob*)) );
    noteJob->start();
  }
  return true;
}

void FacebookResource::retrieveCollections()
{
  Collection friends;
  friends.setRemoteId( friendsRID );
  friends.setName( i18n( "Friends" ) );
  friends.setParentCollection( Akonadi::Collection::root() );
  friends.setContentMimeTypes( QStringList() << "text/directory" );
  friends.setRights( Collection::ReadOnly );
  EntityDisplayAttribute * const friendsDisplayAttribute = new EntityDisplayAttribute();
  friendsDisplayAttribute->setIconName( "facebookresource" );
  friends.addAttribute( friendsDisplayAttribute );

  Collection events;
  events.setRemoteId( eventsRID );
  events.setName( i18n( "Events" ) );
  events.setParentCollection( Akonadi::Collection::root() );
  events.setContentMimeTypes( QStringList() << "text/calendar" << eventMimeType );
  events.setRights( Collection::ReadOnly );
  EntityDisplayAttribute * const evendDisplayAttribute = new EntityDisplayAttribute();
  evendDisplayAttribute->setIconName( "facebookresource" );
  events.addAttribute( evendDisplayAttribute );

  Collection notes;
  notes.setRemoteId( notesRID );
  notes.setName( i18n( "Notes" ) );
  notes.setParentCollection( Akonadi::Collection::root() );
  notes.setContentMimeTypes( QStringList() << "text/x-vnd.akonadi.note"  );
  notes.setRights(Collection::ReadOnly);
  EntityDisplayAttribute * const notesDisplayAttribute = new EntityDisplayAttribute();
  notesDisplayAttribute->setIconName( "facebookresource" );
  notes.addAttribute( notesDisplayAttribute );

  collectionsRetrieved( Collection::List() << friends << events << notes );
}

void FacebookResource::itemRemoved(const Akonadi::Item &item)
{
  if (item.mimeType() == "text/x-vnd.akonadi.note") {
    mIdle = false;
    FacebookDeleteJob * const deleteJob = new FacebookDeleteJob( item.remoteId(),
                                               Settings::self()->accessToken() );
    mCurrentJobs << deleteJob;
    deleteJob->setProperty( "Item", QVariant::fromValue( item ) );
    connect( deleteJob, SIGNAL(result(KJob*)), this, SLOT(deleteJobFinished(KJob*)) );
    deleteJob->start();
  } else {
    // Shouldn't happen, all other items are read-only
    Q_ASSERT(!"Unable to delete item, not ours.");
    cancelTask();
  }
}

void FacebookResource::deleteJobFinished(KJob *job)
{
  Q_ASSERT(!mIdle);
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  mCurrentJobs.removeAll(job);
  if ( job->error() ) {
    abortWithError( i18n( "Unable to delete note from server: %1", job->errorText() ) );
  } else {
    const Item item = job->property( "Item" ).value<Item>(); 
    changeCommitted( item );
    resetState();
  }
}

void FacebookResource::itemAdded( const Akonadi::Item &item, const Akonadi::Collection &collection )
{
  if (collection.remoteId() == notesRID) {
    if (item.hasPayload<KMime::Message::Ptr>()) {
      const KMime::Message::Ptr note = item.payload<KMime::Message::Ptr>();
      const QString subject = note->subject()->asUnicodeString();
      const QString message = note->body();

      mIdle = false;
      NoteAddJob * const addJob = new NoteAddJob( subject, message, Settings::self()->accessToken() );
      mCurrentJobs << addJob;
      addJob->setProperty( "Item", QVariant::fromValue( item ) );
      connect( addJob, SIGNAL(result(KJob *)), this, SLOT(noteAddJobFinished(KJob *)) );
      addJob->start();
    } else {
      Q_ASSERT(!"Note has wrong mimetype.");
      cancelTask();
    }
  } else {
    Q_ASSERT(!"Can not add this type of item!");
    cancelTask();
  }
}

AKONADI_RESOURCE_MAIN( FacebookResource )

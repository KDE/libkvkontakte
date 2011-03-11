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
  mCurrentJobs.clear();
  mExistingFriends.clear();
  mNewOrChangedFriends.clear();
}

void FacebookResource::slotAbortRequested()
{
  if (!mIdle) {
    foreach(const QPointer<KJob> j, mCurrentJobs) {
      kDebug() << "Killing current job:" << j;
      j->kill(KJob::Quietly);
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
    cancelTask( i18n( "Unable to syncronize this collection." ) );
  }
}

void FacebookResource::noteListFetched( KJob* job )
{
  Q_ASSERT( !mIdle );
  AllNotesListJob * const listJob = dynamic_cast<AllNotesListJob*>( job );
  Q_ASSERT( listJob );
  mCurrentJobs.removeAll(job);

  if ( listJob->error() ) {
    abortWithError( i18n( "Unable to get notes from server: %1", listJob->errorString() ),
                    listJob->error() == FacebookJob::AuthenticationProblem );
  } else {
    setItemStreamingEnabled( true );
    
    Item::List noteItems;
    foreach( const NoteInfoPtr &noteInfo, listJob->allNotes() ) {
      Item note;
      note.setRemoteId( noteInfo->id() );
      note.setPayload<KMime::Message::Ptr>( noteInfo->asNote() );
      note.setSize( noteInfo->asNote()->size() );
      note.setMimeType( "text/x-vnd.akonadi.note" );
      noteItems.append(note);
    }

    itemsRetrieved( noteItems );
    itemsRetrievalDone();
    finishNotesFetching();
    
  }
}


void FacebookResource::eventListFetched( KJob* job )
{
  Q_ASSERT( !mIdle );
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  AllEventsListJob * const listJob = dynamic_cast<AllEventsListJob*>( job );
  Q_ASSERT( listJob );
  mCurrentJobs.removeAll(job);

  if ( listJob->error() ) {
    abortWithError( i18n( "Unable to get events from server: %1", listJob->errorString() ),
                    listJob->error() == FacebookJob::AuthenticationProblem );
  } else {
    QStringList eventIds;
    foreach( const EventInfoPtr &event, listJob->allEvents() ) {
      eventIds.append( event->id() );
    }
    if ( eventIds.isEmpty() ) {
      itemsRetrievalDone();
      finishEventsFetching();
      return;
    }
    EventJob * const eventJob = new EventJob( eventIds, Settings::self()->accessToken() );
    mCurrentJobs << eventJob;
    connect( eventJob, SIGNAL(result(KJob*)), this, SLOT(detailedEventListJobFinished(KJob*)) );
    eventJob->start();
  }
}

void FacebookResource::initialItemFetchFinished( KJob* job )
{
  Q_ASSERT(!mIdle);
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  ItemFetchJob * const itemFetchJob = dynamic_cast<ItemFetchJob*>( job );
  Q_ASSERT( itemFetchJob );
  mCurrentJobs.removeAll(job);

  if ( itemFetchJob->error() ) {
    abortWithError( i18n( "Unable to get list of existing friends from the Akonadi server: %1", itemFetchJob->errorString() ) );
  } else {
    foreach( const Item &item, itemFetchJob->items() ) {
      if ( item.hasAttribute<TimeStampAttribute>() )
        mExistingFriends.insert( item.remoteId(), item.attribute<TimeStampAttribute>()->timeStamp() );
    }

    setItemStreamingEnabled( true );
    FriendListJob * const friendListJob = new FriendListJob( Settings::self()->accessToken() );
    mCurrentJobs << friendListJob;
    connect( friendListJob, SIGNAL(result(KJob*)), this, SLOT(friendListJobFinished(KJob*)) );
    emit status( Running, i18n( "Retrieving friends list." ) );
    emit percent( 2 );
    friendListJob->start();
  }
}

void FacebookResource::friendListJobFinished( KJob* job )
{
  Q_ASSERT(!mIdle);
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  FriendListJob * const friendListJob = dynamic_cast<FriendListJob*>( job );
  Q_ASSERT( friendListJob );
  mCurrentJobs.removeAll(job);

  if ( friendListJob->error() ) {
    abortWithError( i18n( "Unable to get list of friends from server: %1", friendListJob->errorText() ),
                    friendListJob->error() == FacebookJob::AuthenticationProblem );
  } else {

    // Figure out which items are new or changed
    foreach( const UserInfoPtr &user, friendListJob->friends() ) {
#if 0 // Bah, Facebook's timestamp doesn't seem to get updated when a user's profile changes :(
      // See http://bugs.developers.facebook.net/show_bug.cgi?id=15475
      const KDateTime stampOfExisting = mExistingFriends.value( user->id(), KDateTime() );
      if ( !stampOfExisting.isValid() ) {
        kDebug() << "Friend" << user->id() << user->name() << "is new!";
        mNewOrChangedFriends.append( user );
      } else if ( user->updatedTime() > stampOfExisting ) {
        kDebug() << "Friend" << user->id() << user->name() << "is updated!";
        mNewOrChangedFriends.append( user );
      } else {
        //kDebug() << "Friend" << user->id() << user->name() << "is old.";
      }
#else
      mNewOrChangedFriends.append( user );
#endif
    }

    // Delete items that are in the Akonadi DB but no on FB
    Item::List removedItems;
    foreach( const QString &friendId, mExistingFriends.keys() ) {
      bool found = false;
      foreach( const UserInfoPtr &user, friendListJob->friends() ) {
        if ( user->id() == friendId ) {
          found = true;
          break;
        }
      }
      if ( !found ) {
        kDebug() << friendId << "is no longer your friend :(";
        Item removedItem;
        removedItem.setRemoteId( friendId );
        removedItems.append( removedItem );
      }
    }
    itemsRetrievedIncremental( Item::List(), removedItems );

    if ( mNewOrChangedFriends.isEmpty() ) {
      finishFriendFetching();
    } else {
      emit status( Running, i18n( "Retrieving friends' details." ) );
      emit percent( 5 );
      fetchNewOrChangedFriends();
    }
  }
}

void FacebookResource::fetchNewOrChangedFriends()
{
  QStringList mewOrChangedFriendIds;
  foreach( const UserInfoPtr &user, mNewOrChangedFriends ) {
    mewOrChangedFriendIds.append( user->id() );
  }
  FriendJob * const friendJob = new FriendJob( mewOrChangedFriendIds, Settings::self()->accessToken() );
  mCurrentJobs << friendJob;
  connect( friendJob, SIGNAL(result(KJob*)), this, SLOT(detailedFriendListJobFinished(KJob*)) );
  friendJob->start();
}

void FacebookResource::detailedEventListJobFinished( KJob* job )
{
  Q_ASSERT( !mIdle );
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  EventJob * const eventJob = dynamic_cast<EventJob*>( job );
  Q_ASSERT( eventJob );
  mCurrentJobs.removeAll(job);

  if ( job->error() ) {
    abortWithError( i18n( "Unable to get list of events from server: %1", eventJob->errorText() ) );
  } else {
    setItemStreamingEnabled( true );

    Item::List eventItems;
    foreach ( const EventInfoPtr &eventInfo, eventJob->eventInfo() ) {
      Item event;
      event.setRemoteId( eventInfo->id() );
      event.setPayload<IncidencePtr>( eventInfo->asEvent() );
      event.setMimeType( eventMimeType );
      eventItems.append( event );
    }
    itemsRetrieved( eventItems );
    itemsRetrievalDone();
    finishEventsFetching();
  }
}

void FacebookResource::detailedFriendListJobFinished( KJob* job )
{
  Q_ASSERT(!mIdle);
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  FriendJob * const friendJob = dynamic_cast<FriendJob*>( job );
  Q_ASSERT( friendJob );
  mCurrentJobs.removeAll(job);

  if ( friendJob->error() ) {
    abortWithError( i18n( "Unable to retrieve friends' information from server: %1", friendJob->errorText() ) );
  } else {
    mPendingFriends = friendJob->friendInfo();
    mNumFriends = mPendingFriends.size();
    emit status( Running, i18n( "Retrieving friends' photos." ) );
    emit percent( 10 );
    fetchPhotos();
  }
}

void FacebookResource::fetchPhotos()
{
  mIdle = false;
  mNumPhotosFetched = 0;
  foreach(const UserInfoPtr f, mPendingFriends) {
    PhotoJob * const photoJob = new PhotoJob(f->id(), Settings::self()->accessToken() );
    mCurrentJobs << photoJob;
    photoJob->setProperty("friend", QVariant::fromValue( f ));
    connect(photoJob, SIGNAL(result(KJob*)), this, SLOT(photoJobFinished(KJob*)));
    photoJob->start();
  }
}

void FacebookResource::finishNotesFetching()
{
  emit percent(100);
  emit status( Idle, i18n( "All notes fetched from server." ) );
  resetState();
}

void FacebookResource::finishEventsFetching()
{
  emit percent(100);
  // TODO: Use an actual number here
  emit status( Idle, i18n( "All events fetched from server." ) );
  resetState();
}

void FacebookResource::finishFriendFetching()
{
  Q_ASSERT(mCurrentJobs.size() == 0);

  mPendingFriends.clear();
  emit percent(100);
  if ( mNumFriends > 0 ) {
    emit status( Idle, i18np( "Updated one friend from the server.",
                              "Updated %1 friends from the server.", mNumFriends ) );
  } else {
    emit status( Idle, i18n( "Finished, no friends needed to be updated." ) );
  }
  resetState();
}

void FacebookResource::photoJobFinished(KJob* job)
{
  Q_ASSERT(!mIdle);
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  PhotoJob * const photoJob = dynamic_cast<PhotoJob*>( job );
  Q_ASSERT(photoJob);
  UserInfoPtr user = job->property("friend").value<UserInfoPtr>();
  mCurrentJobs.removeOne(job);

  if (photoJob->error()) {
    abortWithError( i18n( "Unable to retrieve friends' photo from server: %1", photoJob->errorText() ) );
  } else {
    // Create Item
    KABC::Addressee addressee = user->toAddressee();
    addressee.setPhoto( KABC::Picture( photoJob->photo() ) );
    Item newUser;
    newUser.setRemoteId( user->id() );
    newUser.setMimeType( "text/directory" );
    newUser.setPayload<KABC::Addressee>( addressee );
    TimeStampAttribute * const timeStampAttribute = new TimeStampAttribute();
    timeStampAttribute->setTimeStamp( user->updatedTime() );
    newUser.addAttribute( timeStampAttribute );

    itemsRetrievedIncremental( Item::List() << newUser, Item::List() );
    mNumPhotosFetched++;

    if (mNumPhotosFetched != mNumFriends) {
      const int alreadyDownloadedFriends = mNumFriends - mPendingFriends.size();
      const float percentageDone = alreadyDownloadedFriends / (float)mNumFriends * 100.0f;
      emit percent(10 + percentageDone * 0.9f);
    } else {
      itemsRetrievalDone();
      finishFriendFetching();
    }
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

void FacebookResource::noteJobFinished(KJob* job)
{
  Q_ASSERT(!mIdle);
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  NoteJob * const noteJob = dynamic_cast<NoteJob*>( job );
  Q_ASSERT( noteJob );
  Q_ASSERT( noteJob->noteInfo().size() == 1 );
  mCurrentJobs.removeAll(job);

  if ( noteJob->error() ) {
    abortWithError( i18n( "Unable to get information about note from server: %1", noteJob->errorText() ) );
  } else {
    Item note = noteJob->property( "Item" ).value<Item>();
    note.setPayload( noteJob->noteInfo().first()->asNote() );
    itemRetrieved( note );
    resetState();
  }
}

void FacebookResource::friendJobFinished(KJob* job)
{
  Q_ASSERT(!mIdle);
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  FriendJob * const friendJob = dynamic_cast<FriendJob*>( job );
  Q_ASSERT( friendJob );
  Q_ASSERT( friendJob->friendInfo().size() == 1 );
  mCurrentJobs.removeAll(job);

  if ( friendJob->error() ) {
    abortWithError( i18n( "Unable to get information about friend from server: %1", friendJob->errorText() ) );
  } else {
    Item user = friendJob->property( "Item" ).value<Item>();
    user.setPayload<KABC::Addressee>( friendJob->friendInfo().first()->toAddressee() );
    // TODO: What about picture here?
    itemRetrieved( user );
    resetState();
  }
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
  notes.setRights( Collection::ReadOnly      | Collection::CanChangeItem | 
                   Collection::CanDeleteItem | Collection::CanCreateItem );
  EntityDisplayAttribute * const notesDisplayAttribute = new EntityDisplayAttribute();
  notesDisplayAttribute->setIconName( "facebookresource" );
  notes.addAttribute( notesDisplayAttribute );

  collectionsRetrieved( Collection::List() << friends << events << notes );
}

void FacebookResource::itemRemoved( const Akonadi::Item &item)
{
  /*
   * Delete a note
   * TODO: handle signal! and signal akonadi etc! whoohoo
   */
  if (item.mimeType() == "text/x-vnd.akonadi.note") {
    mIdle = false;
    FacebookDeleteJob * const deleteJob = new FacebookDeleteJob( item.remoteId(),
                                               Settings::self()->accessToken() );
    mCurrentJobs << deleteJob;
    deleteJob->setProperty( "Item", QVariant::fromValue( item ) );
    connect( deleteJob, SIGNAL(result(KJob*)), this, SLOT(deleteJobFinished(KJob*)) );
    deleteJob->start();
  }
}

void FacebookResource::deleteJobFinished(KJob *job)
{
  Q_ASSERT(!mIdle);
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  mCurrentJobs.removeAll(job);
  Item item = job->property( "Item" ).value<Item>(); 
  changeCommitted( item );
}

void FacebookResource::itemAdded( const Akonadi::Item &item, const Akonadi::Collection &collection )
{
  /*
   * A note is added!
   */
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
    }
  } 
}

void FacebookResource::noteAddJobFinished(KJob *job)
{
  Q_ASSERT( !mIdle );
  Q_ASSERT( mCurrentJobs.indexOf(job) != -1 );
  NoteAddJob * const addJob = dynamic_cast<NoteAddJob*>( job );
  Q_ASSERT( addJob );
  mCurrentJobs.removeAll(job);

  Item note = addJob->property( "Item" ).value<Item>();
  note.setRemoteId(addJob->property( "id" ).value<QString>());

  mIdle = true;
  mCurrentJobs.clear();

  changeCommitted( note );
}

AKONADI_RESOURCE_MAIN( FacebookResource )

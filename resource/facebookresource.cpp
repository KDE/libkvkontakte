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
#include "settings.h"
#include "settingsdialog.h"
#include "timestampattribute.h"

#include <libkfacebook/friendlistjob.h>
#include <libkfacebook/friendjob.h>
#include <libkfacebook/photojob.h>
#include <libkfacebook/alleventslistjob.h>
#include <libkfacebook/eventjob.h>

#include <Akonadi/AttributeFactory>
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>

using namespace Akonadi;

static const char * friendsRID = "friends";
static const char * eventsRID = "events";

FacebookResource::FacebookResource( const QString &id )
    : ResourceBase( id )
{
  AttributeFactory::registerAttribute<TimeStampAttribute>();
  setNeedsNetwork( true );
  setAutomaticProgressReporting( false );
  setObjectName( QLatin1String( "FacebookResource" ) );
  resetState();
  Settings::self()->setResourceId( identifier() );

  connect( this, SIGNAL(abortRequested()),
           this, SLOT(slotAbortRequested()) );
  connect( this, SIGNAL(reloadConfiguration()), SLOT(configurationChanged()) );
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

void FacebookResource::abortWithError(const QString errorMessage)
{
  resetState();
  cancelTask(errorMessage);
}

void FacebookResource::resetState()
{
  mIdle = true;
  mNumFriends = -1;
  mCurrentJob = 0;
  mExistingFriends.clear();
  mNewOrChangedFriends.clear();
}

void FacebookResource::slotAbortRequested()
{
  if (!mIdle) {
    if (mCurrentJob) {
      kDebug() << "Killing current job:" << mCurrentJob;
      mCurrentJob->kill(KJob::Quietly);
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
    mCurrentJob = fetchJob;
    connect( fetchJob, SIGNAL(result(KJob*)), this, SLOT(initialItemFetchFinished(KJob*)) );
  } else if ( collection.remoteId() == eventsRID ) {
    mIdle = false;
    emit status( Running, i18n( "Preparing sync of events list." ) );
    emit percent( 0 );
    AllEventsListJob * const listJob = new AllEventsListJob( Settings::self()->accessToken() );
    listJob->setLowerLimit(KDateTime::fromString( Settings::self()->lowerLimit(), "%Y-%m-%d" ));
    mCurrentJob = listJob;
    connect( listJob, SIGNAL(result(KJob*)), this, SLOT(eventListFetched(KJob*)) );
    listJob->start();
  } else {
    cancelTask( i18n( "Unable to syncronize this collection." ) );
  }
}

void FacebookResource::eventListFetched( KJob* job )
{
  Q_ASSERT( !mIdle );
  AllEventsListJob * const listJob = dynamic_cast<AllEventsListJob*>( job );
  Q_ASSERT( listJob );
  if ( listJob->error() ) {
    abortWithError( i18n( "Unable to get events from server: %1", listJob->errorString() ) );
  } else {
    QStringList eventIds;
    foreach( const EventInfoPtr &event, listJob->allEvents() ) {
      eventIds.append( event->id() );
    }
    EventJob * const eventJob = new EventJob( eventIds, Settings::self()->accessToken() );
    mCurrentJob = eventJob;
    connect( eventJob, SIGNAL(result(KJob*)), this, SLOT(detailedEventListJobFinished(KJob*)) );
    eventJob->start();
  }
}

void FacebookResource::initialItemFetchFinished( KJob* job )
{
  Q_ASSERT(!mIdle);
  ItemFetchJob * const itemFetchJob = dynamic_cast<ItemFetchJob*>( job );
  Q_ASSERT( itemFetchJob );
  if ( itemFetchJob->error() ) {
    abortWithError( i18n( "Unable to get list of existing friends from the Akonadi server: %1", itemFetchJob->errorString() ) );
  } else {
    foreach( const Item &item, itemFetchJob->items() ) {
      mExistingFriends.insert( item.remoteId(), item.attribute<TimeStampAttribute>()->timeStamp() );
    }

    setItemStreamingEnabled( true );
    FriendListJob * const friendListJob = new FriendListJob( Settings::self()->accessToken() );
    mCurrentJob = friendListJob;
    connect( friendListJob, SIGNAL(result(KJob*)), this, SLOT(friendListJobFinished(KJob*)) );
    emit status( Running, i18n( "Retrieving friends list." ) );
    emit percent( 2 );
    friendListJob->start();
  }
}

void FacebookResource::friendListJobFinished( KJob* job )
{
  Q_ASSERT(!mIdle);
  FriendListJob * const friendListJob = dynamic_cast<FriendListJob*>( job );
  Q_ASSERT( friendListJob );
  if ( friendListJob->error() ) {
    abortWithError( i18n( "Unable to get list of friends from server: %1", friendListJob->errorText() ) );
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
  mCurrentJob = friendJob;
  connect( friendJob, SIGNAL(result(KJob*)), this, SLOT(detailedFriendListJobFinished(KJob*)) );
  friendJob->start();
}

void FacebookResource::detailedEventListJobFinished( KJob* job )
{
  Q_ASSERT( !mIdle );
  Q_ASSERT( job == mCurrentJob );
  EventJob * const eventJob = dynamic_cast<EventJob*>( job );
  Q_ASSERT( eventJob );
  if ( job->error() ) {
  } else {
    setItemStreamingEnabled( true );

    Item::List eventItems;
    foreach ( const EventInfoPtr &eventInfo, eventJob->eventInfo() ) {
      Item event;
      event.setRemoteId( eventInfo->id() );
      event.setPayload<KCalCore::Incidence::Ptr>( eventInfo->asEvent() );
      event.setMimeType( eventInfo->asEvent()->mimeType() );
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
  FriendJob * const friendJob = dynamic_cast<FriendJob*>( job );
  Q_ASSERT( friendJob );

  if ( friendJob->error() ) {
    abortWithError( i18n( "Unable to retrieve friends' information from server: %1", friendJob->errorText() ) );
  } else {
    mPendingFriends = friendJob->friendInfo();
    mNumFriends = mPendingFriends.size();
    emit status( Running, i18n( "Retrieving friends' photos." ) );
    emit percent( 10 );
    fetchNextPhoto();
  }
}

void FacebookResource::fetchNextPhoto()
{
  if (mPendingFriends.isEmpty()) {
    itemsRetrievalDone();
    finishFriendFetching();
  } else {
    PhotoJob * const photoJob = new PhotoJob( mPendingFriends.first()->id(), Settings::self()->accessToken() );
    mCurrentJob = photoJob;
    connect(photoJob, SIGNAL(result(KJob*)), this, SLOT(photoJobFinished(KJob*)));
    photoJob->start();
  }
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
  Q_ASSERT( mPendingFriends.isEmpty() );

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
  PhotoJob * const photoJob = dynamic_cast<PhotoJob*>( job );
  Q_ASSERT(photoJob);
  Q_ASSERT(!mPendingFriends.isEmpty());
  if (photoJob->error()) {
    abortWithError( i18n( "Unable to retrieve friends' photo from server: %1", photoJob->errorText() ) );
  } else {

    // Update lists
    const UserInfoPtr user = mPendingFriends.first();
    mPendingFriends.removeFirst();

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

    // Done!
    itemsRetrievedIncremental( Item::List() << newUser, Item::List() );
    if (!mPendingFriends.isEmpty()) {
      const int alreadyDownloadedFriends = mNumFriends - mPendingFriends.size();
      const float percentageDone = alreadyDownloadedFriends / (float)mNumFriends * 100.0f;
      emit percent(10 + percentageDone * 0.9f);
    }
    fetchNextPhoto();
  }
}

bool FacebookResource::retrieveItem( const Akonadi::Item &item, const QSet<QByteArray> &parts )
{
  Q_UNUSED( parts );
  // TODO: Is this ever called??
  mIdle = false;
  FriendJob * const friendJob = new FriendJob( item.remoteId(),
                                               Settings::self()->accessToken() );
  mCurrentJob = friendJob;
  friendJob->setProperty( "Item", QVariant::fromValue( item ) );
  connect( friendJob, SIGNAL(result(KJob*)), this, SLOT(friendJobFinished(KJob*)) );
  friendJob->start();
  return true;
}

void FacebookResource::friendJobFinished(KJob* job)
{
  Q_ASSERT(!mIdle);
  FriendJob * const friendJob = dynamic_cast<FriendJob*>( job );
  Q_ASSERT( friendJob );
  Q_ASSERT( friendJob->friendInfo().size() == 1 );
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
  events.setContentMimeTypes( QStringList() << "text/calendar" << KCalCore::Event::eventMimeType() );
  events.setRights( Collection::ReadOnly );
  EntityDisplayAttribute * const evendDisplayAttribute = new EntityDisplayAttribute();
  evendDisplayAttribute->setIconName( "facebookresource" );
  events.addAttribute( evendDisplayAttribute );

  collectionsRetrieved( Collection::List() << friends << events );
}

AKONADI_RESOURCE_MAIN( FacebookResource )

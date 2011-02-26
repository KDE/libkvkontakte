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
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>

using namespace Akonadi;

static const char * friendsRID = "friends";

FacebookResource::FacebookResource( const QString &id )
    : ResourceBase( id )
{
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
  Q_UNUSED( collection );
  Q_ASSERT(mIdle);
  mIdle = false;
  emit status( Running, i18n( "Preparing sync of friends list." ) );
  emit percent( 0 );
  ItemFetchJob * const fetchJob = new ItemFetchJob( collection );
  fetchJob->setObjectName( QLatin1String( "InitialFetchJob" ) );
  fetchJob->fetchScope().fetchAttribute<TimeStampAttribute>();
  fetchJob->fetchScope().fetchFullPayload( false );
  mCurrentJob = fetchJob;
  connect( fetchJob, SIGNAL(result(KJob*)), this, SLOT(initialItemFetchFinished(KJob*)) );
}

void FacebookResource::initialItemFetchFinished( KJob* job )
{
  Q_ASSERT(!mIdle);
  ItemFetchJob * const itemFetchJob = dynamic_cast<ItemFetchJob*>( job );
  Q_ASSERT( itemFetchJob );
  if ( itemFetchJob->error() ) {
    abortWithError( i18n( "Unable to get list of existing friends from the Akonadi server: %1", itemFetchJob->errorString() ) );
  } else {
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
    QStringList friendIds;
    foreach( const UserInfoPtr &user, friendListJob->friends() ) {
      friendIds << user->id();
    }
    emit status( Running, i18n( "Retrieving friends' details." ) );
    emit percent( 5 );
    FriendJob * const friendJob = new FriendJob( friendIds, Settings::self()->accessToken() );
    mCurrentJob = friendJob;
    connect( friendJob, SIGNAL(result(KJob*)), this, SLOT(detailedFriendListJobFinished(KJob*)) );
    friendJob->start();
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
    emit percent(100);
    emit status( Idle, i18n( "Fetched %1 friends from the server.", mNumFriends ) );
    resetState();
  } else {
    PhotoJob * const photoJob = new PhotoJob( mPendingFriends.first()->id(), Settings::self()->accessToken() );
    mCurrentJob = photoJob;
    connect(photoJob, SIGNAL(result(KJob*)), this, SLOT(photoJobFinished(KJob*)));
    photoJob->start();
  }
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
    const UserInfoPtr user = mPendingFriends.first();
    KABC::Addressee addressee = user->toAddressee();
    addressee.setPhoto(KABC::Picture(photoJob->photo()));
    Item newUser;
    newUser.setRemoteId( user->id() );
    newUser.setMimeType( "text/directory" );
    newUser.setPayload<KABC::Addressee>( addressee );
    TimeStampAttribute * const timeStampAttribute = new TimeStampAttribute();
    timeStampAttribute->setTimeStamp(user->updatedTime());
    newUser.addAttribute(timeStampAttribute);
    itemsRetrieved( Akonadi::Item::List() << newUser );
    mPendingFriends.removeFirst();
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
  EntityDisplayAttribute *displayAttribute = new EntityDisplayAttribute();
  displayAttribute->setIconName( "facebookresource" );
  friends.addAttribute( displayAttribute );
  collectionsRetrieved( Collection::List() << friends );
}

AKONADI_RESOURCE_MAIN( FacebookResource )

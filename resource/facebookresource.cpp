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

#include <libkfacebook/friendlistjob.h>
#include <libkfacebook/friendjob.h>
#include <Akonadi/EntityDisplayAttribute>

using namespace Akonadi;

static const char * friendsRID = "friends";

FacebookResource::FacebookResource( const QString &id )
    : ResourceBase( id )
{
  setNeedsNetwork( true );
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
  kDebug();
}

void FacebookResource::slotAbortRequested()
{
  kDebug();
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
  FriendListJob * const friendListJob = new FriendListJob( Settings::self()->accessToken() );
  connect( friendListJob, SIGNAL(result(KJob*)), this, SLOT(friendListJobFinished(KJob*)) );
  friendListJob->start();
}

void FacebookResource::friendListJobFinished( KJob* job )
{
  FriendListJob * const friendListJob = dynamic_cast<FriendListJob*>( job );
  Q_ASSERT( friendListJob );
  if ( friendListJob->error() ) {
    cancelTask( i18n( "Unable to get list of friends from server: %1", friendListJob->errorText() ) );
  } else {
    QList<Item> friends;
    foreach( const UserInfoPtr &user, friendListJob->friends() ) {
      Item newUser;
      newUser.setRemoteId( user->id() );
      newUser.setMimeType( "text/directory" );
      friends << newUser;
    }
    itemsRetrieved( friends );
  }
}

bool FacebookResource::retrieveItem( const Akonadi::Item &item, const QSet<QByteArray> &parts )
{
  Q_UNUSED( parts );
  // FIXME: This is not called if an item changes on Facebook, even after calling retrieveItems()!
  // TODO: Picture support
  FriendJob * const friendJob = new FriendJob( item.remoteId(), Settings::self()->accessToken() );
  friendJob->setProperty( "Item", QVariant::fromValue( item ) );
  connect( friendJob, SIGNAL(result(KJob*)), this, SLOT(friendJobFinished(KJob*)) );
  friendJob->start();
  return true;
}

void FacebookResource::friendJobFinished(KJob* job)
{
  FriendJob * const friendJob = dynamic_cast<FriendJob*>( job );
  Q_ASSERT( friendJob );
  if ( friendJob->error() ) {
    cancelTask( i18n( "Unable to get information about friend from server: %1", friendJob->errorText() ) );
  } else {
    Item user = friendJob->property( "Item" ).value<Item>();
    user.setPayload<KABC::Addressee>( friendJob->friendInfo()->toAddressee() );
    itemRetrieved( user );
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

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

using namespace Akonadi;

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
  QPointer<SettingsDialog> settingsDialog( new SettingsDialog( this, windowId ) );
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
  kDebug();
}

bool FacebookResource::retrieveItem( const Akonadi::Item &item, const QSet<QByteArray> &parts )
{
  Q_UNUSED( item );
  Q_UNUSED( parts );
  kDebug();
  return false;
}

void FacebookResource::retrieveCollections()
{
  kDebug();
}

AKONADI_RESOURCE_MAIN( FacebookResource )

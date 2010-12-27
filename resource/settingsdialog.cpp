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
#include "settingsdialog.h"
#include "facebookresource.h"
#include "settings.h"
#include "authenticationwidget.h"

#include <KWindowSystem>

using namespace Akonadi;

SettingsDialog::SettingsDialog( FacebookResource *parentResource, WId parentWindow )
  : KDialog(),
    mParentResource( parentResource )
{
  KWindowSystem::setMainWindow( this, parentWindow );
  setButtons( Ok|Cancel );
  setWindowIcon( KIcon( "facebookresource" ) );
  setWindowTitle( i18n("Facebook Settings") );

  setupWidgets();
  loadSettings();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::setupWidgets()
{
  QWidget *page = new QWidget( this );
  setupUi( page );
  setMainWidget( page );
  updateAuthenticationWidgets();
  connect( resetButton, SIGNAL(clicked(bool)), this, SLOT(resetAuthentication()) );
  connect( authenticateButton, SIGNAL(clicked(bool)), this, SLOT(showAuthenticationDialog()) );
}

void SettingsDialog::showAuthenticationDialog()
{
  AuthenticationDialog *authDialog = new AuthenticationDialog( this );
  connect( authDialog, SIGNAL(authenticated(QString)),
           this, SLOT(authenticationDone(QString)) );
  connect( authDialog, SIGNAL(canceled()),
           this, SLOT(authenticationCanceled()) );
  authDialog->show();
  authenticateButton->setEnabled( false );
}

void SettingsDialog::authenticationCanceled()
{
  authenticateButton->setEnabled( true );
}

void SettingsDialog::authenticationDone(const QString& accessToken)
{
  Settings::self()->setAccessToken( accessToken );
  updateAuthenticationWidgets();
}

void SettingsDialog::updateAuthenticationWidgets()
{
  if ( Settings::self()->accessToken().isEmpty() ) {
    authenticationStack->setCurrentIndex( 0 );
  } else {
    authenticationStack->setCurrentIndex( 1 );
    //authenticationLabel->setText( i18n( "Authenticated as <b>%1</b>.", Settings::self()->userName() ) );
    authenticationLabel->setText( i18n( "Authenticated." ) );
  }
}

void SettingsDialog::resetAuthentication()
{
  Settings::self()->setAccessToken( QString() );
  updateAuthenticationWidgets();
}

void SettingsDialog::loadSettings()
{
  if ( mParentResource->name() == mParentResource->identifier() )
    mParentResource->setName( i18n( "Faceboook Account") );

  nameEdit->setText( mParentResource->name() );
  nameEdit->setFocus();
}

void SettingsDialog::saveSettings()
{
  mParentResource->setName( nameEdit->text() );
  Settings::self()->writeConfig();
}

void SettingsDialog::slotButtonClicked( int button )
{
  switch( button ) {
  case Ok:
    saveSettings();
    accept();
    break;
  case Cancel:
    reject();
    return;
  }
}

#include "settingsdialog.moc"

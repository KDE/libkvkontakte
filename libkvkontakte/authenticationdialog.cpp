/* Copyright 2010 Thomas McGuire <mcguire@kde.org>
   Copyright 2011 Alexander Potashev <aspotashev@gmail.com>

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
#include "authenticationdialog.h"

#include <KLocale>
#include <KWebView>
#include <KMessageBox>
#include <KDebug>
#include <QVBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QProgressBar>

namespace Vkontakte
{

AuthenticationDialog::AuthenticationDialog( QWidget* parent )
  : KDialog(parent)
{
  setButtons( KDialog::Cancel );
  setCaption( i18n( "Authenticate with VKontakte" ) );
  setAttribute( Qt::WA_DeleteOnClose, true );

  QWidget * const widget = new QWidget( this );
  QVBoxLayout * const layout = new QVBoxLayout( widget );
  QWidget * const progressWidget = new QWidget( this );
  QHBoxLayout * const progressLayout = new QHBoxLayout( progressWidget );
  progressLayout->setMargin( 0 );
  layout->setMargin( 0 );
  setMainWidget( widget );
  mWebView = new KWebView( this );

  mProgressBar = new QProgressBar( this );
  mProgressBar->setRange( 0, 100 );
  QLabel * const progressLabel = new QLabel( i18n( "Loading Page:" ), this );
  progressLayout->addWidget( progressLabel );
  progressLayout->addWidget( mProgressBar );

  layout->addWidget( progressWidget );
  layout->addWidget( mWebView );

  connect( this, SIGNAL(cancelClicked()), SIGNAL(canceled()) );
  connect( mWebView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)) );
  connect( mWebView, SIGNAL(loadStarted()), progressWidget, SLOT(show()) );
  connect( mWebView, SIGNAL(loadFinished(bool)), progressWidget, SLOT(hide()) );
  connect( mWebView, SIGNAL(loadProgress(int)), mProgressBar, SLOT(setValue(int)) );
}

void AuthenticationDialog::setAppId( const QString& appId )
{
  mAppId = appId;
}

void AuthenticationDialog::setPermissions( const QStringList& permissions )
{
  mPermissions = permissions;
}

void AuthenticationDialog::start()
{
  Q_ASSERT( !mAppId.isEmpty() );

  // display= {page, popup, touch, wap}
  const QString url = QString( "http://api.vkontakte.ru/oauth/authorize?"
                               "client_id=%1&"
                               "scope=%2&"
                               "redirect_uri=http://api.vkontakte.ru/blank.html&"
                               "display=page&" // TODO: this should be configurable
                               "response_type=token")
                               .arg( mAppId )
                               .arg( mPermissions.join( "," ) );
  kDebug() << "Showing" << url;
  mWebView->setUrl( QUrl::fromUserInput( url ) );
  show();
}

void AuthenticationDialog::showErrorDialog()
{
  hide();
  const QString details = i18n( "<b>VKontakte Error Description:</b> %1<br>"
                                "<b>VKontakte Error:</b> %2<br>", mErrorDescription, mError );
  KMessageBox::detailedSorry( this, i18n( "Authentication with VKontakte was not successful." ),
                              details, i18n( "Authentication Problem." ) );
  emit canceled();
  close();
}

void AuthenticationDialog::urlChanged( const QUrl& url )
{
  kDebug() << "Navigating to" << url;
  if ( url.host() == "api.vkontakte.ru" && url.path() == "/blank.html" ) {
    mError = url.queryItemValue( "error" );
    mErrorDescription = url.queryItemValue( "error_description" ).replace( '+', ' ' );
    if ( !mError.isEmpty() || !mErrorDescription.isEmpty() ) {
      QTimer::singleShot( 0, this, SLOT( showErrorDialog() ) );
      return;
    }

    // The URL comes in the form "bla#access_token=bla&expires_in=foo", we need to convert from
    // # to ?
    const QUrl fixedURL = QUrl::fromUserInput( url.toString().replace( '#', '?' ) );
    const QString accessToken = fixedURL.queryItemValue( "access_token" );
    const QString tokenExpiresIn = fixedURL.queryItemValue( "expires_in" ); // TODO: use this for something?
    if ( !accessToken.isEmpty() ) {
      emit authenticated( accessToken );
      QTimer::singleShot( 0, this, SLOT( close() ) );
    }
  }
}

} /* namespace Vkontakte */

#include "authenticationdialog.moc"

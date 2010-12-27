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
#include "authenticationdialog.h"

#include <KLocale>
#include <KWebView>
#include <KMessageBox>
#include <KDebug>
#include <QVBoxLayout>
#include <QTimer>
#include <QLabel>
#include <QProgressBar>

AuthenticationDialog::AuthenticationDialog( QWidget* parent )
  : KDialog(parent)
{
  setButtons( KDialog::Cancel );
  setCaption( i18n( "Authenticate with Facebook" ) );
  setAttribute( Qt::WA_DeleteOnClose, true );

  QWidget *widget = new QWidget( this );
  QVBoxLayout *layout = new QVBoxLayout( widget );
  QWidget *progressWidget = new QWidget( this );
  QHBoxLayout *progressLayout = new QHBoxLayout( progressWidget );
  progressLayout->setMargin( 0 );
  layout->setMargin( 0 );
  setMainWidget( widget );
  mWebView = new KWebView( this );

  mProgressBar = new QProgressBar( this );
  mProgressBar->setRange( 0, 100 );
  QLabel *progressLabel = new QLabel( i18n( "Loading Page:" ), this );
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

  const QString url = QString( "https://graph.facebook.com/oauth/authorize?"
                               "client_id=%1&"
                               "redirect_uri=http://www.facebook.com/connect/login_success.html&"
                               "type=user_agent&"
                               "scope=%2" ).arg( mAppId )
                                           .arg( mPermissions.join( "," ) );
  mWebView->setUrl( QUrl::fromUserInput( url ) );
  show();
}

void AuthenticationDialog::showErrorDialog()
{
  hide();
  const QString details = i18n( "<b>Facebook Error Description:</b> %1<br>"
                                "<b>Facebook Error:</b> %2<br>"
                                "<b>Facebook Error Reason:</b> %3", mErrorDescription, mError, mErrorReason );
  KMessageBox::detailedSorry( this, i18n( "Authentication with Facebook was not successful." ),
                              details, i18n( "Authentication Problem." ) );
  emit canceled();
  close();
}

void AuthenticationDialog::urlChanged( const QUrl& url )
{
  kDebug() << "Navigating to" << url;
  if ( url.host() == "www.facebook.com" && url.path() == "/connect/login_success.html" ) {
    mErrorReason = url.queryItemValue( "error_reason" );
    mError = url.queryItemValue( "error" );
    mErrorDescription = url.queryItemValue( "error_description" ).replace( '+', ' ' );
    if ( !mError.isEmpty() || !mErrorReason.isEmpty() || !mErrorDescription.isEmpty() ) {
      QTimer::singleShot( 0, this, SLOT( showErrorDialog() ) );
      return;
    }

    // The URL comes in the form "bla#access_token=bla&expires_in=foo", we need to convert from
    // # to ?
    const QUrl fixedURL = QUrl::fromUserInput( url.toString().replace( '#', '?' ) );
    const QString accessToken = fixedURL.queryItemValue( "access_token" );
    if ( !accessToken.isEmpty() ) {
      emit authenticated( accessToken );
      QTimer::singleShot( 0, this, SLOT( close() ) );
    }
  }
}


#include "authenticationdialog.moc"

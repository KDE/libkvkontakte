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

class AuthenticationDialog::Private
{
public:
    QString appId;
    QStringList permissions;
    KWebView *webView;
    QProgressBar *progressBar;
    QString error;
    QString errorDescription;
};

AuthenticationDialog::AuthenticationDialog(QWidget *parent)
    : KDialog(parent)
    , d(new Private)
{
    setButtons(KDialog::Cancel);
    setCaption(i18nc("@title:window", "Authenticate with VKontakte"));
    setAttribute(Qt::WA_DeleteOnClose, true);

    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    QWidget *progressWidget = new QWidget(this);
    QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
    progressLayout->setMargin(0);
    layout->setMargin(0);
    setMainWidget(widget);
    d->webView = new KWebView(this);

    d->progressBar = new QProgressBar(this);
    d->progressBar->setRange(0, 100);
    QLabel *progressLabel = new QLabel(i18n("Loading Page:"), this);
    progressLayout->addWidget(progressLabel);
    progressLayout->addWidget(d->progressBar);

    layout->addWidget(progressWidget);
    layout->addWidget(d->webView);

    connect(this, SIGNAL(cancelClicked()), SIGNAL(canceled()));
    connect(d->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
    connect(d->webView, SIGNAL(loadStarted()), progressWidget, SLOT(show()));
    connect(d->webView, SIGNAL(loadFinished(bool)), progressWidget, SLOT(hide()));
    connect(d->webView, SIGNAL(loadProgress(int)), d->progressBar, SLOT(setValue(int)));
    connect(d->webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
}

AuthenticationDialog::~AuthenticationDialog()
{
    delete d;
}

void AuthenticationDialog::setAppId(const QString &appId)
{
    d->appId = appId;
}

void AuthenticationDialog::setPermissions(const QStringList &permissions)
{
    d->permissions = permissions;
}

void AuthenticationDialog::start()
{
    Q_ASSERT(!d->appId.isEmpty());

    // display= {page, popup, touch, wap}
    const QString url = QString("http://api.vkontakte.ru/oauth/authorize?"
                                "client_id=%1&"
                                "scope=%2&"
                                "redirect_uri=http://api.vkontakte.ru/blank.html&"
                                "display=page&" // TODO: this should be configurable
                                "response_type=token")
                                .arg(d->appId)
                                .arg(d->permissions.join(","));
    kDebug() << "Showing" << url;
    d->webView->setUrl(QUrl::fromUserInput(url));
    show();
}

void AuthenticationDialog::showErrorDialog()
{
    hide();
    const QString details = i18n("<b>VKontakte Error Description:</b> %1<br>"
                                 "<b>VKontakte Error:</b> %2<br>", d->errorDescription, d->error);
    KMessageBox::detailedSorry(this, i18n("Authentication with VKontakte was not successful."),
                               details, i18nc("@title:window", "Authentication Problem"));
    emit canceled();
    close();
}

void AuthenticationDialog::urlChanged(const QUrl &url)
{
    kDebug() << "Navigating to" << url;
    if (url.host() == "api.vkontakte.ru" && url.path() == "/blank.html")
    {
        d->error = url.queryItemValue("error");
        d->errorDescription = url.queryItemValue("error_description").replace('+', ' ');
        if (!d->error.isEmpty() || !d->errorDescription.isEmpty())
        {
            QTimer::singleShot(0, this, SLOT(showErrorDialog()));
            return;
        }

        // The URL comes in the form "bla#access_token=bla&expires_in=foo", we need to convert from
        // # to ?
        const QUrl fixedURL = QUrl::fromUserInput(url.toString().replace('#', '?'));
        const QString accessToken = fixedURL.queryItemValue("access_token");
        const QString tokenExpiresIn = fixedURL.queryItemValue("expires_in"); // TODO: use this for something?
        if (!accessToken.isEmpty())
        {
            emit authenticated(accessToken);
            QTimer::singleShot(0, this, SLOT(close()));
        }
    }
}

void AuthenticationDialog::loadFinished(bool ok)
{
    if (!ok)
    {
        emit canceled();
        close();

        KMessageBox::error(parentWidget(),
                           i18n("There was a network error when trying to authenticate with VKontakte web service."),
                           i18nc("@title:window", "Network Error"));
    }
}

} /* namespace Vkontakte */

#include "authenticationdialog.moc"

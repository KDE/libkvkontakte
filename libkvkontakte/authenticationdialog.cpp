/*
 * Copyright (C) 2010  Thomas McGuire <mcguire@kde.org>
 * Copyright (C) 2011  Alexander Potashev <aspotashev@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "authenticationdialog.h"

#include "util.h"

#include <KLocalizedString>
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
    Vkontakte::AppPermissions::Value permissions;
    QString displayMode;

    KWebView *webView;
    QProgressBar *progressBar;

    QString error;
    QString errorDescription;
};

AuthenticationDialog::AuthenticationDialog(QWidget *parent)
    : KDialog(parent)
    , d(new Private)
{
    d->displayMode = "page";

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
    setPermissions(appPermissionsFromStringList(permissions));
}

void Vkontakte::AuthenticationDialog::setPermissions(Vkontakte::AppPermissions::Value permissions)
{
    d->permissions = permissions;
}

// display= {page, popup, touch, wap}
void AuthenticationDialog::setDisplayMode(const QString &displayMode)
{
    d->displayMode = displayMode;
}

void AuthenticationDialog::start()
{
    Q_ASSERT(!d->appId.isEmpty());

    const QString url = QString("http://oauth.vk.com/authorize?"
                                "client_id=%1&"
                                "scope=%2&"
                                "redirect_uri=http://oauth.vk.com/blank.html&"
                                "display=%3&"
                                "response_type=token")
                                .arg(d->appId)
                                .arg(appPermissionsToStringList(d->permissions).join(","))
                                .arg(d->displayMode);
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
    if (url.host() == "oauth.vk.com" && url.path() == "/blank.html")
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
        hide();

        KMessageBox::error(parentWidget(),
                           i18n("There was a network error when trying to authenticate with VKontakte web service."),
                           i18nc("@title:window", "Network Error"));

        emit canceled();
        close();
    }
}

} /* namespace Vkontakte */

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
#ifndef FACEBOOKRESOURCE_H
#define FACEBOOKRESOURCE_H

#include <libkfacebook/userinfo.h>
#include <Akonadi/ResourceBase>

class FacebookResource : public Akonadi::ResourceBase,
                         public Akonadi::AgentBase::Observer
{
  Q_OBJECT

  public:
    FacebookResource( const QString &id );
    ~FacebookResource();

  public Q_SLOTS:
    virtual void configure( WId windowId );

  protected Q_SLOTS:
    void retrieveCollections();
    void retrieveItems( const Akonadi::Collection &col );
    bool retrieveItem( const Akonadi::Item &item, const QSet<QByteArray> &parts );

  protected:

    virtual void aboutToQuit();

  private Q_SLOTS:

    void slotAbortRequested();
    void configurationChanged();
    void friendListJobFinished( KJob *job );
    void friendJobFinished( KJob *job );
    void photoJobFinished( KJob *job );
    void detailedFriendListJobFinished( KJob *job );

  private:
    void fetchNextPhoto();

    QList<UserInfoPtr> mPendingFriends;
};

#endif

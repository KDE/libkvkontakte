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
#ifndef USERINFOJOB_H
#define USERINFOJOB_H

#include "libkfacebook_export.h"

#include <KJob>
#include <QSharedPointer>

class LIBKFACEBOOK_EXPORT UserInfo : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString id READ id WRITE setId)
  Q_PROPERTY(QString name READ name WRITE setName)

  public:
    void setId( const QString &id );
    QString id() const;
    void setName( const QString &name );
    QString name() const;

  private:
    QString mId;
    QString mName;
};

typedef QSharedPointer<UserInfo> UserInfoPtr;

/// Gets information about the user that is authenticated
class LIBKFACEBOOK_EXPORT UserInfoJob : public KJob
{
  Q_OBJECT
  public:
    UserInfoJob( const QString &accessToken );
    virtual void start();
    UserInfoPtr userInfo() const;

  private slots:
    void getJobFinished( KJob *job );

  private:
    QString mAccessToken;
    UserInfoPtr mUserInfo;
};

#endif

/* Copyright 2010, 2011 Thomas McGuire <mcguire@kde.org>

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
#ifndef USERINFO_H
#define USERINFO_H

#include "libkfacebook_export.h"

#include <KABC/Addressee>
#include <KDateTime>
#include <QSharedPointer>
#include <QDate>

class LIBKFACEBOOK_EXPORT UserInfo : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString id WRITE setId READ id)
  Q_PROPERTY(QString name WRITE setName READ name)
  Q_PROPERTY(QString first_name WRITE setFirstName READ firstName)
  Q_PROPERTY(QString last_name WRITE setLastName READ lastName)
  Q_PROPERTY(QString birthday WRITE setBirthday READ birthdayAsString)
  Q_PROPERTY(QString website WRITE setWebsite READ website)
  Q_PROPERTY(int timezone WRITE setTimezone READ timezone)
  Q_PROPERTY(QString updated_time WRITE setUpdatedTimeString READ updatedTimeString)
  public:
    UserInfo();
    void setId( const QString &id );
    QString id() const;
    void setName( const QString &name );
    QString name() const;
    void setFirstName( const QString &firstName );
    QString firstName() const;
    void setLastName( const QString &lastName );
    QString lastName() const;
    void setBirthday( const QString &birthday );
    QString birthdayAsString() const;
    QDate birthday() const;
    void setWebsite( const QString &website );
    QString website() const;
    void setCity( const QString &city );
    void setCountry( const QString &country );
    void setCompany( const QString &company );
    QString company() const;
    void setProfession( const QString &profession );
    QString profession() const;
    void setPartner( const QString &partner );
    QString partner() const;
    void setTimezone(int timezone);
    int timezone() const;
    void setUpdatedTimeString(const QString &updatedTime);
    QString updatedTimeString() const;
    KDateTime updatedTime() const;

    KABC::Addressee toAddressee() const;

  private:
    QString mId;
    QString mName;
    QString mFirstName;
    QString mLastName;
    QDate mBirthday;
    QString mWebsite;
    QString mCountry;
    QString mCity;
    QString mCompany;
    QString mProfession;
    QString mPartner;
    QString mUpdatedTime;
    int mTimezone;
};

typedef QSharedPointer<UserInfo> UserInfoPtr;
Q_DECLARE_METATYPE(UserInfoPtr)

#endif

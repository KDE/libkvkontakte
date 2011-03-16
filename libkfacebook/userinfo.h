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

/**
* Class that describes a person on facebook
*/
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

    /**
    * @breif Sets the facebook id of a person
    * @param id The Facebook id
    */
    void setId( const QString &id );
    /**
    * @return The facebook id of this person
    */
    QString id() const;


    /**
    * @brief Set the name of this person.
    * @param name The name of this person.
    */
    void setName( const QString &name );
    /**
    * @return The name this person.
    */
    QString name() const;

    /**
    * @brief Set the first name of this person.
    * @param firstName The first name of this person.
    */
    void setFirstName( const QString &firstName );
    /**
    * @return The first name of this person.
    */
    QString firstName() const;

    /**
    * @brief Set the last name of this person.
    * @param lastName The last name of this person.
    */
    void setLastName( const QString &lastName );
    /**
    * @return The last name of this person.
    */
    QString lastName() const;

    /**
    * @brief Set the birthday of this person.
    * @param birthday The birthday in "facebook format"
    */
    void setBirthday( const QString &birthday );
    /**
    * @return The birthday as a QString in "facebook format"
    */
    QString birthdayAsString() const;
    /**
    * @return The birthday of this person as a QDate.
    */
    QDate birthday() const;

    /**
     * @brief Set the website of this person.
     * @param website The url to the website of this user.
     */
    void setWebsite( const QString &website );
    /**
    * @return The url to the website of this user as a QString.
    */
    QString website() const;

    /**
    * @brief Set the current city of this person.
    * @param city The current city of this person.
    */
    void setCity( const QString &city );

    /**
    * @brief Set the current country of this person.
    * @param country The current country of this person.
    */
    void setCountry( const QString &country );

    /**
    * @brief Set the current company this person is working at.
    * @param company The company this person is working at.
    */
    void setCompany( const QString &company );

    /**
    * @return The company this person is working at.
    */
    QString company() const;

    /**
    * @brief Set the profession of this person.
    * @param profession The profession of this person.
    */
    void setProfession( const QString &profession );
    /**
    * @return The profession of this person.
    */
    QString profession() const;

    /**
    * @brief Set the partner of this person.
    * @param partner The partner of this person.
    */
    void setPartner( const QString &partner );
    /**
    * @return The partner of this person.
    */
    QString partner() const;

    /**
    * @brief Set the timezone of this person.
    * @param timezone The timezone of this person.
    */
    void setTimezone(int timezone);
    /**
    * @return The timezone fo this person.
    */
    int timezone() const;

    /**
    * @brief Set the time this person last updated his/her profile.
    * @param updatedTime The time of the last update in "facebook format"
    */
    void setUpdatedTimeString(const QString &updatedTime);
    /**
    * @return The time of the last update of the profile as a QString
    *         in "facebook format"
    */
    QString updatedTimeString() const;
    /**
    * @return The time of the last update of the profile as a KDateTime
    */
    KDateTime updatedTime() const;

    /**
    * @brief Created a KABC::Addressee for all the information we have about
    *        this person.
    *
    * @return A KABC::Addressee of this person. 
    */
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

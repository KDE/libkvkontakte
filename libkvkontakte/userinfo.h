/* Copyright 2010, 2011 Thomas McGuire <mcguire@kde.org>
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
#ifndef USERINFO_H
#define USERINFO_H

#include "libkvkontakte_export.h"

#include <KDateTime>
#include <QSharedPointer>
#include <QDate>

/**
* Class that describes a person on facebook
*/
class LIBKVKONTAKTE_EXPORT UserInfo : public QObject
{
    Q_OBJECT

    // Some fields are commented out, because nobody uses them.

    // uid, first_name, last_name, nickname, sex, bdate (birthdate), city, country,
    // timezone, photo, photo_medium, photo_big, photo_rec.
    Q_PROPERTY(int uid WRITE setUid READ uid)
    Q_PROPERTY(QString domain WRITE setDomain READ domain)
    Q_PROPERTY(QString first_name WRITE setFirstName READ firstName)
    Q_PROPERTY(QString last_name WRITE setLastName READ lastName)
    Q_PROPERTY(QString nickname WRITE setNickName READ nickName)
//    Q_PROPERTY(QString sex WRITE setSex READ sex)
    Q_PROPERTY(QString bdate WRITE setBirthday READ birthdayAsString)
    Q_PROPERTY(int city WRITE setCity READ city)
    Q_PROPERTY(int country WRITE setCountry READ country)
    Q_PROPERTY(int timezone WRITE setTimezone READ timezone)
    Q_PROPERTY(QString photo WRITE setPhoto READ photo)
    Q_PROPERTY(QString photo_medium WRITE setPhotoMedium READ photoMedium)
//     Q_PROPERTY(QString photo_big WRITE setPhotoBig READ photoBig)
//     Q_PROPERTY(QString photo_rec WRITE setPhotoRec READ photoRec)
//     Q_PROPERTY(int online WRITE setOnline READ online)

    // If "contacts" was requested, then these fields will
    // be also available: home_phone, mobile_phone.
    Q_PROPERTY(QString home_phone WRITE setHomePhone READ homePhone)
    Q_PROPERTY(QString mobile_phone WRITE setMobilePhone READ mobilePhone)
    //Q_PROPERTY(int has_mobile WRITE setHasMobile READ hasMobile)

//     // If "education" was requested, then these fields will
//     // be also available: university, university_name, faculty, faculty_name, graduation.
//     Q_PROPERTY(QString university WRITE setUniversity READ university)
//     Q_PROPERTY(QString university_name WRITE setUniversityName READ universityName)
//     Q_PROPERTY(QString faculty WRITE setFaculty READ faculty)
//     Q_PROPERTY(QString faculty_name WRITE setFacultyName READ facultyName)
//     Q_PROPERTY(QString graduation WRITE setGraduation READ graduation)

public:
    enum
    {
        INVALID_TIMEZONE = 42
    };

    UserInfo();

    /**
    * @brief Sets the facebook id of a person
    * @param id The Vkontakte id
    */
    void setUid(int uid);
    /**
    * @return The vkontakte id of this person
    */
    int uid() const;

    void setDomain(const QString &domain);
    QString domain() const;

    /**
    * @brief Set the first name of this person.
    * @param firstName The first name of this person.
    */
    void setFirstName(const QString &firstName);
    /**
    * @return The first name of this person.
    */
    QString firstName() const;

    /**
    * @brief Set the last name of this person.
    * @param lastName The last name of this person.
    */
    void setLastName(const QString &lastName);
    /**
    * @return The last name of this person.
    */
    QString lastName() const;

    /**
    * @brief Set the nickname of this person.
    * @param nickName The nickname of this person.
    */
    void setNickName( const QString &nickName );
    /**
    * @return The nickname of this person.
    */
    QString nickName() const;

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
    * @brief Set the current city of this person.
    * @param city The current city of this person.
    */
    void setCity(int city);

    int city() const;

    /**
    * @brief Set the current country of this person.
    * @param country The current country of this person.
    */
    void setCountry(int country);

    int country() const;

    void setCountryString(const QString &countryString);
    QString countryString() const;

    void setCityString(const QString &cityString);
    QString cityString() const;

    /**
    * @brief Set the timezone of this person.
    * @param timezone The timezone of this person.
    */
    void setTimezone(int timezone);
    /**
    * @return The timezone of this person.
    */
    int timezone() const;

    /**
     * @brief Set the URL of the profile photo
     *
     * @param photo URL of the profile photo
     **/
    void setPhoto(const QString &photo);

    /**
     * @return The URL of this user's photo
     **/
    QString photo() const;

    /**
     * @brief Set the URL of the medium-sized profile photo
     *
     * @param photoMedium URL of the medium-sized profile photo
     **/
    void setPhotoMedium(const QString &photoMedium);

    /**
     * @return The URL of this user's medium-sized photo
     **/
    QString photoMedium() const;

    void setHomePhone(const QString &homePhone);
    QString homePhone() const;

    void setMobilePhone(const QString &mobilePhone);
    QString mobilePhone() const;

    QString profileUrl() const;

    static QStringList allQueryFields();

private:
    int m_uid; // this could be an integer, but what if something will change?..
    QString m_domain; // plain string
    QString m_firstName; // plain string
    QString m_lastName; // plain string
    QString m_nickName; // plain string

    QDate m_birthday;
    int m_country;
    QString m_countryString;
    int m_city;
    QString m_cityString;
    int m_timezone;

    QString m_photo;
    QString m_photoMedium;

    QString m_homePhone;
    QString m_mobilePhone;
};

typedef QSharedPointer<UserInfo> UserInfoPtr;
Q_DECLARE_METATYPE(UserInfoPtr)

#endif // USERINFO_H

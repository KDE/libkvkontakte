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
#include "userinfo.h"

#include "util.h"

#include <KDebug>
#include <QtCore/QStringList>

namespace Vkontakte
{

UserInfo::UserInfo()
    : m_uid(-1), m_sex(-1), m_online(false)
    , m_city(-1), m_country(-1), m_hasMobile(false), m_rate(-1)
    , m_university(-1), m_faculty(-1), m_graduation(-1)
    , m_canPost(false), m_canWritePrivateMessage(false)
    , m_timezone(INVALID_TIMEZONE)
    , d(0)
{
}

void UserInfo::setUid(int uid)
{
    m_uid = uid;
}

int UserInfo::uid() const
{
    return m_uid;
}

void UserInfo::setFirstName(const QString &firstName)
{
    m_firstName = firstName;
}

QString UserInfo::firstName() const
{
    return m_firstName;
}

void UserInfo::setLastName(const QString &lastName)
{
    m_lastName = lastName;
}

QString UserInfo::lastName() const
{
    return m_lastName;
}

void UserInfo::setNickName(const QString& nickName)
{
    m_nickName = nickName;
}

QString UserInfo::nickName() const
{
    return m_nickName;
}

void UserInfo::setSex(int sex)
{
    m_sex = sex;
}

int UserInfo::sex() const
{
    return m_sex;
}

void UserInfo::setOnline(bool online)
{
    m_online = online;
}

bool UserInfo::online() const
{
    return m_online;
}

// "[day].[month].[year]" or "[day].[month]"
void UserInfo::setBirthday(const QString& birthday)
{
    m_birthday = QDate::fromString( birthday, "dd.MM.yyyy" );
    if ( !m_birthday.isValid() ) {
        // Some users don't tell the year of their birthday.
        m_birthday = QDate::fromString( birthday + ".0001", "dd.MM.yyyy" );
    }
}

QString UserInfo::birthdayAsString() const
{
    return m_birthday.toString();
}

QDate UserInfo::birthday() const
{
    return m_birthday;
}

void UserInfo::setCity(int city)
{
    m_city = city;
}

int UserInfo::city() const
{
    return m_city;
}

void UserInfo::setCityString(const QString &cityString)
{
    m_cityString = cityString;
}

QString UserInfo::cityString() const
{
    return m_cityString;
}

void UserInfo::setCountry(int country)
{
    m_country = country;
}

int UserInfo::country() const
{
    return m_country;
}

void UserInfo::setCountryString(const QString &countryString)
{
    m_countryString = countryString;
}

QString UserInfo::countryString() const
{
    return m_countryString;
}

void UserInfo::setPhoto(const QString &photo)
{
    m_photo = photo;
}

QString UserInfo::photo() const
{
    return m_photo;
}

void UserInfo::setPhotoMedium(const QString &photoMedium)
{
    m_photoMedium = photoMedium;
}

QString UserInfo::photoMedium() const
{
    return m_photoMedium;
}

void UserInfo::setPhotoMediumRec(const QString &photoMediumRec)
{
    m_photoMediumRec = photoMediumRec;
}

QString UserInfo::photoMediumRec() const
{
    return m_photoMediumRec;
}

void UserInfo::setPhotoBig(const QString &photoBig)
{
    m_photoBig = photoBig;
}

QString UserInfo::photoBig() const
{
    return m_photoBig;
}

void UserInfo::setPhotoRec(const QString &photoRec)
{
    m_photoRec = photoRec;
}

QString UserInfo::photoRec() const
{
    return m_photoRec;
}

void UserInfo::setDomain(const QString &domain)
{
    m_domain = domain;
}

QString UserInfo::domain() const
{
    return m_domain;
}

void UserInfo::setHasMobile(bool hasMobile)
{
    m_hasMobile = hasMobile;
}

bool UserInfo::hasMobile() const
{
    return m_hasMobile;
}

void UserInfo::setRate(int rate)
{
    m_rate = rate;
}

int UserInfo::rate() const
{
    return m_rate;
}

void UserInfo::setHomePhone(const QString &homePhone)
{
    m_homePhone = homePhone;
}

QString UserInfo::homePhone() const
{
    return m_homePhone;
}

void UserInfo::setMobilePhone(const QString &mobilePhone)
{
    m_mobilePhone = mobilePhone;
}

QString UserInfo::mobilePhone() const
{
    return m_mobilePhone;
}

void UserInfo::setUniversity(int university)
{
    m_university = university;
}

int UserInfo::university() const
{
    return m_university;
}

void UserInfo::setUniversityName(const QString &universityName)
{
    m_universityName = universityName;
}

QString UserInfo::universityName() const
{
    return m_universityName;
}

void UserInfo::setFaculty(int faculty)
{
    m_faculty = faculty;
}

int UserInfo::faculty() const
{
    return m_faculty;
}

void UserInfo::setFacultyName(const QString &facultyName)
{
    m_facultyName = facultyName;
}

QString UserInfo::facultyName() const
{
    return m_facultyName;
}

void UserInfo::setGraduation(int graduation)
{
    m_graduation = graduation;
}

int UserInfo::graduation() const
{
    return m_graduation;
}

void UserInfo::setCanPost(bool canPost)
{
    m_canPost = canPost;
}

bool UserInfo::canPost() const
{
    return m_canPost;
}

void UserInfo::setCanWritePrivateMessage(bool canWritePrivateMessage)
{
    m_canWritePrivateMessage = canWritePrivateMessage;
}

bool UserInfo::canWritePrivateMessage() const
{
    return m_canWritePrivateMessage;
}

void UserInfo::setCounters(const QVariantMap &counters)
{
    m_counters = counters;
}

QVariantMap UserInfo::counters() const
{
    return m_counters;
}

void UserInfo::setTimezone(int timezone)
{
    m_timezone = timezone;
}

int UserInfo::timezone() const
{
    return m_timezone;
}

QString UserInfo::profileUrl() const
{
    if (domain().isEmpty())
        return "http://vkontakte.ru/id" + uid();
    else
        return "http://vkontakte.ru/" + domain();
}

/**
 * @brief Returns the stringlist of all possible field groups
 * that may be requested in VKontakte API methods "getProfiles" and "friends.get"
 *
 * @return The list of strings to pass as "fields" argument to a method.
 **/
// static
QStringList UserInfo::allQueryFields()
{
    QStringList fields;
    fields << "uid"
           << "first_name"
           << "last_name"
           << "nickname"
           << "domain"
           << "sex"
           << "bdate"
           << "city"
           << "country"
           << "timezone"
           << "photo"
           << "photo_medium"
           << "photo_big"
           << "has_mobile"
           << "rate"
           << "contacts"
           << "education"
           << "online";
    return fields;
}

} /* namespace Vkontakte */

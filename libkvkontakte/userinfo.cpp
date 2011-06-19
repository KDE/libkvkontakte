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

static const int invalidTimezone = 42;

UserInfo::UserInfo()
    : m_timezone(invalidTimezone)
{
}

QString UserInfo::uid() const
{
    return m_uid;
}

void UserInfo::setUid(QString uid)
{
    m_uid = uid;
}

QString UserInfo::domain() const
{
    return m_domain;
}

void UserInfo::setDomain(QString domain)
{
    m_domain = domain;
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

QDate UserInfo::birthday() const
{
    return m_birthday;
}

QString UserInfo::birthdayAsString() const
{
    return m_birthday.toString();
}

QString UserInfo::firstName() const
{
    return m_firstName;
}

QString UserInfo::lastName() const
{
    return m_lastName;
}

QString UserInfo::nickName() const
{
    return m_nickName;
}

void UserInfo::setFirstName(const QString& firstName)
{
    m_firstName = firstName;
}

void UserInfo::setLastName(const QString& lastName)
{
    m_lastName = lastName;
}

void UserInfo::setNickName(const QString& nickName)
{
    m_nickName = nickName;
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

KABC::Addressee UserInfo::toAddressee() const
{
    KABC::Addressee addressee;
    addressee.setGivenName( firstName() );
    addressee.setUid( uid() );
    addressee.setFamilyName( lastName() );
    //addressee.setFormattedName( name() );
    addressee.setUrl( profileUrl() );
    addressee.setBirthday( QDateTime( birthday() ) );
    //addressee.setOrganization(mCompany);
    if (m_timezone != invalidTimezone) {
        addressee.setTimeZone(KABC::TimeZone(m_timezone));
    }
    //addressee.insertCustom("KADDRESSBOOK", "X-Profession", mProfession);
    //addressee.insertCustom("KADDRESSBOOK", "X-SpousesName", mPartner);
    if ( !countryString().isEmpty() || !cityString().isEmpty() ) {
        KABC::Address address(KABC::Address::Home);
        address.setRegion(countryString());
        address.setLocality(cityString());
        addressee.insertAddress(address);
    }
    return addressee;
}

/**
 * @brief Returns the stringlist of all possible field groups
 * that may be requested in VKontakte API methods "getProfiles" and "friends.get"
 *
 * @return The list of strings to pass as "fields" argument to a method.
 **/
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

QString UserInfo::photo() const
{
    return m_photo;
}

void UserInfo::setPhoto(const QString& photo)
{
    m_photo = photo;
}

QString UserInfo::photoMedium() const
{
    return m_photoMedium;
}

void UserInfo::setPhotoMedium(const QString& photoMedium)
{
    m_photoMedium = photoMedium;
}

QString UserInfo::city() const
{
    return m_city;
}

QString UserInfo::country() const
{
    return m_country;
}

void UserInfo::setCity(const QString &city)
{
    m_city = city;
}

void UserInfo::setCountry(const QString &country)
{
    m_country = country;
}

QString UserInfo::cityString() const
{
    return m_cityString;
}

QString UserInfo::countryString() const
{
    return m_countryString;
}

void UserInfo::setCityString(const QString& cityString)
{
    m_cityString = cityString;
}

void UserInfo::setCountryString(const QString& countryString)
{
    m_countryString = countryString;
}

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
#include "userinfo.h"

static const int invalidTimezone = 42;

UserInfo::UserInfo()
  : mTimezone(invalidTimezone)
{
}

QString UserInfo::name() const
{
  return mName;
}

QString UserInfo::id() const
{
  return mId;
}

void UserInfo::setName(const QString& name)
{
  mName = name;
}

void UserInfo::setId(const QString& id)
{
  mId = id;
}

QDate UserInfo::birthday() const
{
  return mBirthday;
}

QString UserInfo::birthdayAsString() const
{
  return mBirthday.toString();
}

QString UserInfo::firstName() const
{
  return mFirstName;
}

QString UserInfo::lastName() const
{
  return mLastName;
}

void UserInfo::setBirthday(const QString& birthday)
{
  mBirthday = QDate::fromString( birthday, "MM/dd/yyyy" );
  if ( !mBirthday.isValid() ) {
    // Some users don't tell the year of their birthday.
    mBirthday = QDate::fromString( birthday + "/0001", "MM/dd/yyyy" );
  }
}

void UserInfo::setFirstName(const QString& firstName)
{
  mFirstName = firstName;
}

void UserInfo::setLastName(const QString& lastName)
{
  mLastName = lastName;
}

void UserInfo::setWebsite(const QString& website)
{
  if ( website.contains( '\r' ) || website.contains( '\n' ) ) {
    QString normalized = website;
    normalized.replace( "\r\n", "\n" );
    normalized.replace( "\r", "\n" );
    const QStringList websites = normalized.split( '\n' );
    mWebsite = websites[0];
  } else {
    mWebsite = website;
  }
}

void UserInfo::setCity(const QString& city)
{
  mCity = city;
}

void UserInfo::setCountry(const QString& country)
{
  mCountry = country;
}


QString UserInfo::website() const
{
  return mWebsite;
}

QString UserInfo::company() const
{
  return mCompany;
}

QString UserInfo::profession() const
{
  return mProfession;
}

void UserInfo::setCompany(const QString& company)
{
  mCompany = company;
}

void UserInfo::setProfession(const QString& profession)
{
  mProfession = profession;
}

QString UserInfo::partner() const
{
  return mPartner;
}

void UserInfo::setPartner(const QString& partner)
{
  mPartner = partner;
}

void UserInfo::setTimezone(int timezone)
{
  mTimezone = timezone;
}

int UserInfo::timezone() const
{
  return mTimezone;
}

KABC::Addressee UserInfo::toAddressee() const
{
  KABC::Addressee addressee;
  addressee.setGivenName( firstName() );
  addressee.setFamilyName( lastName() );
  addressee.setFormattedName( name() );
  addressee.setUrl( website() );
  addressee.setBirthday( QDateTime( birthday() ) );
  addressee.setOrganization(mCompany);
  if (mTimezone != invalidTimezone) {
    addressee.setTimeZone(KABC::TimeZone(mTimezone));
  }
  addressee.insertCustom("KADDRESSBOOK", "X-Profession", mProfession);
  addressee.insertCustom("KADDRESSBOOK", "X-SpousesName", mPartner);
  if ( !mCity.isEmpty() || !mCountry.isEmpty() ) {
    KABC::Address address( KABC::Address::Home );
    address.setRegion( mCountry );
    address.setLocality( mCity );
    addressee.insertAddress( address );
  }
  return addressee;
}


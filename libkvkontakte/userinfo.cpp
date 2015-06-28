/*
 * Copyright (C) 2010, 2011  Thomas McGuire <mcguire@kde.org>
 * Copyright (C) 2011, 2015  Alexander Potashev <aspotashev@gmail.com>
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

#include "userinfo.h"
#include "util.h"

#include <KDebug>

#include <QtCore/QStringList>

namespace Vkontakte
{

class UserInfo::Private
{
public:
    int uid; // this could be an integer, but what if something will change?..
    QString firstName;
    QString lastName;

    QString nickName;

    int sex;

    bool online;

    QDate birthday;

    int city;
    QString cityString;

    int country;
    QString countryString;

    QString photo;
    QString photoMedium;
    QString photoMediumRec;
    QString photoBig;
    QString photoRec;

    QString domain; // domain / screen_name

    bool hasMobile;

    int rate;

    QString homePhone;
    QString mobilePhone;

    int university;
    QString universityName;
    int faculty;
    QString facultyName;
    int graduation; // graduation year

    bool canPost;
    bool canWritePrivateMessage;

    QVariantMap counters; // TODO: split into a few integers? Use "QMap<QString, int>"?

    int timezone;
};

UserInfo::UserInfo()
    : d(new Private)
{
    d->uid = -1;
    d->sex = -1;
    d->online = false;
    d->city = -1;
    d->country = -1;
    d->hasMobile = false;
    d->rate = -1;
    d->university = -1;
    d->faculty = -1;
    d->graduation = -1;
    d->canPost = false;
    d->canWritePrivateMessage = false;
    d->timezone = INVALID_TIMEZONE;
}

UserInfo::~UserInfo()
{
    delete d;
}

void UserInfo::setUid(int uid)
{
    d->uid = uid;
}

int UserInfo::uid() const
{
    return d->uid;
}

void UserInfo::setFirstName(const QString &firstName)
{
    d->firstName = firstName;
}

QString UserInfo::firstName() const
{
    return d->firstName;
}

void UserInfo::setLastName(const QString &lastName)
{
    d->lastName = lastName;
}

QString UserInfo::lastName() const
{
    return d->lastName;
}

void UserInfo::setNickName(const QString& nickName)
{
    d->nickName = nickName;
}

QString UserInfo::nickName() const
{
    return d->nickName;
}

void UserInfo::setSex(int sex)
{
    d->sex = sex;
}

int UserInfo::sex() const
{
    return d->sex;
}

void UserInfo::setOnline(bool online)
{
    d->online = online;
}

bool UserInfo::online() const
{
    return d->online;
}

// "[day].[month].[year]" or "[day].[month]"
void UserInfo::setBirthday(const QString &birthday)
{
    d->birthday = QDate::fromString(birthday, "dd.MM.yyyy");
    if (!d->birthday.isValid())
    {
        // Some users don't tell the year of their birthday.
        d->birthday = QDate::fromString(birthday + ".0001", "dd.MM.yyyy");
    }
}

QString UserInfo::birthdayAsString() const
{
    return d->birthday.toString();
}

QDate UserInfo::birthday() const
{
    return d->birthday;
}

void UserInfo::setCity(int city)
{
    d->city = city;
}

int UserInfo::city() const
{
    return d->city;
}

void UserInfo::setCityString(const QString &cityString)
{
    d->cityString = cityString;
}

QString UserInfo::cityString() const
{
    return d->cityString;
}

void UserInfo::setCountry(int country)
{
    d->country = country;
}

int UserInfo::country() const
{
    return d->country;
}

void UserInfo::setCountryString(const QString &countryString)
{
    d->countryString = countryString;
}

QString UserInfo::countryString() const
{
    return d->countryString;
}

void UserInfo::setPhoto(const QString &photo)
{
    d->photo = photo;
}

QString UserInfo::photo() const
{
    return d->photo;
}

void UserInfo::setPhotoMedium(const QString &photoMedium)
{
    d->photoMedium = photoMedium;
}

QString UserInfo::photoMedium() const
{
    return d->photoMedium;
}

void UserInfo::setPhotoMediumRec(const QString &photoMediumRec)
{
    d->photoMediumRec = photoMediumRec;
}

QString UserInfo::photoMediumRec() const
{
    return d->photoMediumRec;
}

void UserInfo::setPhotoBig(const QString &photoBig)
{
    d->photoBig = photoBig;
}

QString UserInfo::photoBig() const
{
    return d->photoBig;
}

void UserInfo::setPhotoRec(const QString &photoRec)
{
    d->photoRec = photoRec;
}

QString UserInfo::photoRec() const
{
    return d->photoRec;
}

void UserInfo::setDomain(const QString &domain)
{
    d->domain = domain;
}

QString UserInfo::domain() const
{
    return d->domain;
}

void UserInfo::setHasMobile(bool hasMobile)
{
    d->hasMobile = hasMobile;
}

bool UserInfo::hasMobile() const
{
    return d->hasMobile;
}

void UserInfo::setRate(int rate)
{
    d->rate = rate;
}

int UserInfo::rate() const
{
    return d->rate;
}

void UserInfo::setHomePhone(const QString &homePhone)
{
    d->homePhone = homePhone;
}

QString UserInfo::homePhone() const
{
    return d->homePhone;
}

void UserInfo::setMobilePhone(const QString &mobilePhone)
{
    d->mobilePhone = mobilePhone;
}

QString UserInfo::mobilePhone() const
{
    return d->mobilePhone;
}

void UserInfo::setUniversity(int university)
{
    d->university = university;
}

int UserInfo::university() const
{
    return d->university;
}

void UserInfo::setUniversityName(const QString &universityName)
{
    d->universityName = universityName;
}

QString UserInfo::universityName() const
{
    return d->universityName;
}

void UserInfo::setFaculty(int faculty)
{
    d->faculty = faculty;
}

int UserInfo::faculty() const
{
    return d->faculty;
}

void UserInfo::setFacultyName(const QString &facultyName)
{
    d->facultyName = facultyName;
}

QString UserInfo::facultyName() const
{
    return d->facultyName;
}

void UserInfo::setGraduation(int graduation)
{
    d->graduation = graduation;
}

int UserInfo::graduation() const
{
    return d->graduation;
}

void UserInfo::setCanPost(bool canPost)
{
    d->canPost = canPost;
}

bool UserInfo::canPost() const
{
    return d->canPost;
}

void UserInfo::setCanWritePrivateMessage(bool canWritePrivateMessage)
{
    d->canWritePrivateMessage = canWritePrivateMessage;
}

bool UserInfo::canWritePrivateMessage() const
{
    return d->canWritePrivateMessage;
}

void UserInfo::setCounters(const QVariantMap &counters)
{
    d->counters = counters;
}

QVariantMap UserInfo::counters() const
{
    return d->counters;
}

void UserInfo::setTimezone(int timezone)
{
    d->timezone = timezone;
}

int UserInfo::timezone() const
{
    return d->timezone;
}

QString UserInfo::profileUrl() const
{
    if (domain().isEmpty())
        return QString("http://vk.com/id") + QString::number(uid());
    else
        return QString("http://vk.com/") + domain();
}

// static
QStringList UserInfo::allQueryFields()
{
    QStringList fields;
    fields << "sex"
           << "bdate"
           << "city"
           << "country"
           << "photo_50"
           << "photo_100"
           << "photo_200_orig"
           << "photo_200"
           << "photo_400_orig"
           << "photo_max"
           << "photo_max_orig"
           << "photo_id"
           << "online"
           << "online_mobile"
           << "domain"
           << "has_mobile"
           << "contacts"
           << "connections"
           << "site"
           << "education"
           << "universities"
           << "schools"
           << "can_post"
           << "can_see_all_posts"
           << "can_see_audio"
           << "can_write_private_message"
           << "status"
           << "last_seen"
           << "common_count"
           << "relation"
           << "relatives"
           << "counters"
           << "screen_name"
           << "maiden_name"
           << "timezone"
           << "occupation"
           << "activities"
           << "interests"
           << "music"
           << "movies"
           << "tv"
           << "books"
           << "games"
           << "about"
           << "quotes"
           << "personal"
           << "friends_status";
    return fields;
}

} /* namespace Vkontakte */

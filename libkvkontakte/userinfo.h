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

#include <QtCore/QVariantMap>
#include <KDE/KDateTime>

#include <libkvkontakte/libkvkontakte_export.h>

namespace Vkontakte
{

// http://vkontakte.ru/developers.php?o=-1&p=getProfiles
class LIBKVKONTAKTE_EXPORT UserInfo : public QObject
{
    Q_OBJECT

    // These fields will be always returned: uid, first_name, last_name.
    Q_PROPERTY(int uid WRITE setUid READ uid)
    Q_PROPERTY(QString first_name WRITE setFirstName READ firstName)
    Q_PROPERTY(QString last_name WRITE setLastName READ lastName)

    // If "nickname" was requested, then this field will
    // be also available: nickname.
    Q_PROPERTY(QString nickname WRITE setNickName READ nickName)

    // If "sex" was requested, then this field will
    // be also available: sex.
    Q_PROPERTY(int sex WRITE setSex READ sex)

    // If "online" was requested, then this field will
    // be also available: online.
    Q_PROPERTY(bool online WRITE setOnline READ online)

    // If "bdate" was requested, then this field will
    // be also available: bdate.
    Q_PROPERTY(QString bdate WRITE setBirthday READ birthdayAsString)

    // If "city" was requested, then this field will
    // be also available: city.
    Q_PROPERTY(int city WRITE setCity READ city)

    // If "country" was requested, then this field will
    // be also available: country.
    Q_PROPERTY(int country WRITE setCountry READ country)

    // If "photo" was requested, then this field will
    // be also available: photo.
    Q_PROPERTY(QString photo WRITE setPhoto READ photo)

    // If "photo_medium" was requested, then this field will
    // be also available: photo_medium.
    Q_PROPERTY(QString photo_medium WRITE setPhotoMedium READ photoMedium)

    // If "photo_medium_rec" was requested, then this field will
    // be also available: photo_medium_rec.
    Q_PROPERTY(QString photo_medium_rec WRITE setPhotoMediumRec READ photoMediumRec)

    // If "photo_big" was requested, then this field will
    // be also available: photo_big.
    Q_PROPERTY(QString photo_big WRITE setPhotoBig READ photoBig)

    // If "photo_rec" was requested, then this field will
    // be also available: photo_rec.
    Q_PROPERTY(QString photo_rec WRITE setPhotoRec READ photoRec)

    // TODO: add "lists" field

    // If "screen_name" was requested, then this field will
    // be also available: screen_name.
    Q_PROPERTY(QString screen_name WRITE setDomain READ domain)
    // http://vkontakte.ru/developers.php?oid=-1&p=friends.get
    Q_PROPERTY(QString domain WRITE setDomain READ domain)

    // If "has_mobile" was requested, then this field will
    // be also available: has_mobile.
    Q_PROPERTY(bool has_mobile WRITE setHasMobile READ hasMobile)

    // If "rate" was requested, then this field will
    // be also available: rate.
    Q_PROPERTY(int rate WRITE setRate READ rate)

    // If "contacts" was requested, then these fields will
    // be also available: home_phone, mobile_phone.
    Q_PROPERTY(QString home_phone WRITE setHomePhone READ homePhone)
    Q_PROPERTY(QString mobile_phone WRITE setMobilePhone READ mobilePhone)

    // If "education" was requested, then these fields will
    // be also available: university, university_name, faculty, faculty_name, graduation.
    Q_PROPERTY(int university WRITE setUniversity READ university)
    Q_PROPERTY(QString university_name WRITE setUniversityName READ universityName)
    Q_PROPERTY(int faculty WRITE setFaculty READ faculty)
    Q_PROPERTY(QString faculty_name WRITE setFacultyName READ facultyName)
    Q_PROPERTY(int graduation WRITE setGraduation READ graduation)

    // If "can_post" was requested, then this field will
    // be also available: can_post.
    Q_PROPERTY(bool can_post WRITE setCanPost READ canPost)

    // If "can_write_private_message" was requested, then this field will
    // be also available: can_write_private_message.
    Q_PROPERTY(bool can_write_private_message WRITE setCanWritePrivateMessage READ canWritePrivateMessage)

    // If "counters" was requested, then this field will
    // be also available: counters.
    Q_PROPERTY(QVariantMap counters WRITE setCounters READ counters)

    // If "timezone" was requested, then this field will
    // be also available: timezone.
    Q_PROPERTY(int timezone WRITE setTimezone READ timezone)

public:
    enum
    {
        INVALID_TIMEZONE = 42
    };

    UserInfo();
    ~UserInfo();

    void setUid(int uid);
    int uid() const;

    void setFirstName(const QString &firstName);
    QString firstName() const;

    void setLastName(const QString &lastName);
    QString lastName() const;

    void setNickName(const QString &nickName);
    QString nickName() const;

    void setSex(int sex);
    int sex() const;

    void setOnline(bool online);
    bool online() const;

    void setBirthday(const QString &birthday);
    QString birthdayAsString() const;
    /**
    * @return The birthday of this person as a QDate.
    */
    QDate birthday() const;

    void setCity(int city);
    int city() const;
    void setCityString(const QString &cityString);
    QString cityString() const;

    void setCountry(int country);
    int country() const;
    void setCountryString(const QString &countryString);
    QString countryString() const;

    void setPhoto(const QString &photo);
    QString photo() const;

    void setPhotoMedium(const QString &photoMedium);
    QString photoMedium() const;

    void setPhotoMediumRec(const QString &photoMediumRec);
    QString photoMediumRec() const;

    void setPhotoBig(const QString &photoBig);
    QString photoBig() const;

    void setPhotoRec(const QString &photoRec);
    QString photoRec() const;

    void setDomain(const QString &domain);
    QString domain() const;

    void setHasMobile(bool hasMobile);
    bool hasMobile() const;

    void setRate(int rate);
    int rate() const;

    void setHomePhone(const QString &homePhone);
    QString homePhone() const;

    void setMobilePhone(const QString &mobilePhone);
    QString mobilePhone() const;

    void setUniversity(int university);
    int university() const;

    void setUniversityName(const QString &universityName);
    QString universityName() const;

    void setFaculty(int faculty);
    int faculty() const;

    void setFacultyName(const QString &facultyName);
    QString facultyName() const;

    void setGraduation(int graduation);
    int graduation() const;

    void setCanPost(bool canPost);
    bool canPost() const;

    void setCanWritePrivateMessage(bool canWritePrivateMessage);
    bool canWritePrivateMessage() const;

    void setCounters(const QVariantMap &counters);
    QVariantMap counters() const;

    void setTimezone(int timezone);
    int timezone() const;


    QString profileUrl() const;
    static QStringList allQueryFields();

private:
    class Private;
    Private * const d;
};

typedef QSharedPointer<UserInfo> UserInfoPtr;

} /* namespace Vkontakte */

Q_DECLARE_METATYPE(Vkontakte::UserInfoPtr)

#endif // USERINFO_H

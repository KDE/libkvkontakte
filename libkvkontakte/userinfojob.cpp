/* Copyright 2010 Thomas McGuire <mcguire@kde.org>
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
#include "userinfojob.h"

#include <qjson/qobjecthelper.h>
#include <qjson/parser.h>
#include <KIO/Job>
#include <KDebug>
#include <KLocale>

namespace Vkontakte
{

// http://vkontakte.ru/developers.php?o=-1&p=getProfiles
UserInfoJob::UserInfoJob (const QString &accessToken)
    : VkontakteJob(accessToken, "getProfiles")
{
    // The complete list of fields
    setFields(UserInfo::allQueryFields());

    // TODO: support "counters" request (probably in another KJob)
}

UserInfoJob::UserInfoJob(const QString &accessToken, int uid)
    : VkontakteJob(accessToken, "getProfiles")
{
    setFields(UserInfo::allQueryFields());
    addQueryItem("uids", QString::number(uid));
}

UserInfoJob::UserInfoJob(const QString &accessToken, const QIntList &uids)
    : VkontakteJob(accessToken, "getProfiles")
{
    setFields(UserInfo::allQueryFields());
    addQueryItem("uids", uids.join());

    // TODO: make this working for more than 1000 uids
    // ("getProfiles" allows requesting only 1000 users at once)
}

QList<UserInfoPtr> UserInfoJob::userInfo() const
{
    return m_userInfo;
}

void UserInfoJob::setFields(const QStringList &fields)
{
    m_fields = fields;
}

void UserInfoJob::prepareQueryItems()
{
    if (!m_fields.isEmpty())
        addQueryItem("fields", m_fields.join(","));
}

UserInfoPtr UserInfoJob::handleSingleData(const QVariant &data)
{
     UserInfoPtr userInfo = UserInfoPtr(new UserInfo());
     QJson::QObjectHelper::qvariant2qobject(data.toMap(), userInfo.data());
     return userInfo;
}

void UserInfoJob::handleData(const QVariant &data)
{
    foreach (const QVariant &item, data.toList())
        m_userInfo.append(handleSingleData(item));
}

} /* namespace Vkontakte */

#include "userinfojob.moc"

/*
 * Unit tests for libkvkontakte.
 * Copyright (C) 2015  Alexander Potashev <aspotashev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "test_userinfo.h"

#include <libkvkontakte/userinfofulljob.h>

#include <qtest_kde.h>

#include <QtCore/QList>

using namespace Vkontakte;

TestUserInfo::TestUserInfo()
    : VkTestBase()
{
}

void TestUserInfo::initTestCase()
{
    authenticate(AppPermissions::NoPermissions);
}

void TestUserInfo::testUserInfoJob()
{
    Vkontakte::UserInfoJob* const job = new Vkontakte::UserInfoJob(
        accessToken(), 1);
    job->exec();
    QVERIFY(!job->error());

    QList<UserInfoPtr> res = job->userInfo();
    QCOMPARE(res.size(), 1);

    const UserInfoPtr user = res.at(0);
    QCOMPARE(user->uid(), 1);
    QCOMPARE(user->firstName(), QString::fromUtf8("Павел"));
    QCOMPARE(user->lastName(), QString::fromUtf8("Дуров"));
    QCOMPARE(user->nickName(), QString::fromUtf8(""));
    QCOMPARE(user->sex(), 2);
    // TODO: verify that "online" status is received from server
    QCOMPARE(user->birthday(), QDate(1984, 10, 10));
    QCOMPARE(user->city(), 2); // Saint-Petersburg
    QCOMPARE(user->country(), 1); // Russia
    QCOMPARE(user->domain(), QString("durov"));
    QCOMPARE(user->hasMobile(), true);
    QCOMPARE(user->homePhone(), QString(""));
    QCOMPARE(user->mobilePhone(), QString(""));
    QCOMPARE(user->university(), 1); // SPbSU
    QCOMPARE(user->universityName(), QString::fromUtf8("СПбГУ")); // SPbSU
    QCOMPARE(user->faculty(), 0);
    QCOMPARE(user->facultyName(), QString(""));
    QCOMPARE(user->graduation(), 2006); // graduation year
    QCOMPARE(user->canPost(), false);
    QCOMPARE(user->canWritePrivateMessage(), false);
    // Timezone is returned only for the current user
    QCOMPARE(user->timezone(), static_cast<int>(UserInfo::INVALID_TIMEZONE));
}

void TestUserInfo::testSelfUserInfoJob()
{
    Vkontakte::UserInfoJob* const job = new Vkontakte::UserInfoJob(accessToken());
    job->exec();
    QVERIFY(!job->error());

    QList<UserInfoPtr> res = job->userInfo();
    QCOMPARE(res.size(), 1);

    const UserInfoPtr user = res.at(0);
    QVERIFY(user->uid() > 0);
    QVERIFY(!user->domain().isEmpty());
    // Timezone is returned only for the current user
    QVERIFY(user->timezone() != static_cast<int>(UserInfo::INVALID_TIMEZONE));
}

void TestUserInfo::testUserInfoFullJob()
{
    Vkontakte::UserInfoFullJob* const job = new Vkontakte::UserInfoFullJob(
        accessToken(), 1, true, true);
    job->exec();
    QVERIFY(!job->error());

    QList<UserInfoPtr> res = job->userInfo();
    QCOMPARE(res.size(), 1);

    const UserInfoPtr user = res.at(0);
    QCOMPARE(user->domain(), QString("durov"));
    QCOMPARE(user->firstName(), QString::fromUtf8("Павел"));
    QCOMPARE(user->countryString(), QString::fromUtf8("Россия"));
    QCOMPARE(user->cityString(), QString::fromUtf8("Санкт-Петербург"));
}

QTEST_KDEMAIN(TestUserInfo, GUI)

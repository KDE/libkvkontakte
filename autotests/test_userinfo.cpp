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
    authenticate();
}

void TestUserInfo::testUserInfoJob()
{
    Vkontakte::UserInfoJob* const job = new Vkontakte::UserInfoJob(
        accessToken(), 1);
    job->exec();
    QVERIFY(!job->error());

    QList<UserInfoPtr> res = job->userInfo();
    QCOMPARE(res.size(), 1);

    UserInfoPtr user = res.first();
    QCOMPARE(user->domain(), QString("durov"));
    QCOMPARE(user->firstName(), QString::fromUtf8("Павел"));
}

void TestUserInfo::testUserInfoFullJob()
{
    Vkontakte::UserInfoFullJob* const job = new Vkontakte::UserInfoFullJob(
        accessToken(), 1, true, true);
    job->exec();
    QVERIFY(!job->error());

    QList<UserInfoPtr> res = job->userInfo();
    QCOMPARE(res.size(), 1);

    UserInfoPtr user = res.first();
    QCOMPARE(user->domain(), QString("durov"));
    QCOMPARE(user->firstName(), QString::fromUtf8("Павел"));
    QCOMPARE(user->countryString(), QString::fromUtf8("Россия"));
    QCOMPARE(user->cityString(), QString::fromUtf8("Санкт-Петербург"));
}

QTEST_KDEMAIN(TestUserInfo, GUI)
#include "test_userinfo.moc"

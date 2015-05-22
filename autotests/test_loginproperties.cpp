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

#include "test_loginproperties.h"

#include <libkvkontakte/getapplicationpermissionsjob.h>
#include <libkvkontakte/getvariablejob.h>
#include <libkvkontakte/userinfojob.h>

#include <qtest_kde.h>

using namespace Vkontakte;

const AppPermissions::Value testPermissions =
    AppPermissions::Photos | AppPermissions::Offline |
    AppPermissions::Notes | AppPermissions::Messages;

TestLoginProperties::TestLoginProperties()
    : VkTestBase()
{
}

void TestLoginProperties::initTestCase()
{
    authenticate(testPermissions);
}

void TestLoginProperties::testGetApplicationPermissionsJob()
{
    GetApplicationPermissionsJob* const job = new GetApplicationPermissionsJob(accessToken());
    job->exec();
    QVERIFY(!job->error());

    QCOMPARE(job->permissions(), static_cast<int>(testPermissions));
}

void TestLoginProperties::testGetVariableJob()
{
    // Retrieve user info with UserInfoJob
    Vkontakte::UserInfoJob* const job = new Vkontakte::UserInfoJob(accessToken());
    job->exec();
    QVERIFY(!job->error());

    QList<UserInfoPtr> res = job->userInfo();
    QCOMPARE(res.size(), 1);

    const UserInfoPtr user = res.at(0);

    // Test GetVariableJob
    foreach (int index, QList<int>() << 1280 << 1281) {
        GetVariableJob* const job = new GetVariableJob(accessToken(), index);
        job->exec();
        QVERIFY(!job->error());

        int type = static_cast<int>(job->variable().type());
        if (index == 1280) {
            QCOMPARE(type, static_cast<int>(QMetaType::ULongLong));
            QCOMPARE(job->variable().toInt(), user->uid());
        } else { // 1281
            QCOMPARE(type, static_cast<int>(QMetaType::QString));
            QCOMPARE(job->variable().toString(), QString("%1 %2").arg(user->firstName()).arg(user->lastName()));
        }
    }
}

QTEST_KDEMAIN(TestLoginProperties, GUI)

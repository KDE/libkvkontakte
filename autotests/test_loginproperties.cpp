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

#include <qtest_kde.h>

using namespace Vkontakte;

TestLoginProperties::TestLoginProperties()
    : VkTestBase()
{
}

void TestLoginProperties::initTestCase()
{
    authenticate();
}

void TestLoginProperties::testGetApplicationPermissionsJob()
{
    GetApplicationPermissionsJob* const job = new GetApplicationPermissionsJob(accessToken());
    job->exec();
    QVERIFY(!job->error());

    QCOMPARE(job->permissions(), 71684);
}

void TestLoginProperties::testGetVariableJob()
{
    foreach (int index, QList<int>() << 1280 << 1281) {
        GetVariableJob* const job = new GetVariableJob(accessToken(), index);
        job->exec();
        QVERIFY(!job->error());

        int type = static_cast<int>(job->variable().type());
        if (index == 1280) {
            QCOMPARE(type, static_cast<int>(QMetaType::ULongLong));
        } else { // 1281
            QCOMPARE(type, static_cast<int>(QMetaType::QString));
        }
    }
}

QTEST_KDEMAIN(TestLoginProperties, GUI)
#include "test_loginproperties.moc"

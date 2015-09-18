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

#include "test_frequentrequests.h"

#include "userinfojob.h"

#include <QtTest/QtTest>

using namespace Vkontakte;

TestFrequentRequests::TestFrequentRequests()
    : VkTestBase()
{
}

void TestFrequentRequests::initTestCase()
{
    authenticate(Vkontakte::AppPermissions::NoPermissions);
}

void TestFrequentRequests::testUserInfoJob()
{
    // Send 20 requests without delays
    for (int i = 0; i < 20; ++i)
    {
        Vkontakte::UserInfoJob* const job = new Vkontakte::UserInfoJob(
            accessToken(), 1);
        job->exec();
        QVERIFY(!job->error());
    }
}

QTEST_MAIN(TestFrequentRequests)

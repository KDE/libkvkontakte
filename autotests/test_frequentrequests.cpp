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

#include <libkvkontakte/userinfofulljob.h>

#include <qtest_kde.h>

#include <QtCore/QList>

using namespace Vkontakte;

TestFrequentRequests::TestFrequentRequests()
    : VkTestBase()
{
}

void TestFrequentRequests::initTestCase()
{
    authenticate(Vkontakte::AppPermissions::NoPermissions);
}

void TestFrequentRequests::testUserInfoFullJob()
{
    // Send 20 requests without delays
    for (int i = 0; i < 20; ++i)
    {
        Vkontakte::UserInfoFullJob* const job = new Vkontakte::UserInfoFullJob(
            accessToken(), 1, true, true);
        job->exec();
        QVERIFY(!job->error());
    }
}

QTEST_KDEMAIN(TestFrequentRequests, GUI)

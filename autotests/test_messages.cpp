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

#include "test_messages.h"

#include <libkvkontakte/messageslistjob.h>
#include <libkvkontakte/allmessageslistjob.h>
#include <libkvkontakte/discussionslistjob.h>

#include <qtest_kde.h>

using namespace Vkontakte;

TestMessages::TestMessages()
    : VkTestBase()
{
}

void TestMessages::initTestCase()
{
    authenticate(AppPermissions::Messages);
}

void TestMessages::testMessagesListJob()
{
    MessagesListJob* const job = new MessagesListJob(accessToken());
    job->exec();
    QVERIFY(!job->error());
}

void TestMessages::testAllMessagesListJob()
{
    AllMessagesListJob* const job = new AllMessagesListJob(accessToken());
    job->exec();
    QVERIFY(!job->error());
}

void TestMessages::testDiscussionsListJob()
{
    DiscussionsListJob* const job = new DiscussionsListJob(accessToken());
    job->exec();
    QVERIFY(!job->error());
}

QTEST_KDEMAIN(TestMessages, GUI)

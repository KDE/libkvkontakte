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
#include "vkapi.h"

#include <libkvkontakte/userinfofulljob.h>

#include <qtest_kde.h>

#include <QtCore/QList>
#include <QtGui/QMainWindow>

#define VK_APP_ID   "2446321"

using namespace Vkontakte;

TestUserInfo::TestUserInfo()
    : m_vkapi(0)
{
}

void TestUserInfo::initTestCase()
{
    QMainWindow *parentWidget = new QMainWindow();

    m_vkapi = new KIPIVkontaktePlugin::VkAPI(parentWidget);
    m_vkapi->setAppId(VK_APP_ID); // TODO: library should better not crash if setAppId is not called
    m_vkapi->startAuthentication(false);

    // Wait for authentication
    QEventLoop loop;
    // TODO: Wait for any outcome of the authentication process, including failure
    connect(m_vkapi, SIGNAL(authenticated()), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY(m_vkapi->isAuthenticated());
}

void TestUserInfo::testUserInfoJob()
{
    Vkontakte::UserInfoJob* const job = new Vkontakte::UserInfoJob(
        m_vkapi->accessToken(), 1);

    job->exec();

    QList<UserInfoPtr> res = job->userInfo();
    QCOMPARE(res.size(), 1);

    UserInfoPtr user = res.first();
    QCOMPARE(user->domain(), QString("durov"));
    QCOMPARE(user->firstName(), QString::fromUtf8("Павел"));
}

void TestUserInfo::testUserInfoFullJob()
{
    Vkontakte::UserInfoFullJob* const job = new Vkontakte::UserInfoFullJob(
        m_vkapi->accessToken(), 1, true, true);

    job->exec();

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

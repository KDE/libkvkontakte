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

#include "vktestbase.h"

#include <libkvkontakte/vkapi.h>

#include <QtCore/QEventLoop>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtTest/QTest>

#define VK_APP_ID   "2446321"

VkTestBase::VkTestBase()
    : m_vkapi(new Vkontakte::VkApi(0))
{
}

VkTestBase::~VkTestBase()
{
    if (m_vkapi)
    {
        delete m_vkapi;
    }
}

QString VkTestBase::getSavedToken() const
{
    QFile file(AUTOTESTS_API_TOKEN_PATH);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return QString();
    }

    QTextStream in(&file);
    QString line = in.readLine();
    return line.trimmed();
}

void VkTestBase::authenticate(Vkontakte::AppPermissions::Value permissions)
{
    m_vkapi->setAppId(VK_APP_ID); // TODO: library should better not crash if setAppId is not called
    m_vkapi->setRequiredPermissions(permissions);

    QString token = getSavedToken();
    if (!token.isEmpty())
    {
        m_vkapi->setInitialAccessToken(token);
    }

    m_vkapi->startAuthentication(false);

    // Wait for any outcome of the authentication process, including failure
    QEventLoop loop;
    connect(m_vkapi, SIGNAL(authenticated()), &loop, SLOT(quit()));
    connect(m_vkapi, SIGNAL(canceled()), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY(m_vkapi->isAuthenticated());
}

QString VkTestBase::accessToken() const
{
    return m_vkapi->accessToken();
}

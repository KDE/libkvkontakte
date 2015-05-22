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

#include "test_database.h"

#include <libkvkontakte/cidsnamesjob.h>

#include <qtest_kde.h>

using namespace Vkontakte;

TestDatabaseMethods::TestDatabaseMethods()
    : VkTestBase()
{
}

void TestDatabaseMethods::initTestCase()
{
}

void TestDatabaseMethods::testCountriesById()
{
    CidsNamesJob* const job = new CidsNamesJob(
        "database.getCountriesById", QString(), QIntList() << 1 << 2 << 3);
    job->exec();
    QVERIFY(!job->error());

    QMap<int, QString> names = job->names();
    QCOMPARE(names.size(), 3);
    QCOMPARE(names[1], QString::fromUtf8("Россия")); // Russia
    QCOMPARE(names[2], QString::fromUtf8("Украина")); // Ukraine
    QCOMPARE(names[3], QString::fromUtf8("Беларусь")); // Belarus
}

void TestDatabaseMethods::testCitiesById()
{
    CidsNamesJob* const job = new CidsNamesJob(
        "database.getCitiesById", QString(), QIntList() << 1 << 2 << 4);
    job->exec();
    QVERIFY(!job->error());

    QMap<int, QString> names = job->names();
    QCOMPARE(names.size(), 3);
    QCOMPARE(names[1], QString::fromUtf8("Москва")); // Moscow
    QCOMPARE(names[2], QString::fromUtf8("Санкт-Петербург")); // Saint-Petersburg
    QCOMPARE(names[4], QString::fromUtf8("Herzliya"));
}

QTEST_KDEMAIN(TestDatabaseMethods, GUI)

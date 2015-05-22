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

#include "test_albums.h"

#include <libkvkontakte/createalbumjob.h>
#include <libkvkontakte/albumlistjob.h>
#include <libkvkontakte/editalbumjob.h>
#include <libkvkontakte/deletealbumjob.h>

#include <qtest_kde.h>

#define ALBUM1_NAME     "__album for unit testing of libkvkontakte #1"
#define ALBUM2_NAME     "__album for unit testing of libkvkontakte #2"

using namespace Vkontakte;

TestAlbums::TestAlbums()
    : VkTestBase()
{
}

void TestAlbums::initTestCase()
{
    authenticate(Vkontakte::AppPermissions::Photos);

    // Create albums for testing
    QList<QString> albumNames;
    albumNames << ALBUM1_NAME;
    albumNames << ALBUM2_NAME;

    foreach (const QString &name, albumNames) {
        CreateAlbumJob* const job = new CreateAlbumJob(
            accessToken(), name, QString("Description for %1").arg(name),
            AlbumInfo::PRIVACY_PRIVATE, AlbumInfo::PRIVACY_PRIVATE);
        job->exec();
        QVERIFY(!job->error());

        m_albumIds.append(job->album()->aid());
    }
}

void TestAlbums::testListJob()
{
    AlbumListJob* const job = new AlbumListJob(accessToken());
    job->exec();
    QVERIFY(!job->error());

    QList<AlbumInfoPtr> list = job->list();

    // We created two albums but could remove only one in testDeleteJob(),
    // so at least one album should exist at the moment.
    QVERIFY(list.size() >= 1);
}

void TestAlbums::testEditJob()
{
    const int albumId = m_albumIds[0];
    const QString newTitle("what's up?");
    const QString newDescription("my description");

    // Change album title, description and permissions
    {
        EditAlbumJob* const job = new EditAlbumJob(
            accessToken(), albumId, newTitle, newDescription,
            AlbumInfo::PRIVACY_PUBLIC, AlbumInfo::PRIVACY_FRIENDS);
        job->exec();
        QVERIFY(!job->error());
    }

    // Verify that album properties have changed
    AlbumListJob* const listJob = new AlbumListJob(accessToken(), -1, QIntList() << albumId);
    listJob->exec();
    QVERIFY(!listJob->error());

    QList<AlbumInfoPtr> list = listJob->list();
    QCOMPARE(list.size(), 1);

    const AlbumInfoPtr album = list.at(0);
    QCOMPARE(album->aid(), albumId);
    QCOMPARE(album->title(), newTitle);
    QCOMPARE(album->description(), newDescription);
    QCOMPARE(album->privacy(), static_cast<int>(AlbumInfo::PRIVACY_PUBLIC));

    // Privacy mode for comments is not returned by the VK server,
    // so it is not our fault.
//     QCOMPARE(album->commentPrivacy(), static_cast<int>(AlbumInfo::PRIVACY_FRIENDS));
}

void TestAlbums::testDeleteJob()
{
    const int albumId = m_albumIds[1];

    // Delete album
    {
        DeleteAlbumJob* const job = new DeleteAlbumJob(accessToken(), albumId);
        job->exec();
        QVERIFY(!job->error());
    }

    // Verify that the album does not exist anymore
    AlbumListJob* const listJob = new AlbumListJob(accessToken(), -1, QIntList() << albumId);
    listJob->exec();
    QVERIFY(!listJob->error());

    QList<AlbumInfoPtr> list = listJob->list();
    QCOMPARE(list.size(), 0);
}

QTEST_KDEMAIN(TestAlbums, GUI)

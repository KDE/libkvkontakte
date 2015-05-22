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

#include "test_photos.h"

#include <libkvkontakte/uploadphotosjob.h>
#include <libkvkontakte/photolistjob.h>
#include <libkvkontakte/photojob.h>
#include <libkvkontakte/createalbumjob.h>

#include <qtest_kde.h>

#include <QtGui/QImage>

#define ALBUM1_NAME     "__album for unit testing of libkvkontakte #1"

using namespace Vkontakte;

TestPhotos::TestPhotos()
    : VkTestBase()
    , m_albumId(-1)
    , m_photoId(-1)
{
}

void TestPhotos::initTestCase()
{
    authenticate(AppPermissions::Photos);

    // Create album for testing
    CreateAlbumJob* const albumJob = new CreateAlbumJob(
        accessToken(), ALBUM1_NAME, QString("Description for %1").arg(ALBUM1_NAME),
        AlbumInfo::PRIVACY_PRIVATE, AlbumInfo::PRIVACY_PRIVATE);
    albumJob->exec();
    QVERIFY(!albumJob->error());

    m_albumId = albumJob->album()->aid();

    // Upload photo for testing
    UploadPhotosJob* const job = new UploadPhotosJob(
        accessToken(),
        QStringList() << QString(AUTOTESTS_DATA_DIR) + "/image1.png",
        true, m_albumId);
    job->exec();
    QVERIFY(!job->error());

    QList<PhotoInfoPtr> list = job->list();
    QCOMPARE(list.size(), 1);

    const PhotoInfoPtr photo = list.at(0);
    m_photoId = photo->pid();
}

void TestPhotos::testPhotoListJob()
{
    PhotoListJob* const job = new PhotoListJob(
        accessToken(), 0, m_albumId, QIntList() << m_photoId);
    job->exec();
    QVERIFY(!job->error());

    QList<PhotoInfoPtr> list = job->list();
    QCOMPARE(list.size(), 1);

    const PhotoInfoPtr photo = list.at(0);
    QCOMPARE(photo->pid(), m_photoId);

    // Download the image
    PhotoJob* const photoJob = new PhotoJob(photo->src());
    photoJob->exec();
    QVERIFY(!photoJob->error());

    QImage image = photoJob->photo();
    QCOMPARE(image.size(), QSize(10, 10));
}

void TestPhotos::testUploadMultiple()
{
    // Upload multiple photos at once
    const int count = 10;

    QStringList files;
    for (int i = 0; i < count; ++i)
        files << QString(AUTOTESTS_DATA_DIR) + "/image1.png";

    UploadPhotosJob* const job = new UploadPhotosJob(
        accessToken(), files, true, m_albumId);
    job->exec();
    QVERIFY(!job->error());

    QList<PhotoInfoPtr> list = job->list();
    QCOMPARE(list.size(), count);
}

QTEST_KDEMAIN(TestPhotos, GUI)

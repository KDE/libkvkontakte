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

#ifndef TEST_ALBUMS_H
#define TEST_ALBUMS_H

#include <QtCore/QObject>
#include <QtCore/QVector>

namespace KIPIVkontaktePlugin {
    class VkAPI;
}

/*
 * What is tested here:
 *   class AlbumListJob
 *   class CreateAlbumJob - tested in initTestCase()
 *   class EditAlbumJob
 *   class DeleteAlbumJob
 */
class TestAlbums : public QObject
{
    Q_OBJECT

public:
    TestAlbums();

private Q_SLOTS:
    void initTestCase();

    void testListJob();
    void testEditJob();
    void testDeleteJob();

private:
    KIPIVkontaktePlugin::VkAPI *m_vkapi;
    QVector<int> m_albumIds;
};

#endif // TEST_ALBUMS_H

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

#ifndef TEST_NOTES_H
#define TEST_NOTES_H

#include "vktestbase.h"

#include <QtCore/QObject>
#include <QtCore/QVector>

/*
 * What is tested here:
 *   class NotesListJob
 *   class AllNotesListJob
 *   class NoteAddJob - tested in initTestCase()
 *   class NoteJob
 */
class TestNotes : public VkTestBase
{
    Q_OBJECT

public:
    TestNotes();

private Q_SLOTS:
    void initTestCase();

    void testNotesListJob();
    void testAllNotesListJob();
    void testNoteJob();

private:
    QVector<int> m_noteIds;
};

#endif // TEST_NOTES_H

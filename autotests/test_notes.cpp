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

#include "test_notes.h"
#include "vkapi.h"

#include <libkvkontakte/noteaddjob.h>
#include <libkvkontakte/noteslistjob.h>
#include <libkvkontakte/allnoteslistjob.h>
#include <libkvkontakte/notejob.h>

#include <qtest_kde.h>

#include <QtCore/QList>

#define VK_APP_ID   "2446321"

#define NOTE1_NAME     "__note for unit testing of libkvkontakte #1"
#define NOTE2_NAME     "__note for unit testing of libkvkontakte #2"

using namespace Vkontakte;

TestNotes::TestNotes()
    : m_vkapi(0)
{
}

void TestNotes::initTestCase()
{
    m_vkapi = new KIPIVkontaktePlugin::VkAPI(0);
    m_vkapi->setAppId(VK_APP_ID); // TODO: library should better not crash if setAppId is not called
    m_vkapi->startAuthentication(false);

    // Wait for authentication
    QEventLoop loop;
    // TODO: Wait for any outcome of the authentication process, including failure
    connect(m_vkapi, SIGNAL(authenticated()), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY(m_vkapi->isAuthenticated());

    // Create notes for testing
    QList<QString> noteNames;
    noteNames << NOTE1_NAME;
    noteNames << NOTE2_NAME;

    foreach (const QString &name, noteNames) {
        NoteAddJob* const job = new NoteAddJob(
            m_vkapi->accessToken(), name, QString("Text for %1").arg(name));
        job->exec();
        QVERIFY(!job->error());

        m_noteIds.append(job->nid());
    }
}

void TestNotes::testNotesListJob()
{
    NotesListJob* const job = new NotesListJob(m_vkapi->accessToken(), 0, 0, 100);
    job->exec();
    QVERIFY(!job->error());

    QList<NoteInfoPtr> list = job->list();

    // We created two notes and did not remove them,
    // so at least two notes should exist at the moment.
    QVERIFY(list.size() >= 2);
}

void TestNotes::testAllNotesListJob()
{
    AllNotesListJob* const job = new AllNotesListJob(m_vkapi->accessToken(), 0);
    job->exec();
    QVERIFY(!job->error());

    QList<NoteInfoPtr> list = job->list();

    // We created two notes and did not remove them,
    // so at least two notes should exist at the moment.
    QVERIFY(list.size() >= 2);
}

void TestNotes::testNoteJob()
{
    int noteId = m_noteIds[0];

    NoteJob* const job = new NoteJob(m_vkapi->accessToken(), noteId);
    job->exec();
    QVERIFY(!job->error());

    NoteInfoPtr note = job->noteInfo();
    QCOMPARE(note->title(), QString(NOTE1_NAME));
}

QTEST_KDEMAIN(TestNotes, GUI)
#include "test_notes.moc"

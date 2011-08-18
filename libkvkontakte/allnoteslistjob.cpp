/* Copyright 2011 Alexander Potashev <aspotashev@gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "allnoteslistjob.h"
#include <KDebug>
#include <KLocalizedString>

namespace Vkontakte
{

AllNotesListJob::AllNotesListJob(const QString &accessToken, int uid)
    : KJobWithSubjobs()
    , m_accessToken(accessToken)
    , m_uid(uid)
    , d(0)
{
    m_totalCount = -1;
}

void AllNotesListJob::startNewJob(int offset, int count)
{
    NotesListJob *job = new NotesListJob(m_accessToken, m_uid, offset, count);
    connect(job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)));
    m_jobs.append(job);
    job->start();
}

void AllNotesListJob::start()
{
    startNewJob(0, 100);
}

void AllNotesListJob::jobFinished(KJob *kjob)
{
    NotesListJob *job = dynamic_cast<NotesListJob *>(kjob);
    Q_ASSERT(job);
    m_jobs.removeAll(job);
    if (job->error()) {
        setError(job->error());
        setErrorText(job->errorText());
        kWarning() << "Job error: " << job->errorString();
        return;
    }

    m_list.append(job->list());

    // If this was the first job, start all others
    if (m_totalCount == -1) {
        m_totalCount = job->totalCount();
        for (int offset = 100; offset < m_totalCount; offset += 100)
            startNewJob(offset, qMin(100, m_totalCount - offset));
    }
    else if (m_totalCount != job->totalCount())
    {
        // TODO: some new notes might have been added, what should we do then?
        doKill();
        setError(KJob::UserDefinedError + 1);
        setErrorText(i18n("The number of notes has changed between requests."));
        kWarning() << "Job error: " << job->errorString();
        emitResult();
        return;
    }

    // All jobs have finished
    if (m_jobs.size() == 0) {
//        qSort(m_list); // sort by message ID (which should be equivalent to sorting by date)
        emitResult();
    }
}

QList<NoteInfoPtr> AllNotesListJob::list() const
{
    return m_list;
}

int AllNotesListJob::count() const
{
    return m_list.size();
}

} /* namespace Vkontakte */

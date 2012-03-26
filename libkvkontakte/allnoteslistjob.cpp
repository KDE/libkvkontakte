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
#include "allnoteslistjob.moc"

#include <KDebug>
#include <KLocalizedString>

namespace Vkontakte
{

class AllNotesListJob::Private
{
public:
    QString accessToken;
    int uid;

    int totalCount;
    QList<NoteInfoPtr> list;
};

AllNotesListJob::AllNotesListJob(const QString &accessToken, int uid)
    : KJobWithSubjobs()
    , d(new Private)
{
    d->accessToken = accessToken;
    d->uid = uid;
    d->totalCount = -1;
}

AllNotesListJob::~AllNotesListJob()
{
    delete d;
}

void AllNotesListJob::startNewJob(int offset, int count)
{
    NotesListJob *job = new NotesListJob(d->accessToken, d->uid, offset, count);
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

    d->list.append(job->list());

    // If this was the first job, start all others
    if (d->totalCount == -1) {
        d->totalCount = job->totalCount();
        for (int offset = 100; offset < d->totalCount; offset += 100)
            startNewJob(offset, qMin(100, d->totalCount - offset));
    }
    else if (d->totalCount != job->totalCount())
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
//        qSort(list); // sort by message ID (which should be equivalent to sorting by date)
        emitResult();
    }
}

QList<NoteInfoPtr> AllNotesListJob::list() const
{
    return d->list;
}

int AllNotesListJob::count() const
{
    return d->list.size();
}

} /* namespace Vkontakte */

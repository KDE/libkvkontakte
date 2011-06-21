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
#include "allmessageslistjob.h"
#include <QPointer>
#include "messageslistjob.h"

#include <KDebug>

AllMessagesListJob::AllMessagesListJob(const QString& accessToken,
                                       int out, int previewLength,
                                       int filters, int timeOffset)
    : KJob()
    , m_accessToken(accessToken)
    , m_out(out)
    , m_previewLength(previewLength)
    , m_filters(filters)
    , m_timeOffset(timeOffset)
    , m_totalCount(-1)
{
}

bool AllMessagesListJob::doKill()
{
    foreach (MessagesListJob *job, m_jobs) {
        job->kill(KJob::Quietly);
    }
    m_jobs.clear();

    return KJob::doKill();
}

void AllMessagesListJob::startNewJob(int offset, int count)
{
    MessagesListJob *job = new MessagesListJob(m_accessToken, m_out,
                                               offset, count,
                                               m_previewLength, m_filters, m_timeOffset);
    connect(job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)));
    m_jobs.append(job);
    job->start();
}

void AllMessagesListJob::start()
{
    // TODO: make m_out=-1 working
    startNewJob(0, 100);
}

void AllMessagesListJob::jobFinished(KJob* job)
{
    MessagesListJob *listJob = dynamic_cast<MessagesListJob *>(job);
    Q_ASSERT(listJob);
    m_jobs.removeAll(listJob);
    if (listJob->error()) {
        setError(listJob->error());
        setErrorText(listJob->errorText());
        kWarning() << "Job error: " << listJob->errorString();
        return;
    }

    m_list.append(listJob->list());
    if (m_totalCount == -1) { // this was the first job, starting all others
        m_totalCount = listJob->totalCount();
        for (int offset = 100; offset < m_totalCount; offset += 100) {
            startNewJob(offset, qMin(100, m_totalCount - offset));
        }
    }
    else {
        // TODO: some new messages might have been added, what should we do then?
        Q_ASSERT(m_totalCount == listJob->totalCount());
    }

    // All jobs have finished
    if (m_jobs.size() == 0)
        emitResult();
}

QList<MessageInfoPtr> AllMessagesListJob::list() const
{
    return m_list;
}

int AllMessagesListJob::count() const
{
    return m_list.size();
}

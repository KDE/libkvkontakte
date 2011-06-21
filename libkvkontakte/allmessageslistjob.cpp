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
    : KJobWithSubjobs()
    , m_accessToken(accessToken)
    , m_out(out)
    , m_previewLength(previewLength)
    , m_filters(filters)
    , m_timeOffset(timeOffset)
{
    m_totalCount[0] = -1; // for incoming messages
    m_totalCount[1] = -1; // for outgoing messages
}

void AllMessagesListJob::startNewJob(int offset, int count, int out)
{
    Q_ASSERT(out == 0 || out == 1);

    MessagesListJob *job = new MessagesListJob(m_accessToken, out,
                                               offset, count,
                                               m_previewLength, m_filters, m_timeOffset);
    connect(job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)));
    m_jobs.append(job);
    job->start();
}

void AllMessagesListJob::start()
{
    // m_out=-1 means to retrieve both incoming and outgoing messages
    if (m_out == 0 || m_out == -1) // incoming
        startNewJob(0, 100, 0);
    if (m_out == 1 || m_out == -1) // outgoing
        startNewJob(0, 100, 1);
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

    int out = listJob->out(); // incoming or outgoing
    Q_ASSERT(out == 0 || out == 1);
    // If this was the first job, start all others
    if (m_totalCount[out] == -1) {
        m_totalCount[out] = listJob->totalCount();
        for (int offset = 100; offset < m_totalCount[out]; offset += 100) {
            startNewJob(offset, qMin(100, m_totalCount[out] - offset), out);
        }
    }
    else {
        // TODO: some new messages might have been added, what should we do then?
        Q_ASSERT(m_totalCount[out] == listJob->totalCount());
    }

    // All jobs have finished
    if (m_jobs.size() == 0)
        qSort(m_list); // sort by message ID (which should be equvalent to sorting by date)
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

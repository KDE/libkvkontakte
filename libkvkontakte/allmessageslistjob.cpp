/*
 * Copyright (C) 2011  Alexander Potashev <aspotashev@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "allmessageslistjob.h"
#include "messageslistjob.h"

#include <KDebug>
#include <KLocalizedString>

namespace Vkontakte
{

class AllMessagesListJob::Private
{
public:
    QString accessToken;         /** Vkontakte Access token */
    int out;
    int previewLength;
    int filters;
    int timeOffset;

    int totalCount[2];
    QList<MessageInfoPtr> list;
};

AllMessagesListJob::AllMessagesListJob(const QString &accessToken,
                                       int out, int previewLength,
                                       int filters, int timeOffset)
    : KJobWithSubjobs()
    , d(new Private)
{
    d->accessToken = accessToken;
    d->out = out;
    d->previewLength = previewLength;
    d->filters = filters;
    d->timeOffset = timeOffset;
    d->totalCount[0] = -1; // for incoming messages
    d->totalCount[1] = -1; // for outgoing messages
}

AllMessagesListJob::~AllMessagesListJob()
{
    delete d;
}

void AllMessagesListJob::startNewJob(int offset, int count, int out)
{
    Q_ASSERT(out == 0 || out == 1);

    MessagesListJob *job = new MessagesListJob(d->accessToken, out,
                                               offset, count,
                                               d->previewLength, d->filters, d->timeOffset);
    connect(job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)));
    m_jobs.append(job);
    job->start();
}

void AllMessagesListJob::start()
{
    // out=-1 means to retrieve both incoming and outgoing messages
    if (d->out == 0 || d->out == -1) // incoming
        startNewJob(0, 100, 0);
    if (d->out == 1 || d->out == -1) // outgoing
        startNewJob(0, 100, 1);
}

void AllMessagesListJob::jobFinished(KJob *kjob)
{
    MessagesListJob *job = dynamic_cast<MessagesListJob *>(kjob);
    Q_ASSERT(job);

    if (!job) return;
    m_jobs.removeAll(job);

    if (job->error()) {
        setError(job->error());
        setErrorText(job->errorText());
        kWarning() << "Job error: " << job->errorString();
        return;
    }

    d->list.append(job->list());

    int out = job->out(); // incoming or outgoing
    Q_ASSERT(out == 0 || out == 1);
    // If this was the first job, start all others
    if (d->totalCount[out] == -1)
    {
        d->totalCount[out] = job->totalCount();
        for (int offset = 100; offset < d->totalCount[out]; offset += 100)
            startNewJob(offset, qMin(100, d->totalCount[out] - offset), out);
    }
    else if (d->totalCount[out] != job->totalCount())
    {
        // TODO: some new messages might have been added, what should we do then?
        doKill();
        setError(KJob::UserDefinedError + 2);
        if (out == 1)
        {
            setErrorText(i18n("The number of outgoing messages has changed between requests."));
        }
        else // out == 0
        {
            setErrorText(i18n("The number of incoming messages has changed between requests."));
        }

        kWarning() << "Job error: " << job->errorString();
        emitResult();
        return;
    }

    // All jobs have finished
    if (m_jobs.size() == 0)
    {
        qSort(d->list); // sort by message ID (which should be equivalent to sorting by date)
        emitResult();
    }
}

QList<MessageInfoPtr> AllMessagesListJob::list() const
{
    return d->list;
}

int AllMessagesListJob::count() const
{
    return d->list.size();
}

} /* namespace Vkontakte */

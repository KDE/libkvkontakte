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

    d->list.append(listJob->list());

    int out = listJob->out(); // incoming or outgoing
    Q_ASSERT(out == 0 || out == 1);
    // If this was the first job, start all others
    if (d->totalCount[out] == -1) {
        d->totalCount[out] = listJob->totalCount();
        for (int offset = 100; offset < d->totalCount[out]; offset += 100) {
            startNewJob(offset, qMin(100, d->totalCount[out] - offset), out);
        }
    }
    else if (d->totalCount[out] != listJob->totalCount())
    {
        // TODO: some new messages might have been added, what should we do then?
        doKill();
        setError(KJob::UserDefinedError + 2);
        setErrorText(i18nc("%1 can be \'incoming\' or \'outgoing\'",
                            "The number of %1 messages has changed between requests.",
                            out ? i18n("outgoing") : i18n("incoming")));
        kWarning() << "Job error: " << listJob->errorString();
        emitResult();
        return;
    }

    // All jobs have finished
    if (m_jobs.size() == 0) {
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

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
#include "photojob.h"
#include <KIO/Job>
#include <KDebug>

namespace Vkontakte
{

class PhotoJob::Private
{
public:
    KUrl url;
    QImage photo;
};

PhotoJob::PhotoJob(const KUrl &url)
    : d(new Private)
{
    d->url = url;
}

PhotoJob::~PhotoJob()
{
    delete d;
}

void PhotoJob::start()
{
    kDebug() << "Starting photo download" << d->url;
    KIO::StoredTransferJob * const job = KIO::storedGet(d->url, KIO::Reload, KIO::HideProgressInfo);
    m_job = job;
    connect(job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)));
    job->start();
}

void PhotoJob::jobFinished(KJob *kjob)
{
    KIO::StoredTransferJob *job = dynamic_cast<KIO::StoredTransferJob *>(kjob);
    Q_ASSERT(job);
    if (job->error())
    {
        setError(job->error());
        setErrorText(KIO::buildErrorString(error(), job->errorText()));
        kWarning() << "Job error: " << job->errorString();
    }
    else
        d->photo = QImage::fromData(job->data());

    emitResult();
    m_job = 0;
}

QImage PhotoJob::photo() const
{
    return d->photo;
}

} /* namespace Vkontakte */

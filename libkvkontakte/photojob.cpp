/*
 * Copyright (C) 2011, 2015  Alexander Potashev <aspotashev@gmail.com>
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

#include "photojob.h"

#include <KIO/Job>

#include <QtCore/QDebug>
#include <QtGui/QImage>

namespace Vkontakte
{

class PhotoJob::Private
{
public:
    QUrl url;
    QImage photo;
};

PhotoJob::PhotoJob(const QUrl &url)
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
    qDebug() << "Starting photo download" << d->url;
    KIO::StoredTransferJob * const job = KIO::storedGet(d->url, KIO::Reload, KIO::HideProgressInfo);
    m_job = job;
    connect(job, SIGNAL(result(KJob*)), this, SLOT(jobFinished(KJob*)));
    job->start();
}

void PhotoJob::jobFinished(KJob *kjob)
{
    KIO::StoredTransferJob *job = dynamic_cast<KIO::StoredTransferJob *>(kjob);
    Q_ASSERT(job);
    if (job && job->error())
    {
        setError(job->error());
        setErrorText(KIO::buildErrorString(error(), job->errorText()));
        qWarning() << "Job error:" << job->errorString();
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

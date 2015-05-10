/* Copyright 2011, 2012 Alexander Potashev <aspotashev@gmail.com>

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
#include "uploadphotosjob.moc"
#include "getphotouploadserverjob.h"
#include "photopostjob.h"
#include "savephotojob.h"

#include <KDebug>

namespace Vkontakte
{

const int UploadPhotosJob::MAX_POST_JOBS = 2;
const int UploadPhotosJob::REQUEST_FILES_COUNT = 1; // <= 5

class UploadPhotosJob::Private
{
public:
    QString accessToken;
    QStringList files;
    int aid;
    int gid;
    bool saveBig;

    UploadPhotosJob::Dest dest;

    QString uploadUrl;
    QList<PhotoInfoPtr> list;

    QList<PhotoPostJob *> pendingPostJobs;
    int workingPostJobs;
};

UploadPhotosJob::UploadPhotosJob(const QString &accessToken,
                                 const QStringList &files, bool saveBig, int aid, int gid)
    : KJobWithSubjobs()
    , d(new Private)
{
    d->accessToken = accessToken;
    d->files = files;
    d->aid = aid;
    d->gid = gid;
    d->saveBig = saveBig;
    d->workingPostJobs = 0;
    d->dest = Vkontakte::UploadPhotosJob::DEST_ALBUM;
}

UploadPhotosJob::~UploadPhotosJob()
{
    delete d;
}

int UploadPhotosJob::getMaxRequestFilesCount() const
{
    return REQUEST_FILES_COUNT;
}

void UploadPhotosJob::start()
{
    emit progress(0);

    GetPhotoUploadServerJob *job = new GetPhotoUploadServerJob(d->accessToken, d->dest);
    job->initUploadAlbum(d->aid, d->gid);

    m_jobs.append(job);
    connect(job, SIGNAL(result(KJob*)), this, SLOT(serverJobFinished(KJob*)));
    job->start();
}

void UploadPhotosJob::serverJobFinished(KJob *kjob)
{
    GetPhotoUploadServerJob *job = dynamic_cast<GetPhotoUploadServerJob *>(kjob);
    Q_ASSERT(job);

    if(!job) return;
    m_jobs.removeAll(job);

    if (job->error()) {
        setError(job->error());
        setErrorText(job->errorText());
        kWarning() << "Job error: " << job->errorString();

        // It is safe to emit result here because there are no jobs
        // running in parallel with this one.
        emitResult();
        return;
    }

    d->uploadUrl = job->uploadUrl();

    int totalCount = d->files.size();
    int requestFilesCount = getMaxRequestFilesCount();
    for (int offset = 0; offset < totalCount; offset += requestFilesCount)
        startPostJob(offset, qMin(requestFilesCount, totalCount - offset));

    // All subjobs have finished
    if (m_jobs.size() == 0)
        emitResult();
}

bool UploadPhotosJob::mayStartPostJob()
{
    return d->workingPostJobs < MAX_POST_JOBS;
}

void UploadPhotosJob::startPostJob(int offset, int count)
{
    PhotoPostJob *job = new PhotoPostJob(d->dest, d->uploadUrl, d->files.mid(offset, count));
    m_jobs.append(job);
    connect(job, SIGNAL(result(KJob*)), this, SLOT(postJobFinished(KJob*)));

    if (mayStartPostJob())
    {
        d->workingPostJobs ++;
        job->start();
    }
    else
        d->pendingPostJobs.append(job);
}

void UploadPhotosJob::postJobFinished(KJob *kjob)
{
    PhotoPostJob *job = dynamic_cast<PhotoPostJob *>(kjob);
    Q_ASSERT(job);

    if (!job) return;
    m_jobs.removeAll(job);

    d->workingPostJobs --;

    // start one pending job if possible
    if (mayStartPostJob() && !d->pendingPostJobs.empty())
    {
        PhotoPostJob *nextJob = d->pendingPostJobs.first();
        d->pendingPostJobs.removeAll(nextJob);

        d->workingPostJobs ++;
        nextJob->start();
    }

    if (job->error()) {
        setError(job->error());
        setErrorText(job->errorText());
        kWarning() << "Job error: " << job->errorString();
        emitResult();
        return;
    }

    startSaveJob(job->response());

    // All subjobs have finished
    if (m_jobs.size() == 0)
        emitResult();
}

void UploadPhotosJob::startSaveJob(const QVariantMap &photoIdData)
{
    SavePhotoJob *job = new SavePhotoJob(d->accessToken, d->dest, photoIdData);
    m_jobs.append(job);
    connect(job, SIGNAL(result(KJob*)), this, SLOT(saveJobFinished(KJob*)));
    job->start();
}

void UploadPhotosJob::saveJobFinished(KJob *kjob)
{
    // TODO: Try to preserve the original order of photos.
    // This task might be difficult when MAX_POST_JOBS > 1

    SavePhotoJob *job = dynamic_cast<SavePhotoJob *>(kjob);
    Q_ASSERT(job);

    if (!job) return;
    m_jobs.removeAll(job);

    if (job->error()) {
        setError(job->error());
        setErrorText(job->errorText());
        kWarning() << "Job error: " << job->errorString();
        emitResult();
        return;
    }

    d->list.append(job->list());
    emit progress(100 * d->list.size() / d->files.size());

    // All subjobs have finished
    if (m_jobs.size() == 0)
        emitResult();
}

QList<PhotoInfoPtr> UploadPhotosJob::list() const
{
    return d->list;
}

} /* namespace Vkontakte */

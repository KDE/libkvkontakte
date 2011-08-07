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
#include "uploadphotosjob.h"
#include "getphotouploadserverjob.h"
#include <KDebug>
#include "photopostjob.h"
#include "savephotojob.h"

namespace Vkontakte
{

UploadPhotosJob::UploadPhotosJob(const QString &accessToken,
                                 const QStringList &files, bool saveBig, int aid, int gid)
    : KJobWithSubjobs()
    , m_accessToken(accessToken)
    , m_files(files)
    , m_aid(aid)
    , m_gid(gid)
    , m_saveBig(saveBig)
{
}

void UploadPhotosJob::start()
{
    emit progress(0);

    GetPhotoUploadServerJob *job = new GetPhotoUploadServerJob(m_accessToken, m_saveBig, m_aid, m_gid);
    connect(job, SIGNAL(result(KJob*)), this, SLOT(serverJobFinished(KJob*)));
    m_jobs.append(job);
    job->start();
}

void UploadPhotosJob::serverJobFinished(KJob *kjob)
{
    GetPhotoUploadServerJob *job = dynamic_cast<GetPhotoUploadServerJob *>(kjob);
    Q_ASSERT(job);
    m_jobs.removeAll(job);
    if (job->error()) {
        setError(job->error());
        setErrorText(job->errorText());
        kWarning() << "Job error: " << job->errorString();
        emitResult();
        return;
    }

    m_uploadUrl = job->uploadUrl();

    int totalCount = m_files.size();
    for (int offset = 0; offset < totalCount; offset += 5)
        startPostJob(offset, qMin(5, totalCount - offset));

    // All subjobs have finished
    if (m_jobs.size() == 0)
        emitResult();
}

void UploadPhotosJob::startPostJob(int offset, int count)
{
    PhotoPostJob *job = new PhotoPostJob(m_uploadUrl, m_files.mid(offset, count));
    connect(job, SIGNAL(result(KJob*)), this, SLOT(postJobFinished(KJob*)));
    m_jobs.append(job);
    job->start();
}

void UploadPhotosJob::postJobFinished(KJob *kjob)
{
    PhotoPostJob *job = dynamic_cast<PhotoPostJob *>(kjob);
    Q_ASSERT(job);
    m_jobs.removeAll(job);
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

void UploadPhotosJob::startSaveJob(const QMap<QString, QVariant> &photoIdData)
{
    SavePhotoJob *job = new SavePhotoJob(m_accessToken, photoIdData);
    connect(job, SIGNAL(result(KJob*)), this, SLOT(saveJobFinished(KJob*)));
    m_jobs.append(job);
    job->start();
}

void UploadPhotosJob::saveJobFinished(KJob *kjob)
{
    SavePhotoJob *job = dynamic_cast<SavePhotoJob *>(kjob);
    Q_ASSERT(job);
    m_jobs.removeAll(job);
    if (job->error()) {
        setError(job->error());
        setErrorText(job->errorText());
        kWarning() << "Job error: " << job->errorString();
        emitResult();
        return;
    }

    m_list.append(job->list());
    emit progress(100 * m_list.size() / m_files.size());

    // All subjobs have finished
    if (m_jobs.size() == 0)
        emitResult();
}

QList<PhotoInfoPtr> UploadPhotosJob::list() const
{
    return m_list;
}

} /* namespace Vkontakte */

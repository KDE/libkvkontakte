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

#ifndef UPLOADPHOTOSJOB_H
#define UPLOADPHOTOSJOB_H

#include "vkontaktejobs.h"
#include "photoinfo.h"

#include <QtCore/QVariantMap>

namespace Vkontakte
{

class LIBKVKONTAKTE_EXPORT UploadPhotosJob : public KJobWithSubjobs
{
    Q_OBJECT
public:
    enum Dest {
        DEST_ALBUM = 1,
        DEST_PROFILE,
        DEST_WALL
    };

    UploadPhotosJob(const QString &accessToken,
                    const QStringList &files, bool saveBig, int aid, int gid = -1);
    ~UploadPhotosJob();

    virtual void start();

    QList<PhotoInfoPtr> list() const;

protected:
    static const int MAX_POST_JOBS;
    static const int REQUEST_FILES_COUNT;

    int getMaxRequestFilesCount() const;

    bool mayStartPostJob();

    void startPostJob(int offset, int count);
    void startSaveJob(const QVariantMap &photoIdData);

Q_SIGNALS:
    void progress(int);

private Q_SLOTS:
    void serverJobFinished(KJob *);
    void postJobFinished(KJob *);
    void saveJobFinished(KJob *);

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // UPLOADPHOTOSJOB_H

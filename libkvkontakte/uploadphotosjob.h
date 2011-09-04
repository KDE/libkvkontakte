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
#ifndef UPLOADPHOTOSJOB_H
#define UPLOADPHOTOSJOB_H

#include <QVariantMap>

#include <libkvkontakte/vkontaktejobs.h>
#include <libkvkontakte/photoinfo.h>

namespace Vkontakte
{

class LIBKVKONTAKTE_EXPORT UploadPhotosJob : public KJobWithSubjobs
{
    Q_OBJECT
public:
    UploadPhotosJob(const QString &accessToken,
                    const QStringList &files, bool saveBig, int aid, int gid = -1);
    ~UploadPhotosJob();

    virtual void start();

    QList<PhotoInfoPtr> list() const;

protected:
    static const int MAX_POST_JOBS;
    static const int REQUEST_FILES_COUNT;

    bool mayStartPostJob();

    void startPostJob(int offset, int count);
    void startSaveJob(const QVariantMap &photoIdData);

signals:
    void progress(int);

private slots:
    void serverJobFinished(KJob *);
    void postJobFinished(KJob *);
    void saveJobFinished(KJob *);

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // UPLOADPHOTOSJOB_H

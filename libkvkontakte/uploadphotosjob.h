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

#include "vkontaktejobs.h"
#include <QStringList>
#include <QMap>
#include <QVariant>
#include "photoinfo.h"

class LIBKVKONTAKTE_EXPORT UploadPhotosJob : public KJobWithSubjobs
{
    Q_OBJECT
public:
    UploadPhotosJob(const QString &accessToken,
                    const QStringList &files, int aid, int gid = -1);

    virtual void start();

    QList<PhotoInfoPtr> list() const;

protected:
    void startPostJob(int offset, int count);
    void startSaveJob(const QMap<QString, QVariant> &photoIdData);

private slots:
    void serverJobFinished(KJob *);
    void postJobFinished(KJob *);
    void saveJobFinished(KJob *);

private:
    QString m_accessToken;
    QStringList m_files;
    int m_aid;
    int m_gid;

    QString m_uploadUrl;
    QList<PhotoInfoPtr> m_list;
};

#endif // UPLOADPHOTOSJOB_H

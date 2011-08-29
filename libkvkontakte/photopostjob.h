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
#ifndef PHOTOPOSTJOB_H
#define PHOTOPOSTJOB_H

#include "libkvkontakte_export.h"
#include <KJob>
#include <QPointer>
#include <QStringList>
#include <QVariantMap>
#include "vkontaktejobs.h"

namespace Vkontakte
{

// This class is not exported, so:
//    - we don't care about ABI of this class (not using Pimpl)
//    - library's users should use class UploadPhotosJob
class PhotoPostJob : public KJobWithSubjob
{
    Q_OBJECT
public:
    // files.size <= 5
    PhotoPostJob(const QString &url, const QStringList &files);

    virtual void start();
    QVariantMap response() const;

protected:
    /** Check for a return error and set the appropiate error messages */
    void handleError(const QVariant &data);

    virtual void handleData(const QVariant &data);

private slots:
    void jobFinished(KJob *job);

private:
    QString m_url;
    QStringList m_files;
    bool m_ok;
    QVariantMap m_response;
};

} /* namespace Vkontakte */

#endif // PHOTOPOSTJOB_H

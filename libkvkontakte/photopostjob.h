/* Copyright 2011, 2015  Alexander Potashev <aspotashev@gmail.com>

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

#include <QtCore/QStringList>
#include <QtCore/QVariantMap>

#include "vkontaktejobs.h"
#include "uploadphotosjob.h"

class QHttpMultiPart;
class QNetworkReply;

namespace Vkontakte
{

// This class is not exported, so:
//    - we don't care about ABI of this class (not using Pimpl)
//    - library's users should use class UploadPhotosJob
class PhotoPostJob : public KJob
{
    Q_OBJECT
public:
    // files.size <= 5
    PhotoPostJob(Vkontakte::UploadPhotosJob::Dest dest, const QString &url, const QStringList &files);

    virtual void start();
    QVariantMap response() const;

protected:
    /** Check for a return error and set the appropriate error messages */
    void handleError(const QVariant &data);

    virtual void handleData(const QVariant &data);

private Q_SLOTS:
    void parseNetworkResponse(QNetworkReply *reply);

private:
    bool appendFile(QHttpMultiPart *multiPart, const QString &header, const QString &path);

    UploadPhotosJob::Dest m_dest;
    QString m_url;
    QStringList m_files;
    bool m_ok;
    QVariantMap m_response;
};

} /* namespace Vkontakte */

#endif // PHOTOPOSTJOB_H

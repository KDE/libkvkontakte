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
#ifndef SAVEPHOTOJOB_H
#define SAVEPHOTOJOB_H

#include <QMap>
#include <QVariant>
#include "vkontaktejobs.h"
#include "photoinfo.h"

class LIBKVKONTAKTE_EXPORT SavePhotoJob : public VkontakteJob
{
public:
    SavePhotoJob(const QString &accessToken,
                 const QMap<QString, QVariant> &photoIdData, int gid = -1);

    QList<PhotoInfoPtr> list() const;

protected:
    virtual void handleData(const QVariant &data);
    void handleItem(const QVariant &data);

private:
    QList<PhotoInfoPtr> m_list;
};

#endif // SAVEPHOTOJOB_H

/* Copyright 2010 Thomas McGuire <mcguire@kde.org>
   Copyright 2011 Alexander Potashev <aspotashev@gmail.com>

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
#ifndef GROUPLISTJOB_H
#define GROUPLISTJOB_H

#include "vkontaktejobs.h"
#include "groupinfo.h"

namespace Vkontakte
{

// http://vkontakte.ru/developers.php?oid=-1&p=groups.get
// TODO: for how many groups does this method work?
class LIBKVKONTAKTE_EXPORT GroupListJob : public VkontakteJob
{
    Q_OBJECT
public:
    // This class was never used in KDE 4, will be removed in the KF5 version
    Q_DECL_DEPRECATED explicit GroupListJob(const QString &accessToken, int uid = -1, bool extended = true);
    ~GroupListJob();

    QList<GroupInfoPtr> list() const;

protected:
    GroupInfoPtr handleSingleData(const QVariant &data);
    virtual void handleData(const QVariant &data);

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // GROUPLISTJOB_H

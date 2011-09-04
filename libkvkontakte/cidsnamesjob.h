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
#ifndef CIDSNAMESJOB_H
#define CIDSNAMESJOB_H

#include <QtCore/QMap>

#include <libkvkontakte/vkontaktejobs.h>
#include <libkvkontakte/qintlist.h>

namespace Vkontakte
{

class CidsNamesJob : public VkontakteJob
{
public:
    CidsNamesJob(const QString &method,
                 const QString &accessToken,
                 const QIntList &cids);
    ~CidsNamesJob();

    QMap<int, QString> names() const;

protected:
    virtual void handleData(const QVariant &data);

private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // CIDSNAMESJOB_H

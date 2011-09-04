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
#ifndef GETVARIABLEJOB_H
#define GETVARIABLEJOB_H

#include <QtCore/QVariant>

#include <libkvkontakte/vkontaktejobs.h>

namespace Vkontakte
{

// For the info about specific variables see:
// http://vkontakte.ru/developers.php?o=-1&p=%D5%F0%E0%ED%E5%ED%E8%E5%20%E4%E0%ED%ED%FB%F5%20%ED%E0%20%F1%E5%F0%E2%E5%F0%E0%F5%20%C2%CA%EE%ED%F2%E0%EA%F2%E5
class LIBKVKONTAKTE_EXPORT GetVariableJob : public VkontakteJob
{

public:
    GetVariableJob(const QString &accessToken, int index);
    ~GetVariableJob();

    QVariant variable() const;

protected:
    virtual void handleData(const QVariant &data);
    
private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // GETVARIABLEJOB_H

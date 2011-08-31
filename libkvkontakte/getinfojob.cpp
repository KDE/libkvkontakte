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
#include "getinfojob.h"

namespace Vkontakte
{

class GetInfoJob::Private
{
public:
    QVariantMap info;
};

// example 'method': "messages.getLongPollServer"
GetInfoJob::GetInfoJob(const QString &accessToken, const QString &method)
    : VkontakteJob(accessToken, method)
    , d(new Private)
{
}

GetInfoJob::~GetInfoJob()
{
    delete d;
}

void GetInfoJob::handleData(const QVariant &data)
{
     d->info = data.toMap();
}

QVariantMap GetInfoJob::info()
{
    return d->info;
}

} /* namespace Vkontakte */

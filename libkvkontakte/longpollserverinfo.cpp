/* Copyright 2010, 2011 Thomas McGuire <mcguire@kde.org>
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
#include "longpollserverinfo.h"

#include <KDebug>

namespace Vkontakte
{

LongPollServerInfo::LongPollServerInfo()
    : d(0)
{
}

void LongPollServerInfo::setKey(const QString& key)
{
    m_key = key;
}

QString LongPollServerInfo::key() const
{
    return m_key;
}

void LongPollServerInfo::setServer(const QString& server)
{
    m_server = server;
}

QString LongPollServerInfo::server() const
{
    return m_server;
}

void LongPollServerInfo::setTs(const QString& ts)
{
    m_ts = ts;
}

QString LongPollServerInfo::ts() const
{
    return m_ts;
}

} /* namespace Vkontakte */

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
#include "cidsnamesjob.h"
#include <QtCore/QMap>
#include <QVariant>

namespace Vkontakte
{

// http://vkontakte.ru/developers.php?o=-1&p=places.getCountryById
CidsNamesJob::CidsNamesJob(const QString &method,
                           const QString &accessToken,
                           const QIntList &cids)
    : VkontakteJob(accessToken, method)
{
    m_cids = cids;
    addQueryItem("cids", cids.join());
}

void CidsNamesJob::handleData(const QVariant &data)
{
    // We need QMap, because vkontakte.ru might reorder results
    // (I did not check it, but they do not guarantee the same order as in "cids")
    foreach (const QVariant &item, data.toList())
    {
        QMap<QString, QVariant> item_map = item.toMap();
        m_names[item_map["cid"].toInt()] = item_map["name"].toString();
    }
}

QMap<int, QString> CidsNamesJob::names() const
{
    return m_names;
}

} /* namespace Vkontakte */

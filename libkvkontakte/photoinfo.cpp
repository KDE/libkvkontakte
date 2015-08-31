/*
 * Copyright (C) 2011, 2015  Alexander Potashev <aspotashev@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "photoinfo.h"
#include "util.h"

namespace Vkontakte
{

class Q_DECL_HIDDEN PhotoInfo::Private : public QSharedData
{
public:
    QJsonObject jsonData;
};

PhotoInfo::PhotoInfo()
    : d(new Private)
{
}

PhotoInfo::PhotoInfo(const QJsonObject &jsonData)
    : d(new Private)
{
    d->jsonData = jsonData;
}

PhotoInfo::PhotoInfo(const PhotoInfo &other)
{
    d = other.d;
}

PhotoInfo::~PhotoInfo()
{
}

PhotoInfo &PhotoInfo::operator=(const PhotoInfo &other)
{
    if (this != &other)
    {
        d = other.d;
    }

    return *this;
}

int PhotoInfo::pid() const
{
    return d->jsonData.value(QStringLiteral("pid")).toInt(-1);
}

QUrl PhotoInfo::urlMaxResolution() const
{
    // Photo URLs in the order of decresing size
    QStringList srcKeys;
    srcKeys.append(QStringLiteral("src_xxxbig"));
    srcKeys.append(QStringLiteral("src_xxbig"));
    srcKeys.append(QStringLiteral("src_xbig"));
    srcKeys.append(QStringLiteral("src_big"));
    srcKeys.append(QStringLiteral("src"));
    srcKeys.append(QStringLiteral("src_small"));

    foreach (const QString &key, srcKeys)
    {
        if (!d->jsonData.contains(key))
        {
            continue;
        }

        QJsonValue value = d->jsonData.value(key);
        if (value.isString())
        {
            return QUrl(value.toString());
        }
    }

    return QUrl();
}

} /* namespace Vkontakte */

/*
 * Copyright (C) 2010  Thomas McGuire <mcguire@kde.org>
 * Copyright (C) 2011  Alexander Potashev <aspotashev@gmail.com>
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

#include "grouplistjob.h"

#include <qjson/qobjecthelper.h>

namespace Vkontakte
{

class GroupListJob::Private
{
public:
    QList<GroupInfoPtr> list;
    bool extended;
};

GroupListJob::GroupListJob(const QString &accessToken, int uid, bool extended)
    : VkontakteJob(accessToken, "groups.get")
    , d(new Private)
{
    d->extended = extended;

    if (uid != -1)
        addQueryItem("uid", QString::number(uid));
    if (extended)
        addQueryItem("extended", "1");
}

GroupListJob::~GroupListJob()
{
    delete d;
}

QList<GroupInfoPtr> GroupListJob::list() const
{
    return d->list;
}

GroupInfoPtr GroupListJob::handleSingleData(const QVariant &data)
{
     GroupInfoPtr info = GroupInfoPtr(new GroupInfo());
     QJson::QObjectHelper::qvariant2qobject(data.toMap(), info.data());
     return info;
}

void GroupListJob::handleData(const QVariant &data)
{
    if (d->extended)
    {
        QVariantList dataList = data.toList();
        dataList.pop_front(); // total count (unused)
        foreach(const QVariant &item, dataList)
            d->list.append(handleSingleData(item));
    }
    else
    {
        // TODO: test with both extended={true, false}

        foreach(const QVariant &item, data.toList())
        {
            GroupInfoPtr group = GroupInfoPtr(new GroupInfo());
            group->setGid(item.toInt());
            d->list.append(group);
        }
    }
}

} /* namespace Vkontakte */

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

#ifndef GETVARIABLEJOB_H
#define GETVARIABLEJOB_H

#include "vkontaktejobs.h"

#include <QtCore/QVariant>

namespace Vkontakte
{

// For the info about specific variables see:
// http://vkontakte.ru/developers.php?o=-1&p=%D5%F0%E0%ED%E5%ED%E8%E5%20%E4%E0%ED%ED%FB%F5%20%ED%E0%20%F1%E5%F0%E2%E5%F0%E0%F5%20%C2%CA%EE%ED%F2%E0%EA%F2%E5
class LIBKVKONTAKTE_EXPORT GetVariableJob : public VkontakteJob
{

public:
    // "getVariable" is deprecated, please use "users.get" with empty user_ids
    Q_DECL_DEPRECATED GetVariableJob(const QString &accessToken, int index);
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

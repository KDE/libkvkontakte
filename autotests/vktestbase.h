/*
 * Unit tests for libkvkontakte.
 * Copyright (C) 2015  Alexander Potashev <aspotashev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef VKTESTBASE_H
#define VKTESTBASE_H

#include <libkvkontakte/apppermissions.h>

#include <QtCore/QObject>
#include <QtCore/QVector>

namespace Vkontakte {
    class VkApi;
}

class VkTestBase : public QObject
{
    Q_OBJECT

public:
    VkTestBase();
    virtual ~VkTestBase();

protected:
    void authenticate(Vkontakte::AppPermissions::Value permissions);
    QString accessToken() const;

private:
    QString getSavedToken() const;

    Vkontakte::VkApi *m_vkapi;
};

#endif // VKTESTBASE_H

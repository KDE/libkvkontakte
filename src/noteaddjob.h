/*
 * Copyright (C) 2011  Roeland Jago Douma <unix@rullzer.com>
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

#ifndef NOTEADDJOB_H
#define NOTEADDJOB_H

#include "vkontaktejobs.h"
#include "noteinfo.h"

namespace Vkontakte
{

/**
* Add a note to vkontakte
*/
class LIBKVKONTAKTE_EXPORT NoteAddJob : public VkontakteJob
{
    Q_OBJECT
public:
    NoteAddJob(const QString &accessToken, const QString &title, const QString &text);
    ~NoteAddJob();

    int nid() const;

protected:
    void handleData(const QVariant &data) Q_DECL_OVERRIDE;
    
private:
    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // NOTEADDJOB_H

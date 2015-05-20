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

#ifndef LIBKVKONTAKTE_EXPORT_H
#define LIBKVKONTAKTE_EXPORT_H

/* needed for KDE_EXPORT and KDE_IMPORT macros */
#include <kdemacros.h>

#ifndef LIBKVKONTAKTE_EXPORT
# if defined(MAKE_KVKONTAKTE_LIB)
   /* We are building this library */
#  define LIBKVKONTAKTE_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define LIBKVKONTAKTE_EXPORT KDE_IMPORT
# endif
#endif

#endif // LIBKVKONTAKTE_EXPORT_H

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

#ifndef LIBKVKONTAKTE_VERSION_H
#define LIBKVKONTAKTE_VERSION_H

#include <kdeversion.h>

#define LIBKVKONTAKTE_MAJOR_VERSION @LIBKVKONTAKTE_VERSION_MAJOR@
#define LIBKVKONTAKTE_MINOR_VERSION @LIBKVKONTAKTE_VERSION_MINOR@
#define LIBKVKONTAKTE_PATCH_VERSION @LIBKVKONTAKTE_VERSION_PATCH@

#define LIBKVKONTAKTE_VERSION_STR "@LIBKVKONTAKTE_VERSION_MAJOR@.@LIBKVKONTAKTE_VERSION_MINOR@.@LIBKVKONTAKTE_VERSION_PATCH@"

#define LIBKVKONTAKTE_VERSION KDE_MAKE_VERSION(@LIBKVKONTAKTE_VERSION_MAJOR@, @LIBKVKONTAKTE_VERSION_MINOR@, @LIBKVKONTAKTE_VERSION_PATCH@)

// TODO: remove this line as obsolete
static const char libkvkontakte_version[] = KVKONTAKTE_VERSION_STR;

#endif // LIBKVKONTAKTE_VERSION_H

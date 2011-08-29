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
#include "photoinfo.h"

#include "util.h"

#include <KDebug>
#include <KLocalizedString>

namespace Vkontakte
{

class PhotoInfo::Private
{
public:
    int pid;
    int aid;
    int uid;
    QString src;
    QString srcSmall;
    QString srcBig;
    QString srcXBig;
    QString srcXXBig;
    QString text;
    QString dateCreated;
};

PhotoInfo::PhotoInfo()
    : d(new Private)
{
    d->pid = -1;
    d->aid = -1;
    d->uid = -1;
}

PhotoInfo::~PhotoInfo()
{
    delete d;
}

void PhotoInfo::setPid(int pid)
{
    d->pid = pid;
}

int PhotoInfo::pid() const
{
    return d->pid;
}

void PhotoInfo::setAid(int aid)
{
    d->aid = aid;
}

int PhotoInfo::aid() const
{
    return d->aid;
}

void PhotoInfo::setUid(int uid)
{
    d->uid = uid;
}

int PhotoInfo::uid() const
{
    return d->uid;
}

void PhotoInfo::setSrc(const QString &src)
{
    d->src = src;
}

QString PhotoInfo::src() const
{
    return d->src;
}

void PhotoInfo::setSrcSmall(const QString &srcSmall)
{
    d->srcSmall = srcSmall;
}

QString PhotoInfo::srcSmall() const
{
    return d->srcSmall;
}

void PhotoInfo::setSrcBig(const QString &srcBig)
{
    d->srcBig = srcBig;
}

QString PhotoInfo::srcBig() const
{
    return d->srcBig;
}

void PhotoInfo::setSrcXBig(const QString &srcXBig)
{
    d->srcXBig = srcXBig;
}

QString PhotoInfo::srcXBig() const
{
    return d->srcXBig;
}

void PhotoInfo::setSrcXXBig(const QString &srcXXBig)
{
    d->srcXXBig = srcXXBig;
}

QString PhotoInfo::srcXXBig() const
{
    return d->srcXXBig;
}

void PhotoInfo::setText(const QString &text)
{
    d->text = text;
}

QString PhotoInfo::text() const
{
    return d->text;
}

void PhotoInfo::setDateCreatedString(const QString &dateCreatedString)
{
    d->dateCreated = dateCreatedString;
}

QString PhotoInfo::dateCreatedString() const
{
    return d->dateCreated;
}

KDateTime PhotoInfo::dateCreated() const
{
    return unixTimeToKDateTime(d->dateCreated);
}

} /* namespace Vkontakte */

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

PhotoInfo::PhotoInfo()
    : m_pid(-1), m_aid(-1), m_uid(-1)
    , d(0)
{
}

void PhotoInfo::setPid(int pid)
{
    m_pid = pid;
}

int PhotoInfo::pid() const
{
    return m_pid;
}

void PhotoInfo::setAid(int aid)
{
    m_aid = aid;
}

int PhotoInfo::aid() const
{
    return m_aid;
}

void PhotoInfo::setUid(int uid)
{
    m_uid = uid;
}

int PhotoInfo::uid() const
{
    return m_uid;
}

void PhotoInfo::setSrc(const QString &src)
{
    m_src = src;
}

QString PhotoInfo::src() const
{
    return m_src;
}

void PhotoInfo::setSrcSmall(const QString &srcSmall)
{
    m_srcSmall = srcSmall;
}

QString PhotoInfo::srcSmall() const
{
    return m_srcSmall;
}

void PhotoInfo::setSrcBig(const QString &srcBig)
{
    m_srcBig = srcBig;
}

QString PhotoInfo::srcBig() const
{
    return m_srcBig;
}

void PhotoInfo::setSrcXBig(const QString &srcXBig)
{
    m_srcXBig = srcXBig;
}

QString PhotoInfo::srcXBig() const
{
    return m_srcXBig;
}

void PhotoInfo::setSrcXXBig(const QString &srcXXBig)
{
    m_srcXXBig = srcXXBig;
}

QString PhotoInfo::srcXXBig() const
{
    return m_srcXXBig;
}

void PhotoInfo::setText(const QString &text)
{
    m_text = text;
}

QString PhotoInfo::text() const
{
    return m_text;
}

void PhotoInfo::setDateCreatedString(const QString &dateCreatedString)
{
    m_dateCreated = dateCreatedString;
}

QString PhotoInfo::dateCreatedString() const
{
    return m_dateCreated;
}

KDateTime PhotoInfo::dateCreated() const
{
    return unixTimeToKDateTime(m_dateCreated);
}

} /* namespace Vkontakte */

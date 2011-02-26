/* Copyright 2011 Thomas McGuire <mcguire@kde.org>

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
#include "eventinfo.h"

#include "util.h"

KCalCore::Event EventInfo::asEvent() const
{
  return KCalCore::Event();
  // TODO
}

KDateTime EventInfo::endTime() const
{
  return facebookTimeToKDateTime(mEndTime);
}

QString EventInfo::endTimeString() const
{
  return mEndTime;
}

QString EventInfo::id() const
{
  return mId;
}

QString EventInfo::location() const
{
  return mLocation;
}

QString EventInfo::name() const
{
  return mName;
}

void EventInfo::setEndTimeString(const QString& endTime)
{
  mEndTime = endTime;
}

void EventInfo::setId(const QString& id)
{
  mId = id;
}

void EventInfo::setLocation(const QString& location)
{
  mLocation = location;
}

void EventInfo::setName(const QString& name)
{
  mName = name;
}

void EventInfo::setStartTimeString(const QString& startTime)
{
  mStartTime = startTime;
}

KDateTime EventInfo::startTime() const
{
  return facebookTimeToKDateTime(mStartTime);
}

QString EventInfo::startTimeString() const
{
  return mStartTime;
}

QString EventInfo::description() const
{
  return mDescription;
}

void EventInfo::setDescription( const QString& description )
{
  mDescription = description;
}

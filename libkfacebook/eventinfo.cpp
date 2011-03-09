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

#include <KDebug>
#include <KLocalizedString>
#include <KPIMUtils/LinkLocator>

EventPtr EventInfo::asEvent() const
{
  EventPtr event( new Event );
  QString desc = description();
  desc = KPIMUtils::LinkLocator::convertToHtml( desc, KPIMUtils::LinkLocator::ReplaceSmileys );
  if ( !desc.isEmpty() ) {
    desc += "<br><br>";
  }
  desc += "<a href=\"" + QString( "http://www.facebook.com/event.php?eid=%1" ).arg( id() ) +
          "\">" + i18n( "View Event on Facebook" ) + "</a>";

  event->setSummary( name() );
  event->setDescription( desc, true );
  event->setLocation( location() );
  event->setHasEndDate( endTime().isValid() );
  event->setOrganizer( organizer() );
  event->setUid( id() );
  if ( startTime().isValid() ) {
    event->setDtStart( startTime() );
  } else {
    kWarning() << "WTF, event has no start date??";
  }
  if ( endTime().isValid() ) {
    event->setDtEnd( endTime() );
  } else if ( startTime().isValid() && !endTime().isValid() ) {
    // Urgh...
    KDateTime endDate;
    endDate.setDate( startTime().date() );
    endDate.setTime( QTime::fromString( "23:59:00" ) );
    kWarning() << "Event without end time: " << event->summary() << event->dtStart();
    kWarning() << "Making it an event until the end of the day.";
    event->setDtEnd( endDate );
    //kWarning() << "Using a duration of 2 hours";
    //event->setDuration( KCalCore::Duration( 2 * 60 * 60, KCalCore::Duration::Seconds ) );
  }

  // TODO: Organizer
  //       Attendees (accepted, declined)
  //       Public/Private -> freebusy!
  //       venue: add to location?
  //       picture?

  return event;
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

QString EventInfo::organizer() const
{
  return mOrganizer;
}

void EventInfo::setOrganizer(const QString& organizer)
{
  mOrganizer = organizer;
}

void EventInfo::setUpdatedTimeString(const QString& updatedTime)
{
  mUpdatedTime = updatedTime;
}

KDateTime EventInfo::updatedTime() const
{
  return facebookTimeToKDateTime(mUpdatedTime);
}

QString EventInfo::updatedTimeString() const
{
  return mUpdatedTime;
}

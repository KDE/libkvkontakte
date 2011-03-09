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
#include <KCal/Attendee>

Attendee::Attendee(const QString &name, const QString &id, const QString &status)
  : mName(name), mId(id), mStatus(status)
{

}

QString Attendee::getName() const
{
  return mName;
}

QString Attendee::getId() const
{
  return mId;
}

QString Attendee::getStatus() const
{
  return mStatus;
}


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
  event->setLastModified( updatedTime() );
  event->setCreated( updatedTime() ); // That's a lie, but Facebook doesn't give us the created time
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
  //       Public/Private -> freebusy!
  //       venue: add to location?
  //       picture?
  
  const QList<Attendee const *> eventAttendees = attendees();

  QList<Attendee const *>::const_iterator i;
  for (i = eventAttendees.begin(); i != eventAttendees.end(); i++) {
    const Attendee *a = *i;

    KCal::Attendee::PartStat status = KCal::Attendee::NeedsAction;
    if (a->getStatus() == "attending") {
      status = KCal::Attendee::Accepted;
    } else if (a->getStatus() == "declined") {
      status = KCal::Attendee::Declined;
    } else if (a->getStatus() == "unsure") {
      status = KCal::Attendee::Tentative;
    }

    KCal::Attendee *b = new KCal::Attendee(a->getName(), 
                                           "facebook@unkown.invalid", 
                                           false, 
                                           status,
                                           KCal::Attendee::OptParticipant,
                                           a->getId() );
    event->addAttendee(b);
  }

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

void EventInfo::addAttendee(Attendee const *a )
{
  mAttendees << a;
}

QList<Attendee const *> EventInfo::attendees() const
{
  return mAttendees;
}

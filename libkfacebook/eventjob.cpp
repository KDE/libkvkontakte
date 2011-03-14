/* Copyright 2010, 2011 Thomas McGuire <mcguire@kde.org>

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
#include "eventjob.h"

#include <KDebug>
#include <qjson/qobjecthelper.h>

EventJob::EventJob( const QString& eventId, const QString& accessToken )
  : FacebookGetJob( '/' + eventId, accessToken),
    mMultiQuery( false )
{
  setFields( eventFields() );
}

EventJob::EventJob( const QStringList& eventIds, const QString& accessToken )
  : FacebookGetJob( accessToken ),
    mMultiQuery( true )
{
  setFields( eventFields() );
  setIds( eventIds );
}

QStringList EventJob::eventFields() const
{
  QStringList fields;
  fields << "owner"
         << "name"
         << "description"
         << "start_time"
         << "end_time"
         << "location"
         << "venue"
         << "privacy"
         << "updated_time"
         << "noreply"
         << "maybe"
         << "attending"
         << "declined";
  return fields;
}

QList<EventInfoPtr> EventJob::eventInfo() const
{
  return mEventInfo;
}

EventInfoPtr EventJob::handleSingleEvent( const QVariant& data )
{
  EventInfoPtr eventInfo( new EventInfo() );
  const QVariantMap dataMap = data.toMap();
  QJson::QObjectHelper::qvariant2qobject( dataMap, eventInfo.data() );
  const QVariant owner = dataMap.value( "owner" );
  if ( !owner.isNull() && owner.isValid() ) {
    eventInfo->setOrganizer( owner.toMap().value( "name" ).toString() );
  }

  /*
   * People that have not yet responded
   */
  const QVariantList noreply = dataMap.value("noreply").toMap().value("data").toList();
  foreach(const QVariant &attendee, noreply) {
    const QVariantMap map = attendee.toMap();
    AttendeePtr a( new Attendee(map["name"].toString(), map["id"].toString(), KCal::Attendee::NeedsAction) );
    eventInfo->addAttendee(a);
  }

  /*
   * People that maybe will attend
   */
  const QVariantList maybe = dataMap.value("maybe").toMap().value("data").toList();
  foreach(const QVariant &attendee, maybe) {
    const QVariantMap map = attendee.toMap();
    AttendeePtr a( new Attendee(map["name"].toString(), map["id"].toString(), KCal::Attendee::Tentative) );
    eventInfo->addAttendee(a);
  }

  /*
   * People that will attend
   */
  const QVariantList attending = dataMap.value("attending").toMap().value("data").toList();
  foreach(const QVariant &attendee, attending) {
    const QVariantMap map = attendee.toMap();
    AttendeePtr a( new Attendee(map["name"].toString(), map["id"].toString(), KCal::Attendee::Accepted) );
    eventInfo->addAttendee(a);
  }

  /*
   * People that have declined
   */
  const QVariantList declined = dataMap.value("declined").toMap().value("data").toList();
  foreach(const QVariant &attendee, declined) {
    const QVariantMap map = attendee.toMap();
    AttendeePtr a( new Attendee(map["name"].toString(), map["id"].toString(), KCal::Attendee::Declined) );
    eventInfo->addAttendee(a);
  }

  return eventInfo;
}

void EventJob::handleData( const QVariant& data )
{
  if ( !mMultiQuery ) {
    mEventInfo.append( handleSingleEvent( data ) );
  } else {
    foreach( const QVariant &event, data.toMap() ) {
      mEventInfo.append( handleSingleEvent( event ) );
    }
  }
}

#include "eventjob.moc"

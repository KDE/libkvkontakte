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
#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <config.h>
#include "libkfacebook_export.h"

#include <KDateTime>
#include <QObject>

#ifndef KDEPIM_44_COMPAT
#include <KCalCore/Event>
#include <KCalCore/Attendee>
typedef KCalCore::Event Event;
typedef KCalCore::Attendee Attendee;
typedef KCalCore::Event::Ptr EventPtr;
typedef KCalCore::Incidence::Ptr IncidencePtr;
typedef KCalCore::Attendee::Ptr AttendeePtr;
#else
#include <boost/shared_ptr.hpp>
#include <KCal/Event>
#include <KCal/Attendee>
typedef KCal::Event Event;
typedef KCal::Attendee Attendee;
typedef boost::shared_ptr<KCal::Event> EventPtr;
typedef boost::shared_ptr<KCal::Incidence> IncidencePtr;
typedef KCal::Attendee* AttendeePtr;
#endif

/**
* An attendee to an event
*/
class AttendeeInfo
{
  public:

    /**
    * @brief Construct an attendee to an event.
    *
    * @param name The name of the attendee.
    * @param id The facebook id of the attendee.
    * @param status The RSVP status of the attendee.
    */
    AttendeeInfo(const QString &name, const QString &id, const Attendee::PartStat &status);

    /**
    * @return The name of the attendee.
    */
    QString name() const;
    /**
    * @return Returns the facebook id of the attendee.
    */
    QString id() const;
    /**
    * @return Returns the RSVP state of the attendee.
    */
    Attendee::PartStat status() const;

  private:
    QString mName;
    QString mId;
    Attendee::PartStat mStatus;
};
typedef QSharedPointer<AttendeeInfo> AttendeeInfoPtr;


/**
* Class to describe a facebook event.
*/
class LIBKFACEBOOK_EXPORT EventInfo : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name WRITE setName READ name)
  Q_PROPERTY(QString start_time WRITE setStartTimeString READ startTimeString)
  Q_PROPERTY(QString end_time WRITE setEndTimeString READ endTimeString)
  Q_PROPERTY(QString location WRITE setLocation READ location)
  Q_PROPERTY(QString id WRITE setId READ id)
  Q_PROPERTY(QString description WRITE setDescription READ description)
  Q_PROPERTY(QString updated_time WRITE setUpdatedTimeString READ updatedTimeString)
  public:
    /**
    * @brief Set the name of the event.
    * @param name The name of the event.
    */
    void setName( const QString &name );
    /**
    * @return The name of the event.
    */
    QString name() const;

    /**
    * @brief Set the start time of the event.
    * @param startTime The start time of the event as a QString in the
    *                  "facebook format".
    */
    void setStartTimeString( const QString &startTime );
    /**
    * @return The start time of the event as a QString in the "facebook format".
    */
    QString startTimeString() const;
    /**
    * @return The start time of the event as a KDateTime object.
    */
    KDateTime startTime() const;

    /**
    * @brief Set the end time of the event.
    * @param endTime The end time of the event as a QString in the
    *                "facebook format".
    */
    void setEndTimeString( const QString &endTime );
    /**
    * @return The end time of the event as a QString in the "facebook format".
    */
    QString endTimeString() const;
    /**
    * @return The end time of the event as a KDateTime object.
    */
    KDateTime endTime() const;

    /**
    * @brief Set the location of the event,
    * @param location The location of the event,
    */
    void setLocation( const QString &location );
    /**
    * @return The location of the event
    */
    QString location() const;

    /**
    * @brief Set the facebook id of the event.
    * @param id The facebook id of the event.
    */
    void setId( const QString &id );
    /**
    * @return The facebook id of the event.
    */
    QString id() const;

    /**
    * @brief Set the description of the event.
    * @param description The description of the event.
    */
    void setDescription( const QString &description );
    /**
    * @return The description of the event.
    */
    QString description() const;

    /**
    * @brief Set the organizer of the event.
    * @param organizer The organizer of the event.
    */
    void setOrganizer( const QString &organizer );
    /**
    * @return The organizer of the event as a QString.
    */
    QString organizer() const;

    /**
    * @brief Set the time the event is last updated.
    * @param updatedTime The last update time of the event in "facebook format".
    */
    void setUpdatedTimeString( const QString & updatedTime );
    /**
    * @return The time of the last update of the event in "facebook format".
    */
    QString updatedTimeString() const;
    /**
    * @return The time of the last update of the event as a KDateTime object.
    */
    KDateTime updatedTime() const;

    /**
    * @brief Add a list of attendee objects to this event.
    * @param attendees A list of attendee objects of people that have responded
    *                  or are requested to respond to this event.
    */
    void addAttendees(const QList<AttendeeInfoPtr> &attendees);
    QList<AttendeeInfoPtr> attendees() const;

    EventPtr asEvent() const;

  private:
    QString mName;
    QString mStartTime;
    QString mEndTime;
    QString mLocation;
    QString mId;
    QString mDescription;
    QString mOrganizer;
    QString mUpdatedTime;

    QList<AttendeeInfoPtr> mAttendees;
};

typedef QSharedPointer<EventInfo> EventInfoPtr;

#endif

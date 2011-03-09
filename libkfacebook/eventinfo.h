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
typedef KCalCore::Event Event;
typedef KCalCore::Event::Ptr EventPtr;
typedef KCalCore::Incidence::Ptr IncidencePtr;
#else
#include <boost/shared_ptr.hpp>
#include <KCal/Event>
typedef KCal::Event Event;
typedef boost::shared_ptr<KCal::Event> EventPtr;
typedef boost::shared_ptr<KCal::Incidence> IncidencePtr;
#endif

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
    void setName( const QString &name );
    QString name() const;

    void setStartTimeString( const QString &startTime );
    QString startTimeString() const;
    KDateTime startTime() const;

    void setEndTimeString( const QString &endTime );
    QString endTimeString() const;
    KDateTime endTime() const;

    void setLocation( const QString &location );
    QString location() const;

    void setId( const QString &id );
    QString id() const;

    void setDescription( const QString &description );
    QString description() const;

    void setOrganizer( const QString &organizer );
    QString organizer() const;

    void setUpdatedTimeString( const QString & updatedTime );
    QString updatedTimeString() const;
    KDateTime updatedTime() const;

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
};

typedef QSharedPointer<EventInfo> EventInfoPtr;

#endif

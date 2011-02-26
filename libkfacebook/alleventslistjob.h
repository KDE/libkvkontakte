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
#ifndef ALLEVENTSLISTJOB_H
#define ALLEVENTSLISTJOB_H

#include "eventinfo.h"
#include "libkfacebook_export.h"

#include <KJob>
#include <QPointer>

class EventsListJob;

class LIBKFACEBOOK_EXPORT AllEventsListJob : public KJob
{
  Q_OBJECT
  public:
    explicit AllEventsListJob( const QString &accessToken );
    void setLowerLimit( const KDateTime &lowerLimit );
    QList<EventInfoPtr> allEvents() const;
    virtual void start();

  protected slots:
    void eventListJobFinished( KJob * job );

  protected:
    virtual bool doKill();

  private:
    KDateTime mLowerLimit;
    QPointer<EventsListJob> mCurrentJob;
    QString mAccessToken;
    QList<EventInfoPtr> mEvents;
};

#endif
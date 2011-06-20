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
#ifndef LONGPOLLSERVERINFO_H
#define LONGPOLLSERVERINFO_H

#include "libkvkontakte_export.h"

#include <QSharedPointer>

/**
* Class that describes a person on facebook
*/
class LIBKVKONTAKTE_EXPORT LongPollServerInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString key WRITE setKey READ key)
    Q_PROPERTY(QString server WRITE setServer READ server)
    Q_PROPERTY(QString ts WRITE setTs READ ts)

public:
    LongPollServerInfo();

    void setKey(const QString &key);
    QString key() const;

    void setServer(const QString &server);
    QString server() const;

    void setTs(const QString &ts);
    QString ts() const;


//     /**
//     * @brief Created a KABC::Addressee for all the information we have about this person.
//     *
//     * @return A KABC::Addressee of this person. 
//     */
//     KABC::Addressee toAddressee() const;
// 
//     static QStringList allQueryFields();

private:
    QString m_key;
    QString m_server;
    QString m_ts;
};

typedef QSharedPointer<LongPollServerInfo> LongPollServerInfoPtr;

#endif // LONGPOLLSERVERINFO_H

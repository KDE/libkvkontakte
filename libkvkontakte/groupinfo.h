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
#ifndef GROUPINFO_H
#define GROUPINFO_H

#include <QtCore/QSharedPointer>
//#include <KDE/KDateTime>

#include "libkvkontakte_export.h"

namespace Vkontakte
{

// http://vkontakte.ru/developers.php?oid=-1&p=groups.get
class LIBKVKONTAKTE_EXPORT GroupInfo : public QObject
{
    Q_OBJECT

    // gid
    Q_PROPERTY(int gid WRITE setGid READ gid)

    // If "extended" was requested, then these fields will be also available:
    // name, screen_name, is_closed, is_admin, photo, photo_medium, photo_big
    Q_PROPERTY(QString name WRITE setName READ name)
    Q_PROPERTY(QString screen_name WRITE setScreenName READ screenName)
    Q_PROPERTY(bool is_closed WRITE setClosed READ isClosed)
    Q_PROPERTY(bool is_admin WRITE setAdmin READ isAdmin)
    Q_PROPERTY(QString photo WRITE setPhoto READ photo)
    Q_PROPERTY(QString photo_medium WRITE setPhotoMedium READ photoMedium)
    Q_PROPERTY(QString photo_big WRITE setPhotoBig READ photoBig)

public:
    GroupInfo();
    ~GroupInfo();

    void setGid(int gid);
    int gid() const;

    void setName(const QString &name);
    QString name() const;

    void setScreenName(const QString &screenName);
    QString screenName() const;

    void setClosed(bool closed);
    bool isClosed() const;

    void setAdmin(bool admin);
    bool isAdmin() const;

    void setPhoto(const QString &photo);
    QString photo() const;

    void setPhotoMedium(const QString &photoMedium);
    QString photoMedium() const;

    void setPhotoBig(const QString &photoBig);
    QString photoBig() const;

private:
    class Private;
    Private * const d;
};

typedef QSharedPointer<GroupInfo> GroupInfoPtr;

} /* namespace Vkontakte */

#endif // GROUPINFO_H

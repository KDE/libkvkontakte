/*
 * Copyright (C) 2011  Alexander Potashev <aspotashev@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PHOTOINFO_H
#define PHOTOINFO_H

#include "libkvkontakte_export.h"

#include <KDE/KDateTime>

namespace Vkontakte
{

// http://vk.com/dev/photos.get
class LIBKVKONTAKTE_EXPORT PhotoInfo : public QObject
{
    Q_OBJECT

    // pid, aid, owner_id, src, src_small, src_big, text, created
    // When save_big=1: src_xbig, src_xxbig
    Q_PROPERTY(int pid WRITE setPid READ pid)
    Q_PROPERTY(int aid WRITE setAid READ aid)
    Q_PROPERTY(int owner_id WRITE setUid READ uid)
    Q_PROPERTY(QString src WRITE setSrc READ src)
    Q_PROPERTY(QString src_small WRITE setSrcSmall READ srcSmall)
    Q_PROPERTY(QString src_big WRITE setSrcBig READ srcBig)
    Q_PROPERTY(QString src_xbig WRITE setSrcXBig READ srcXBig)
    Q_PROPERTY(QString src_xxbig WRITE setSrcXXBig READ srcXXBig)
    Q_PROPERTY(QString text WRITE setText READ text)
    Q_PROPERTY(QString created WRITE setDateCreatedString READ dateCreatedString)

public:
    PhotoInfo();
    ~PhotoInfo();

    void setPid(int pid);
    int pid() const;

    void setAid(int aid);
    int aid() const;

    void setUid(int uid);
    int uid() const;

    void setSrc(const QString &src);
    QString src() const;

    void setSrcSmall(const QString &srcSmall);
    QString srcSmall() const;

    void setSrcBig(const QString &srcBig);
    QString srcBig() const;

    void setSrcXBig(const QString &srcXBig);
    QString srcXBig() const;

    void setSrcXXBig(const QString &srcXXBig);
    QString srcXXBig() const;

    void setText(const QString &text);
    QString text() const;

    /**
     * Set the creation time of the note
     * @param createdTime Time as UNIX timestamp
     */
    void setDateCreatedString(const QString &dateCreatedString);
    /**
     * Returns the creation date/time as a UNIX timestamp
     */
    QString dateCreatedString() const;
    /**
     * Returns the creation date/time in KDateTime
     */
    KDateTime dateCreated() const;

private:
    class Private;
    Private * const d;
};

typedef QSharedPointer<PhotoInfo> PhotoInfoPtr;

} /* namespace Vkontakte */

#endif // PHOTOINFO_H

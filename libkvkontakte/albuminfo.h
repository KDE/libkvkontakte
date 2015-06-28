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

#ifndef ALBUMINFO_H
#define ALBUMINFO_H

#include "libkvkontakte_export.h"

#include <KDE/KDateTime>

namespace Vkontakte
{

// http://vk.com/dev/photos.getAlbums
class LIBKVKONTAKTE_EXPORT AlbumInfo : public QObject
{
    Q_OBJECT

    // aid, thumb_id, owner_id, title, description, created, updated, size, privacy
    Q_PROPERTY(int aid WRITE setAid READ aid)
    Q_PROPERTY(int thumb_id WRITE setThumbId READ thumbId)
    Q_PROPERTY(int owner_id WRITE setUid READ uid)
    Q_PROPERTY(QString title WRITE setTitle READ title)
    Q_PROPERTY(QString description WRITE setDescription READ description)
    Q_PROPERTY(QString created WRITE setDateCreatedString READ dateCreatedString)
    Q_PROPERTY(QString updated WRITE setDateUpdatedString READ dateUpdatedString)
    Q_PROPERTY(int size WRITE setSize READ size)
    Q_PROPERTY(int privacy WRITE setPrivacy READ privacy)
    Q_PROPERTY(int comment_privacy WRITE setPrivacy READ privacy)

public:
    enum
    {
        PRIVACY_UNKNOWN = -1,
        PRIVACY_PUBLIC = 0,
        PRIVACY_FRIENDS = 1,
        PRIVACY_FRIENDS_OF_FRIENDS = 2,
        PRIVACY_PRIVATE = 3
    };

    AlbumInfo();
    ~AlbumInfo();

    void setAid(int aid);
    int aid() const;

    void setThumbId(int thumbId);
    int thumbId() const;

    void setUid(int uid);
    int uid() const;

    void setTitle(const QString &title);
    QString title() const;

    void setDescription(const QString &description);
    QString description() const;

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

    void setDateUpdatedString(const QString &dateUpdatedString);
    QString dateUpdatedString() const;
    KDateTime dateUpdated() const;

    void setSize(int size);
    int size() const;

    void setPrivacy(int privacy);
    int privacy() const;

    void setCommentPrivacy(int commentPrivacy);
    int commentPrivacy() const;

private:
    class Private;
    Private * const d;
};

typedef QSharedPointer<AlbumInfo> AlbumInfoPtr;

} /* namespace Vkontakte */

#endif // ALBUMINFO_H

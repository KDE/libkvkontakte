/* Copyright 2011 Roeland Jago Douma <unix@rullzer.com>
   Copyright 2011 Alexander Potashev <aspotashev@gmail.com>

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
#ifndef NOTEINFO_H
#define NOTEINFO_H

#include "libkvkontakte_export.h"

#include <KDateTime>
#include <QtCore/QObject>

namespace Vkontakte
{

// http://vkontakte.ru/developers.php?o=-1&p=notes.getById
class LIBKVKONTAKTE_EXPORT NoteInfo : public QObject
{
    Q_OBJECT

    // nid, uid, title, text, text_wiki, date, ncom, read_ncom, privacy, comment_privacy, can_comment
    Q_PROPERTY(int nid WRITE setNid READ nid)
    Q_PROPERTY(int uid WRITE setUid READ uid)
    Q_PROPERTY(QString title WRITE setTitle READ title)
    Q_PROPERTY(QString text WRITE setText READ text)
    Q_PROPERTY(QString text_wiki WRITE setTextWiki READ textWiki)
    Q_PROPERTY(QString date WRITE setDateString READ dateString)
    Q_PROPERTY(int ncom WRITE setNcom READ ncom)
    Q_PROPERTY(int read_ncom WRITE setReadNcom READ readNcom)
    Q_PROPERTY(int privacy WRITE setPrivacy READ privacy)
    Q_PROPERTY(int comment_privacy WRITE setCommentPrivacy READ commentPrivacy)
    Q_PROPERTY(bool can_comment WRITE setCanComment READ canComment)

public:
    NoteInfo();

    void setNid(int nid);
    int nid() const;

    void setUid(int uid);
    int uid() const;

    /**
     * Set the subject of the note
     * @param subject the subject
     */
    void setTitle(const QString &title);
    /**
     * Returns the subject of the note.
     */
    QString title() const;

    /**
     * Set the actual content of the note
     * @param text The actual content of the note
     */
    void setText( const QString &text);
    /**
     * Returns the content of the note.
     */
    QString text() const;

    /**
     * Set the actual content of the note
     * @param textWiki The actual content of the note
     */
    void setTextWiki( const QString &textWiki);
    /**
     * Returns the content of the note.
     */
    QString textWiki() const;

    /**
     * Set the creation time of the note
     * @param createdTime Time as UNIX timestamp
     */
    void setDateString(const QString &createdDate);
    /**
     * Returns the creation date/time as a UNIX timestamp
     */
    QString dateString() const;
    /**
     * Returns the creation date/time in KDateTime
     */
    KDateTime date() const;

    void setNcom(int ncom);
    int ncom() const;

    void setReadNcom(int readNcom);
    int readNcom() const;

    void setPrivacy(int privacy);
    int privacy() const;

    void setCommentPrivacy(int commentPrivacy);
    int commentPrivacy() const;

    void setCanComment(bool canComment);
    bool canComment() const;

private:
    int m_nid;          /* Vkontakte id of the note. */
    int m_uid;        /* Creator of the note. */
    QString m_title;     /* Subject of the note. */
    QString m_text;     /* Actual content of the note. */
    QString m_textWiki;
    QString m_date; /* Creation time of the note. */
    int m_ncom; /* Number of comments. */
    int m_readNcom; /* Number of read comments. */
    int m_privacy;
    int m_commentPrivacy;
    int m_canComment;

    class Private;
    Private * const d;
};

typedef QSharedPointer<NoteInfo> NoteInfoPtr;

} /* namespace Vkontakte */

#endif // NOTEINFO_H

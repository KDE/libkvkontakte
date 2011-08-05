/* Copyright 2010, 2011 Thomas McGuire <mcguire@kde.org>
   Copyright 2011 Roeland Jago Douma <unix@rullzer.com>
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
#ifndef VKONTAKTEJOBS_H
#define VKONTAKTEJOBS_H

#include "libkvkontakte_export.h"
#include <KDE/KJob>
#include <QtCore/QPointer>

typedef QPair<QString, QString> QueryItem;

class LIBKVKONTAKTE_EXPORT KJobWithSubjob : public KJob
{
protected:
    /** Kill the currentjobs and its subjobs */
    virtual bool doKill();


    QPointer<KJob> m_job; /** Pointer to the running job */
};

class LIBKVKONTAKTE_EXPORT KJobWithSubjobs : public KJob
{
protected:
    /** Kill the currentjobs and its subjobs */
    virtual bool doKill();

    QList<KJob *> m_jobs; /** Pointers to running jobs */
};

/**
 * VkontakteJob base class
 */
class LIBKVKONTAKTE_EXPORT VkontakteJob : public KJobWithSubjob
{
    Q_OBJECT
public:
    /**
     * Constructor that sets the path and the accesstoken
     *
     * @param method For the list of methods see http://vkontakte.ru/developers.php?o=-1&p=%CE%EF%E8%F1%E0%ED%E8%E5%20%EC%E5%F2%EE%E4%EE%E2%20API (in Russian)
     * @param accessToken The accessToken to access our data on vkontakte
     * @param httpPost Whether to make a POST http request instead of GET ("false" by default)
     * */
    VkontakteJob(const QString &method, const QString &accessToken, bool httpPost = false);

    virtual void start();

    enum JobErrorType { AuthenticationProblem = KJob::UserDefinedError + 42 };

protected:
    /** Add a query item to the list */
    void addQueryItem(const QString &key, const QString &value);

    /** Check for a return error and set the appropiate error messags */
    void handleError(const QVariant &data);

    virtual void handleData(const QVariant& data) = 0;

    /** Called right before sending request to server */
    virtual void prepareQueryItems() {}

    QString m_accessToken;         /** Vkontakte Access token */
    QString m_method;
    bool m_httpPost;
    QList<QueryItem> m_queryItems; /** The query items */

private slots:
    void jobFinished(KJob *job);
};

#endif

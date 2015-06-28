/*
 * Copyright (C) 2010, 2011  Thomas McGuire <mcguire@kde.org>
 * Copyright (C) 2011  Roeland Jago Douma <unix@rullzer.com>
 * Copyright (C) 2011, 2015  Alexander Potashev <aspotashev@gmail.com>
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

#ifndef VKONTAKTEJOBS_H
#define VKONTAKTEJOBS_H

#include "libkvkontakte_export.h"

#include <KDE/KJob>

#include <QtCore/QPointer>

namespace Vkontakte
{

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
     * @param method For the list of methods see http://vk.com/dev/methods (in English or Russian)
     * @param accessToken The accessToken to access our data on vkontakte
     * @param httpPost Whether to make a POST http request instead of GET ("false" by default)
     * */
    VkontakteJob(const QString &accessToken, const QString &method, bool httpPost = false);

    virtual void start();

    enum JobErrorType { AuthenticationProblem = KJob::UserDefinedError + 42 };

protected:
    /** Add a query item to the list */
    void addQueryItem(const QString &key, const QString &value);

private:
    /** Check for a return error and set the appropriate error messages */
    // Returns "true" if we will retry the call.
    bool handleError(const QVariant &data);

    virtual void handleData(const QVariant &data) = 0;

    /** Called right before sending request to server */
    virtual void prepareQueryItems() {}

    // TODO: cache url in a member variable
    KJob* createHttpJob();

    QString m_accessToken;         /** Vkontakte Access token */
    QString m_method;
    bool m_httpPost;
    QList<QueryItem> m_queryItems; /** The query items */

private Q_SLOTS:
    void jobFinished(KJob *kjob);
    void slotRetry();
};

} /* namespace Vkontakte */

#endif // VKONTAKTEJOBS_H

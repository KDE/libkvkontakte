/*
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

#include "userinfofulljob.h"
#include "userinfojob.h"
#include "cidsnamesjob.h"

#include <KDebug>

namespace Vkontakte
{

class UserInfoFullJob::Private
{
public:
    QString accessToken;
    QIntList uids;
    bool needCountryNames;
    bool needCityNames;
    QIntList countryIds;
    QIntList cityIds;

    UserInfoJob *mainJob;
    CidsNamesJob *countriesJob;
    CidsNamesJob *citiesJob;

    QList<UserInfoPtr> userInfo;
};

UserInfoFullJob::UserInfoFullJob(const QString &accessToken,
                                 const QIntList &uids,
                                 bool needCountryNames, bool needCityNames)
    : KJob()
    , d(new Private)
{
    d->accessToken = accessToken;
    d->uids = uids;
    d->needCountryNames = needCountryNames;
    d->needCityNames = needCityNames;
}

UserInfoFullJob::UserInfoFullJob(const QString &accessToken,
                                 int uid,
                                 bool needCountryNames, bool needCityNames)
    : KJob()
    , d(new Private)
{
    d->accessToken = accessToken;
    d->uids << uid;
    d->needCountryNames = needCountryNames;
    d->needCityNames = needCityNames;
}

UserInfoFullJob::~UserInfoFullJob()
{
    delete d;
}

void UserInfoFullJob::handleFinish()
{
    emitResult();
    d->mainJob = 0;
    d->countriesJob = 0;
    d->citiesJob = 0;
}

void UserInfoFullJob::handleError(KJob *kjob)
{
    setError(kjob->error());
    setErrorText(kjob->errorText());
    kWarning() << "Job error: " << kjob->errorString();
    handleFinish();
}

void UserInfoFullJob::start()
{
    d->mainJob = new UserInfoJob(d->accessToken, d->uids);
    connect(d->mainJob, SIGNAL(result(KJob*)), this, SLOT(mainJobFinished(KJob*)));
    d->mainJob->start();
}

void UserInfoFullJob::mainJobFinished(KJob *)
{
    Q_ASSERT(d->mainJob);
    if (d->mainJob->error())
    {
        handleError(d->mainJob);
    }
    else
    {
        d->userInfo = d->mainJob->userInfo();
        if (d->needCountryNames)
            startCountriesJob();
        else if (d->needCityNames) // only cities
            startCitiesJob();
        else // nothing needed
            handleFinish();
    }
}

// d->userInfo is ready
void UserInfoFullJob::startCountriesJob()
{
    // TODO: use usersPropertyToStringList
    QSet<int> cids; // this will remove duplicates
    foreach(const UserInfoPtr &user, d->userInfo)
        if (user->country())
            cids.insert(user->country());
    d->countryIds = cids.toList();

    d->countriesJob = new CidsNamesJob("database.getCountriesById", d->accessToken, d->countryIds);
    connect(d->countriesJob, SIGNAL(result(KJob*)), this, SLOT(countriesJobFinished(KJob*)));
    d->countriesJob->start();
}

void UserInfoFullJob::countriesJobFinished(KJob *)
{
    Q_ASSERT(d->countriesJob);
    if (d->countriesJob->error())
    {
        handleError(d->countriesJob);
    }
    else
    {
        QMap<int, QString> names = d->countriesJob->names();
        foreach(const UserInfoPtr &user, d->userInfo)
            user->setCountryString(names[user->country()]);

        if (d->needCityNames)
            startCitiesJob();
        else // nothing needed
            handleFinish();
    }
}

void UserInfoFullJob::startCitiesJob()
{
    // TODO: use usersPropertyToStringList
    QSet<int> cids; // this will remove duplicates
    foreach(const UserInfoPtr &user, d->userInfo)
        if (user->city())
            cids.insert(user->city());
    d->cityIds = cids.toList();

    d->citiesJob = new CidsNamesJob("database.getCitiesById", d->accessToken, d->cityIds);
    connect(d->citiesJob, SIGNAL(result(KJob*)), this, SLOT(citiesJobFinished(KJob*)));
    d->citiesJob->start();
}

void UserInfoFullJob::citiesJobFinished(KJob *)
{
    Q_ASSERT(d->citiesJob);
    if (d->citiesJob->error())
    {
        handleError(d->citiesJob);
    }
    else
    {
        QMap<int, QString> names = d->citiesJob->names();
        foreach(const UserInfoPtr &user, d->userInfo)
            user->setCityString(names[user->city()]);

        handleFinish();
    }
}

QList<UserInfoPtr> UserInfoFullJob::userInfo() const
{
    return d->userInfo;
}

} /* namespace Vkontakte */

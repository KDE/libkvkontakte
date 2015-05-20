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

#ifndef USERINFOFULLJOB_H
#define USERINFOFULLJOB_H

#include "userinfojob.h"

namespace Vkontakte
{

class LIBKVKONTAKTE_EXPORT UserInfoFullJob : public KJob
{
    Q_OBJECT
public:
    UserInfoFullJob(const QString &accessToken, const QIntList &uids,
                    bool needCountryNames = true, bool needCityNames = true);
    UserInfoFullJob(const QString &accessToken, int uid,
                    bool needCountryNames = true, bool needCityNames = true);
    ~UserInfoFullJob();

    QList<UserInfoPtr> userInfo() const;

    virtual void start();

private Q_SLOTS:
    void mainJobFinished(KJob *kjob);
    void countriesJobFinished(KJob *kjob);
    void citiesJobFinished(KJob *kjob);

private:
    void handleFinish();
    void handleError(KJob *kjob);

    void startCountriesJob();
    void startCitiesJob();


    class Private;
    Private * const d;
};

} /* namespace Vkontakte */

#endif // USERINFOFULLJOB_H

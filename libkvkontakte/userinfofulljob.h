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
#ifndef USERINFOFULLJOB_H
#define USERINFOFULLJOB_H

#include <KJob>
#include "userinfojob.h"

namespace Vkontakte
{

class CidsNamesJob;

class LIBKVKONTAKTE_EXPORT UserInfoFullJob : public KJob
{
    Q_OBJECT
public:
    UserInfoFullJob(const QString &accessToken, const QIntList &uids,
                    bool needCountryNames = true, bool needCityNames = true);
    UserInfoFullJob(const QString &accessToken, int uid,
                    bool needCountryNames = true, bool needCityNames = true);

    QList<UserInfoPtr> userInfo() const;

    virtual void start();

private slots:
    void mainJobFinished(KJob *);
    void countriesJobFinished(KJob *);
    void citiesJobFinished(KJob *);

private:
    void handleFinish();
    void handleError(KJob *job);

    void startCountriesJob();
    void startCitiesJob();


    QString m_accessToken;
    QIntList m_uids;
    bool m_needCountryNames;
    bool m_needCityNames;
    QIntList m_countryIds;
    QIntList m_cityIds;

    UserInfoJob *m_mainJob;
    CidsNamesJob *m_countriesJob;
    CidsNamesJob *m_citiesJob;

    QList<UserInfoPtr> m_userInfo;
};

} /* namespace Vkontakte */

#endif // USERINFOFULLJOB_H

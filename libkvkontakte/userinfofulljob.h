#ifndef USERINFOFULLJOB_H
#define USERINFOFULLJOB_H

#include <KJob>
#include "userinfojob.h"

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

#endif // USERINFOFULLJOB_H

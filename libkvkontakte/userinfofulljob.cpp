#include <KDebug>
#include "userinfofulljob.h"
#include "userinfojob.h"
#include "cidsnamesjob.h"

UserInfoFullJob::UserInfoFullJob(const QString &accessToken,
                                 const QIntList &uids,
                                 bool needCountryNames, bool needCityNames)
    : KJob()
    , m_accessToken(accessToken)
    , m_uids(uids)
    , m_needCountryNames(needCountryNames), m_needCityNames(needCityNames)
{
}

UserInfoFullJob::UserInfoFullJob(const QString &accessToken,
                                 int uid,
                                 bool needCountryNames, bool needCityNames)
    : KJob()
    , m_accessToken(accessToken)
    , m_needCountryNames(needCountryNames), m_needCityNames(needCityNames)
{
    m_uids << uid;
}

void UserInfoFullJob::handleFinish()
{
    emitResult();
    m_mainJob = 0;
    m_countriesJob = 0;
    m_citiesJob = 0;
}

void UserInfoFullJob::handleError(KJob* job)
{
    setError( job->error() );
    setErrorText( job->errorText() );
    kWarning() << "Job error: " << job->errorString();
    handleFinish();
}

void UserInfoFullJob::start()
{
    m_mainJob = new UserInfoJob(m_accessToken, m_uids);
    connect(m_mainJob, SIGNAL(result(KJob*)), this, SLOT(mainJobFinished(KJob*)));
    m_mainJob->start();
}

void UserInfoFullJob::mainJobFinished(KJob *)
{
    Q_ASSERT( m_mainJob );
    if ( m_mainJob->error() ) {
        handleError(m_mainJob);
    } else {
        m_userInfo = m_mainJob->userInfo();
        if (m_needCountryNames)
            startCountriesJob();
        else if (m_needCityNames) // only cities
            startCitiesJob();
        else // nothing needed
            handleFinish();
    }
}

// m_userInfo is ready
void UserInfoFullJob::startCountriesJob()
{
    // TODO: use usersPropertyToStringList
    QSet<int> cids; // this will remove duplicates
    foreach (const UserInfoPtr& user, m_userInfo) {
        if (user->country())
            cids.insert(user->country());
    }
    m_countryIds = cids.toList();

    m_countriesJob = new CidsNamesJob("places.getCountryById", m_accessToken, m_countryIds);
    connect(m_countriesJob, SIGNAL(result(KJob*)), this, SLOT(countriesJobFinished(KJob*)));
    m_countriesJob->start();
}

void UserInfoFullJob::countriesJobFinished(KJob *)
{
    Q_ASSERT( m_countriesJob );
    if ( m_countriesJob->error() ) {
        handleError(m_countriesJob);
    } else {
        QMap<int, QString> names = m_countriesJob->names();
        foreach (const UserInfoPtr &user, m_userInfo) {
            user->setCountryString(names[user->country()]);
        }

        if (m_needCityNames)
            startCitiesJob();
        else // nothing needed
            handleFinish();
    }
}

void UserInfoFullJob::startCitiesJob()
{
    // TODO: use usersPropertyToStringList
    QSet<int> cids; // this will remove duplicates
    foreach (const UserInfoPtr& user, m_userInfo) {
        if (user->city())
            cids.insert(user->city());
    }
    m_cityIds = cids.toList();

    m_citiesJob = new CidsNamesJob("places.getCityById", m_accessToken, m_cityIds);
    connect(m_citiesJob, SIGNAL(result(KJob*)), this, SLOT(citiesJobFinished(KJob*)));
    m_citiesJob->start();
}

void UserInfoFullJob::citiesJobFinished(KJob *)
{
    Q_ASSERT( m_citiesJob );
    if ( m_citiesJob->error() ) {
        handleError(m_citiesJob);
    } else {
        QMap<int, QString> names = m_citiesJob->names();
        foreach (const UserInfoPtr &user, m_userInfo) {
            user->setCityString(names[user->city()]);
        }

        handleFinish();
    }
}

QList<UserInfoPtr> UserInfoFullJob::userInfo() const
{
    return m_userInfo;
}

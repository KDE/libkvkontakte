#ifndef CIDSNAMESJOB_H
#define CIDSNAMESJOB_H

#include <vkontaktejobs.h>
#include <QtCore/QMap>
#include "qintlist.h"


class CidsNamesJob : public VkontakteJob
{
public:
    CidsNamesJob(const QString &method,
                 const QString &accessToken,
                 const QIntList &cids);

    QMap<int, QString> names() const;

protected:
    virtual void handleData(const QVariant& data);

    QIntList m_cids;
    QMap<int, QString> m_names; // cid -> name
};

#endif // CIDSNAMESJOB_H

#ifndef CIDSNAMESJOB_H
#define CIDSNAMESJOB_H

#include <vkontaktejobs.h>
#include <QtCore/QMap>


class CidsNamesJob : public VkontakteJob
{
public:
    CidsNamesJob ( const QString &method,
                   const QString &accessToken,
                   const QStringList &cids );

    QMap<QString, QString> names() const;

protected:
    virtual void handleData(const QVariant& data);

    QStringList m_cids;
    QMap<QString, QString> m_names; // cid -> name
};

#endif // CIDSNAMESJOB_H

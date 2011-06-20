#include "cidsnamesjob.h"
#include <QtCore/QMap>
#include <QVariant>

// http://vkontakte.ru/developers.php?o=-1&p=places.getCountryById
CidsNamesJob::CidsNamesJob(const QString &method,
                           const QString& accessToken,
                           const QIntList &cids)
    : VkontakteJob(method, accessToken)
{
    m_cids = cids;
    addQueryItem("cids", cids.join());
}

void CidsNamesJob::handleData(const QVariant& data)
{
    // We need QMap, because vkontakte.ru might reorder results
    // (I did not check it, but they do not guarantee the same order as in "cids")
    foreach(const QVariant &item, data.toList()) {
        QMap<QString, QVariant> item_map = item.toMap();
        m_names[item_map["cid"].toInt()] = item_map["name"].toString();
    }
}

QMap<int, QString> CidsNamesJob::names() const
{
    return m_names;
}

#ifndef QINTLIST_H
#define QINTLIST_H

#include <libkvkontakte/libkvkontakte_export.h>

#include <QtCore/QList>

class LIBKVKONTAKTE_EXPORT QIntList : public QList<int>
{
public:
    QIntList();
    QIntList(const QList<int> &list);

    QString join() const;
};

#endif // QINTLIST_H

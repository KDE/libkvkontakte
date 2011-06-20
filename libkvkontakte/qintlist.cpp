#include "qintlist.h"
#include <QtCore/QString>

QIntList::QIntList()
{
}

QIntList::QIntList(const QList<int> &list)
{
    append(list);
}

QString QIntList::join() const
{
    QString res;
    for (int i = 0; i < size(); i ++)
    {
        if (i)
            res += ",";
        res += QString::number(at(i));
    }

    return res;
}

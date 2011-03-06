/* Copyright 2011 Roeland Jago Douma <unix@rullzer.com>

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
#ifndef FACEBOOKADDJOB_H
#define FACEBOOKADDJOB_H

#include "libkfacebook_export.h"
#include <KJob>
#include <QStringList>
#include <QPointer>
#include <QMap>

typedef QPair<QString, QString> QueryItem;

class LIBKFACEBOOK_EXPORT FacebookAddJob : public KJob
{
  Q_OBJECT
  public:
    FacebookAddJob( const QString &path, const QString &accessToken );

    void addField( const QString &key, const QString &value );

    virtual void start();

    enum JobErrorType { AuthenticationProblem = KJob::UserDefinedError + 42 };

  protected slots:
    virtual void addJobFinished( KJob *job );
    virtual bool doKill();

  private:
    void handleError( const QVariant &data );

    QString mAccessToken;
    QString mPath;
    QPointer<KJob> mJob;
    QMap<QString, QString> mFields;
};

#endif

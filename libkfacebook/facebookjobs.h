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
#ifndef FACEBOOKJOBS_H
#define FACEBOOKJOBS_H

#include "libkfacebook_export.h"
#include <KJob>
#include <QStringList>
#include <QPointer>

typedef QPair<QString, QString> QueryItem;

class LIBKFACEBOOK_EXPORT FacebookJob : public KJob
{
  Q_OBJECT
  public:
    FacebookJob( const QString &path, const QString &accessToken );
    explicit FacebookJob( const QString &accessToken );

    void addQueryItem( const QString &key, const QString &value );

    virtual void start() = 0;

    enum JobErrorType { AuthenticationProblem = KJob::UserDefinedError + 42 };

  protected:
    virtual bool doKill();

  private slots:
    virtual void jobFinished( KJob *job ) = 0;

  protected:
    QString mAccessToken;
    QString mPath;
    QList<QueryItem> mQueryItems;
    QPointer<KJob> mJob;
};

class LIBKFACEBOOK_EXPORT FacebookAddJob : public FacebookJob
{
  Q_OBJECT

  public:
    FacebookAddJob( const QString &path, const QString &accessToken );

    virtual void start();

  private slots:
    void jobFinished(KJob *job);
};

class LIBKFACEBOOK_EXPORT FacebookDeleteJob : public FacebookJob
{
  Q_OBJECT

  public:
    FacebookDeleteJob( const QString &id, const QString &accessToken);

    virtual void start();

  private slots:
    void jobFinished(KJob *job);
};

class LIBKFACEBOOK_EXPORT FacebookGetJob : public FacebookJob
{
  Q_OBJECT

  public:
    FacebookGetJob( const QString &path, const QString &accessToken ); 
    explicit FacebookGetJob( const QString &accessToken ); 

    void setFields( const QStringList &fields ); 

    // If ids are set, the path is ignored.
    void setIds( const QStringList &ids ); 

    virtual void start();

  protected:
    virtual void handleData( const QVariant &data ) = 0; 

  private slots:
    void jobFinished( KJob *job ); 

  private:
    void handleError( const QVariant &data ); 

    QStringList mFields;
    QStringList mIds;
};

#endif

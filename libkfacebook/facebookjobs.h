/* Copyright 2010, 2011 Thomas McGuire <mcguire@kde.org>
   Copyright 2011 Roeland Jago Douma <unix@rullzer.com>

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

/**
 * Facebook Job base class
 */
class LIBKFACEBOOK_EXPORT FacebookJob : public KJob
{
  Q_OBJECT
  public:
    /** 
     * Constructor that sets the path and the accesstoken 
     *
     * @param path The path after https://graphs.facebook.com
     * @param accessToken The accessToken to access our data on facebook
     * */
    FacebookJob( const QString &path, const QString &accessToken );
    explicit FacebookJob( const QString &accessToken );

    /** Add a query item to the list */
    void addQueryItem( const QString &key, const QString &value );

    virtual void start() = 0;

    enum JobErrorType { AuthenticationProblem = KJob::UserDefinedError + 42 };
  
  protected:
    /** Kill the currentjobs and its subjobs */
    virtual bool doKill();

    /** Check for a return error and set the appropiate error messags */
    void handleError( const QVariant &data );

    QString mAccessToken;         /** Facebook Access token */
    QString mPath;                /** path after https://graph.facebook.com/ */
    QList<QueryItem> mQueryItems; /** The query items */
    QPointer<KJob> mJob;          /** Pointer to the running job */

  private slots:
    virtual void jobFinished( KJob *job ) = 0;

};

/**
 * FacebookJob that add data to facebook
 */
class LIBKFACEBOOK_EXPORT FacebookAddJob : public FacebookJob
{
  Q_OBJECT

  public:
    FacebookAddJob( const QString &path, const QString &accessToken );

    virtual void start();

  private slots:
    void jobFinished(KJob *job);
};

/**
 * Facebook job that deletes data from facebook
 */
class LIBKFACEBOOK_EXPORT FacebookDeleteJob : public FacebookJob
{
  Q_OBJECT

  public:
    FacebookDeleteJob( const QString &id, const QString &accessToken);

    virtual void start();

  private slots:
    void jobFinished(KJob *job);
};

/**
 * Facebook job that gets data from facebook
 */
class LIBKFACEBOOK_EXPORT FacebookGetJob : public FacebookJob
{
  Q_OBJECT

  public:
    FacebookGetJob( const QString &path, const QString &accessToken ); 
    explicit FacebookGetJob( const QString &accessToken ); 

    /** Set the fields the job should retrieve from facebook */
    void setFields( const QStringList &fields ); 

    /** Set the Id's the job should retrieve from facebook.
     * If this is set then the path is ignored */
    void setIds( const QStringList &ids ); 

    virtual void start();

  protected:
    virtual void handleData( const QVariant &data ) = 0; 

  private slots:
    void jobFinished( KJob *job ); 

  private:
    QStringList mFields; /** The field to retrieve from facebook */
    QStringList mIds;    /** The id's to retrieve from facebook */
};

#endif

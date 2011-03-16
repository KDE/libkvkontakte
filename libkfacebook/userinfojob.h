/* Copyright 2010 Thomas McGuire <mcguire@kde.org>

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
#ifndef USERINFOJOB_H
#define USERINFOJOB_H

#include "userinfo.h"
#include "facebookjobs.h"

/**
 * Get information about the currently authenticated user
 */
class LIBKFACEBOOK_EXPORT UserInfoJob : public FacebookGetJob
{
  Q_OBJECT
  public:
    /**
    * @brief Construct a job to retrieve the data of the currently authenticaed user.
    * @param accessToken The access token to access data on facebook.
    */
    UserInfoJob( const QString &accessToken );
    /**
    * @return A pointer to a userInfo object about the currently authenticated user.
    */
    UserInfoPtr userInfo() const;

  protected:
    /**
    * @brief Handles the data returned by the FacebookGetJob
    * @param data A JSON string containing the data of the currently authenticated user.
    */
    virtual void handleData( const QVariant& data );

  private:
    UserInfoPtr mUserInfo;
};

#endif

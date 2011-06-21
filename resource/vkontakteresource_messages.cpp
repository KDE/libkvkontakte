/* Copyright 2011 Alexander Potashev <aspotashev@gmail.com>

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

#include "vkontakteresource.h"
#include <config.h>
#include "settings.h"
#include "settingsdialog.h"

#include <Akonadi/AttributeFactory>
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <akonadi/changerecorder.h>
#include <libkvkontakte/friendlistjob.h>
#include <libkvkontakte/friendjob.h>
#include <libkvkontakte/userinfofulljob.h>
#include <libkvkontakte/profilephotojob.h>
#include <libkvkontakte/allmessageslistjob.h>

using namespace Akonadi;

void VkontakteResource::messageListFetched(KJob *job)
{
    Q_ASSERT( !m_idle );
    AllMessagesListJob * const listJob = dynamic_cast<AllMessagesListJob*>(job);
    Q_ASSERT( listJob );
    m_currentJobs.removeAll(job);

    if (listJob->error()) {
        abortWithError( i18n( "Unable to get messages from server: %1", listJob->errorString() ),
                        listJob->error() == VkontakteJob::AuthenticationProblem );
        return;
    }


    setItemStreamingEnabled( true ); // how does this work???

    m_allMessages = listJob->list();

    // IDs of users that you communicated with using messages
    QSet<int> uidsSet; // this will remove duplicates
    foreach (const MessageInfoPtr &item, m_allMessages) {
        uidsSet.insert(item->uid());
    }

    UserInfoJob * const usersJob = new UserInfoJob(Settings::self()->accessToken(), uidsSet.toList());
    m_currentJobs << usersJob;
    connect(usersJob, SIGNAL(result(KJob*)), this, SLOT(messageListUsersFetched(KJob*)));
    usersJob->start();
}

void VkontakteResource::messageListUsersFetched(KJob *job)
{
    Q_ASSERT(!m_idle);
    UserInfoJob * const usersJob = dynamic_cast<UserInfoJob*>(job);
    Q_ASSERT(usersJob);
    m_currentJobs.removeAll(job);

    if (usersJob->error()) {
        abortWithError( i18n( "Unable to get users for message list from server: %1", usersJob->errorString() ),
                        usersJob->error() == VkontakteJob::AuthenticationProblem );
        return;
    }

    // TODO: UserInfoJob::userInfoMap
    QList<UserInfoPtr> usersList = usersJob->userInfo();
    QMap<int, UserInfoPtr> usersMap;
    foreach (const UserInfoPtr user, usersList) {
        usersMap.insert(user->uid(), user);
    }

    Item::List items;
    foreach( const MessageInfoPtr &messageInfo, m_allMessages ) {
        // TODO: Multiple-user messages ("chat messages") should be handled differently
        if (!messageInfo->chatId().isEmpty() || !messageInfo->chatActive().isEmpty())
            continue;

        UserInfoPtr user = usersMap[messageInfo->uid()];
        if (!user.isNull()) {
            messageInfo->setOwnAddress(QString("%1 <you@vkontakte>").arg(Settings::self()->userName()));
            messageInfo->setUserAddress(QString("%1 %2 <%3@vkontakte>").arg(user->firstName()).arg(user->lastName()).arg(user->uid()));
        }

        KMime::Message::Ptr mail = messageInfo->asMessage();

        Item item;
        item.setRemoteId( messageInfo->remoteId() );
        item.setPayload( mail );
        item.setSize( mail->size() );
        item.setMimeType( KMime::Message::mimeType() );
        items.append(item);
    }

    itemsRetrieved(items);
    itemsRetrievalDone();
    finishMessagesFetching();
}

// void VkontakteResource::messageJobFinished(KJob *job)
// {
//     Q_ASSERT(!m_idle);
//     Q_ASSERT(m_currentJobs.indexOf(job) != -1);
//     MessageJob * const messageJob = dynamic_cast<MessageJob*>( job );
//     Q_ASSERT( messageJob );
//     m_currentJobs.removeAll(job);
// 
//     if ( messageJob->error() ) {
//         abortWithError( i18n( "Unable to get information about note from server: %1",
//                               messageJob->errorText() ) );
//     }
// 
//     Item item = messageJob->property( "Item" ).value<Item>();
//     KMime::Message::Ptr mail = messageJob->messageInfo()->asMessage();
//     item.setPayload(mail);
//     item.setSize(mail->size());
// 
//     itemRetrieved(item);
//     resetState();
// }

void VkontakteResource::finishMessagesFetching()
{
    emit percent(100);
    emit status( Idle, i18n( "All messages fetched from server." ) );
    resetState();
}

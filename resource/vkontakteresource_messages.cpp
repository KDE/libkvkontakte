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
#include <libkvkontakte/discussionslistjob.h>

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
    foreach (const UserInfoPtr user, usersList) {
        m_messagesUsersMap.insert(user->uid(), user);
    }

    DiscussionsListJob * const discussionsJob = new DiscussionsListJob(Settings::self()->accessToken());
    m_currentJobs << discussionsJob;
    connect(discussionsJob, SIGNAL(result(KJob*)), this, SLOT(messageDiscussionsFetched(KJob*)));
    discussionsJob->start();
}

void VkontakteResource::messageDiscussionsFetched(KJob *job)
{
    Q_ASSERT(!m_idle);
    DiscussionsListJob * const discussionsJob = dynamic_cast<DiscussionsListJob*>(job);
    Q_ASSERT(discussionsJob);
    m_currentJobs.removeAll(job);

    if (discussionsJob->error()) {
        abortWithError( i18n( "Unable to get discussion list from server: %1", discussionsJob->errorString() ),
                        discussionsJob->error() == VkontakteJob::AuthenticationProblem );
        return;
    }

    QSet<int> discussionIds;
    foreach (const MessageInfoPtr &message, discussionsJob->list()) {
        discussionIds.insert(message->mid());
    }

    Item::List items;
    for (int i = 0; i < m_allMessages.size(); i ++) {
        const MessageInfoPtr &messageInfo = m_allMessages[i];

        // TODO: Multiple-user messages ("chat messages") should be handled differently
        if (!messageInfo->chatId().isEmpty() || !messageInfo->chatActive().isEmpty())
            continue;

        UserInfoPtr user = m_messagesUsersMap[messageInfo->uid()];
        QString userAddress;
        QString ownAddress;
        if (!user.isNull()) {
            userAddress = QString("%1 %2 <%3@vkontakte>").arg(user->firstName()).arg(user->lastName()).arg(user->uid());
            ownAddress = QString("%1 <you@vkontakte>").arg(Settings::self()->userName());
        }

        // Trying to find the previous message in the same discussion
        int j = i - 1;
        while (j >= 0 && m_allMessages[j]->uid() != messageInfo->uid())
            j --;
        // If we bump into the next discussion (older one), then
        // not attaching our message to it.
        if (j >= 0 && discussionIds.contains(m_allMessages[j]->mid())) {
            kDebug() << "next discussion starts:" << m_allMessages[j]->mid();
            j = -1;
        }

        kWarning() << "message threading: " << i << "->" << j << ", mid: " << m_allMessages[i]->mid() << "->" << (j == -1 ? -1 : m_allMessages[j]->mid());

        if (messageInfo->title().isEmpty()) {
            messageInfo->setTitle(j >= 0 ? m_allMessages[j]->title() : QString("No subject <mid%1>").arg(messageInfo->mid()));
        }

        // Cut the thread when subject changes
        if (j >= 0 && messageInfo->title() != m_allMessages[j]->title()) {
            j = -1;
        }

        KMime::Message::Ptr mail =
            messageInfo->asMessage(userAddress, ownAddress,
                                   QString("<%1@vkontakte>").arg(m_allMessages[i]->remoteId()),
                                   j >= 0 ? QString("<%1@vkontakte>").arg(m_allMessages[j]->remoteId()) : QString());

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

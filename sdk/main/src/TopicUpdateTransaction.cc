/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "TopicUpdateTransaction.h"
#include "KeyList.h"
#include "impl/DurationConverter.h"
#include "impl/Node.h"
#include "impl/TimestampConverter.h"

#include <grpcpp/client_context.h>
#include <proto/consensus_update_topic.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
TopicUpdateTransaction::TopicUpdateTransaction(const proto::TransactionBody& transactionBody)
{
  if (!transactionBody.has_consensusupdatetopic())
  {
    throw std::invalid_argument("Transaction body doesn't contain ConsensusUpdateTopic data");
  }

  const proto::ConsensusUpdateTopicTransactionBody& body = transactionBody.consensusupdatetopic();

  if (body.has_topicid())
  {
    mTopicId = TopicId::fromProtobuf(body.topicid());
  }

  if (body.has_memo())
  {
    mMemo = body.memo().value();
  }

  if (body.has_expirationtime())
  {
    mExpirationTime = internal::TimestampConverter::fromProtobuf(body.expirationtime());
  }

  if (body.has_adminkey())
  {
    mAdminKey = Key::fromProtobuf(body.adminkey());
  }

  if (body.has_submitkey())
  {
    mSubmitKey = Key::fromProtobuf(body.submitkey());
  }

  if (body.has_autorenewperiod())
  {
    mAutoRenewPeriod = internal::DurationConverter::fromProtobuf(body.autorenewperiod());
  }

  if (body.has_autorenewaccount())
  {
    mAutoRenewAccountId = AccountId::fromProtobuf(body.autorenewaccount());
  }
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::setTopicId(const TopicId& topicId)
{
  requireNotFrozen();
  mTopicId = topicId;
  return *this;
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::setMemo(std::string_view memo)
{
  requireNotFrozen();
  mMemo = memo;
  return *this;
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::setExpirationTime(const std::chrono::system_clock::time_point& expiry)
{
  requireNotFrozen();
  mExpirationTime = expiry;
  return *this;
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::setAdminKey(const std::shared_ptr<Key>& key)
{
  requireNotFrozen();
  mAdminKey = key;
  return *this;
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::setSubmitKey(const std::shared_ptr<Key>& key)
{
  requireNotFrozen();
  mSubmitKey = key;
  return *this;
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::setAutoRenewPeriod(const std::chrono::duration<double>& autoRenew)
{
  requireNotFrozen();
  mAutoRenewPeriod = autoRenew;
  return *this;
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::setAutoRenewAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mAutoRenewAccountId = accountId;
  return *this;
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::clearTopicMemo()
{
  requireNotFrozen();
  mMemo = "";
  return *this;
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::clearAdminKey()
{
  requireNotFrozen();
  mAdminKey = std::make_unique<KeyList>();
  return *this;
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::clearSubmitKey()
{
  requireNotFrozen();
  mSubmitKey = std::make_unique<KeyList>();
  return *this;
}

//-----
TopicUpdateTransaction& TopicUpdateTransaction::clearAutoRenewAccountId()
{
  requireNotFrozen();
  mAutoRenewAccountId = AccountId();
  return *this;
}

//-----
proto::Transaction TopicUpdateTransaction::makeRequest(const Client& client,
                                                       const std::shared_ptr<internal::Node>&) const
{
  return signTransaction(generateTransactionBody(&client), client);
}

//-----
grpc::Status TopicUpdateTransaction::submitRequest(const Client& client,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kConsensusUpdateTopic, makeRequest(client, node), deadline, response);
}

//-----
void TopicUpdateTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_consensusupdatetopic(build());
}

//-----
proto::ConsensusUpdateTopicTransactionBody* TopicUpdateTransaction::build() const
{
  auto body = std::make_unique<proto::ConsensusUpdateTopicTransactionBody>();

  if (!(mTopicId == TopicId()))
  {
    body->set_allocated_topicid(mTopicId.toProtobuf().release());
  }

  if (mMemo.has_value())
  {
    body->mutable_memo()->set_value(mMemo.value());
  }

  if (mExpirationTime.has_value())
  {
    body->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(mExpirationTime.value()));
  }

  if (mAdminKey)
  {
    body->set_allocated_adminkey(mAdminKey->toProtobufKey().release());
  }

  if (mSubmitKey)
  {
    body->set_allocated_submitkey(mSubmitKey->toProtobufKey().release());
  }

  if (mAutoRenewPeriod.has_value())
  {
    body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(mAutoRenewPeriod.value()));
  }

  if (mAutoRenewAccountId.has_value())
  {
    body->set_allocated_autorenewaccount(mAutoRenewAccountId->toProtobuf().release());
  }

  return body.release();
}

} // namespace Hedera

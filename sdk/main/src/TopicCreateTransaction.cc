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
#include "TopicCreateTransaction.h"
#include "impl/DurationConverter.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/consensus_create_topic.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
TopicCreateTransaction::TopicCreateTransaction(const proto::TransactionBody& transactionBody)
{
  if (!transactionBody.has_consensuscreatetopic())
  {
    throw std::invalid_argument("Transaction body doesn't contain ConsensusCreateTopic data");
  }

  const proto::ConsensusCreateTopicTransactionBody& body = transactionBody.consensuscreatetopic();

  mMemo = body.memo();

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
TopicCreateTransaction& TopicCreateTransaction::setMemo(std::string_view memo)
{
  requireNotFrozen();
  mMemo = memo;
  return *this;
}

//-----
TopicCreateTransaction& TopicCreateTransaction::setAdminKey(const std::shared_ptr<Key>& key)
{
  requireNotFrozen();
  mAdminKey = key;
  return *this;
}

//-----
TopicCreateTransaction& TopicCreateTransaction::setSubmitKey(const std::shared_ptr<Key>& key)
{
  requireNotFrozen();
  mSubmitKey = key;
  return *this;
}

//-----
TopicCreateTransaction& TopicCreateTransaction::setAutoRenewPeriod(const std::chrono::duration<double>& autoRenew)
{
  requireNotFrozen();
  mAutoRenewPeriod = autoRenew;
  return *this;
}

//-----
TopicCreateTransaction& TopicCreateTransaction::setAutoRenewAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mAutoRenewAccountId = accountId;
  return *this;
}

//-----
proto::Transaction TopicCreateTransaction::makeRequest(const Client& client,
                                                       const std::shared_ptr<internal::Node>&) const
{
  return signTransaction(generateTransactionBody(&client), client);
}

//-----
grpc::Status TopicCreateTransaction::submitRequest(const Client& client,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kConsensusCreateTopic, makeRequest(client, node), deadline, response);
}

//-----
void TopicCreateTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_consensuscreatetopic(build());
}

//-----
proto::ConsensusCreateTopicTransactionBody* TopicCreateTransaction::build() const
{
  auto body = std::make_unique<proto::ConsensusCreateTopicTransactionBody>();

  body->set_memo(mMemo);

  if (mAdminKey)
  {
    body->set_allocated_adminkey(mAdminKey->toProtobufKey().release());
  }

  if (mSubmitKey)
  {
    body->set_allocated_submitkey(mSubmitKey->toProtobufKey().release());
  }

  body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(mAutoRenewPeriod));

  if (mAutoRenewAccountId.has_value())
  {
    body->set_allocated_autorenewaccount(mAutoRenewAccountId->toProtobuf().release());
  }

  return body.release();
}

} // namespace Hedera

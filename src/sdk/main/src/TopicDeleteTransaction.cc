/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "TopicDeleteTransaction.h"
#include "impl/Node.h"

#include <consensus_delete_topic.pb.h>
#include <grpcpp/client_context.h>
#include <stdexcept>
#include <transaction.pb.h>

namespace Hedera
{
//-----
TopicDeleteTransaction::TopicDeleteTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TopicDeleteTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TopicDeleteTransaction::TopicDeleteTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TopicDeleteTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TopicDeleteTransaction& TopicDeleteTransaction::setTopicId(const TopicId& TopicId)
{
  requireNotFrozen();
  mTopicId = TopicId;
  return *this;
}

//-----
grpc::Status TopicDeleteTransaction::submitRequest(const proto::Transaction& request,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kConsensusDeleteTopic, request, deadline, response);
}

//-----
void TopicDeleteTransaction::validateChecksums(const Client& client) const
{
  if (mTopicId.has_value())
  {
    mTopicId->validateChecksum(client);
  }
}

//-----
void TopicDeleteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_consensusdeletetopic(build());
}

//-----
void TopicDeleteTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_consensusdeletetopic())
  {
    throw std::invalid_argument("Transaction body doesn't contain ConsensusDeleteTopic data");
  }

  const proto::ConsensusDeleteTopicTransactionBody& body = transactionBody.consensusdeletetopic();

  if (body.has_topicid())
  {
    mTopicId = TopicId::fromProtobuf(body.topicid());
  }
}

//-----
proto::ConsensusDeleteTopicTransactionBody* TopicDeleteTransaction::build() const
{
  auto body = std::make_unique<proto::ConsensusDeleteTopicTransactionBody>();

  if (mTopicId.has_value())
  {
    body->set_allocated_topicid(mTopicId->toProtobuf().release());
  }

  return body.release();
}

} // namespace Hedera
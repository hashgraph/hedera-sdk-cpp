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
#include "TopicDeleteTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/consensus_delete_topic.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TopicDeleteTransaction::TopicDeleteTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TopicDeleteTransaction>(transactionBody)
{
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
TopicDeleteTransaction& TopicDeleteTransaction::setTopicId(const TopicId& TopicId)
{
  requireNotFrozen();
  mTopicId = TopicId;
  return *this;
}

//-----
proto::Transaction TopicDeleteTransaction::makeRequest(const Client& client,
                                                       const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody transactionBody = generateTransactionBody(client);
  transactionBody.set_allocated_consensusdeletetopic(build());

  return signTransaction(transactionBody, client);
}

//-----
grpc::Status TopicDeleteTransaction::submitRequest(const Client& client,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kConsensusDeleteTopic, makeRequest(client, node), deadline, response);
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
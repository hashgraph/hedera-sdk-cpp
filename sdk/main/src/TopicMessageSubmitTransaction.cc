/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "TopicMessageSubmitTransaction.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <grpcpp/client_context.h>
#include <proto/consensus_submit_message.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TopicMessageSubmitTransaction::TopicMessageSubmitTransaction(const proto::TransactionBody& transactionBody)
  : ChunkedTransaction<TopicMessageSubmitTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TopicMessageSubmitTransaction::TopicMessageSubmitTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : ChunkedTransaction<TopicMessageSubmitTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TopicMessageSubmitTransaction& TopicMessageSubmitTransaction::setTopicId(const TopicId& topicId)
{
  requireNotFrozen();
  mTopicId = topicId;
  return *this;
}

//-----
TopicMessageSubmitTransaction& TopicMessageSubmitTransaction::setMessage(const std::vector<std::byte>& message)
{
  requireNotFrozen();
  setData(message);
  return *this;
}

//-----
TopicMessageSubmitTransaction& TopicMessageSubmitTransaction::setMessage(std::string_view message)
{
  requireNotFrozen();
  setData(message);
  return *this;
}

//-----
grpc::Status TopicMessageSubmitTransaction::submitRequest(const proto::Transaction& request,
                                                          const std::shared_ptr<internal::Node>& node,
                                                          const std::chrono::system_clock::time_point& deadline,
                                                          proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kConsensusSubmitMessage, request, deadline, response);
}

//-----
void TopicMessageSubmitTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_consensussubmitmessage(build());
}

//-----
void TopicMessageSubmitTransaction::addToChunk(uint32_t chunk, uint32_t total, proto::TransactionBody& body) const
{
  body.set_allocated_consensussubmitmessage(build(static_cast<int>(chunk)));
  body.mutable_consensussubmitmessage()->mutable_chunkinfo()->set_allocated_initialtransactionid(
    getTransactionId().toProtobuf().release());
  body.mutable_consensussubmitmessage()->mutable_chunkinfo()->set_number(static_cast<int32_t>(chunk + 1));
  body.mutable_consensussubmitmessage()->mutable_chunkinfo()->set_total(static_cast<int32_t>(total));
}

//-----
void TopicMessageSubmitTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_consensussubmitmessage())
  {
    throw std::invalid_argument("Transaction body doesn't contain ConsensusSubmitMessage data");
  }

  const proto::ConsensusSubmitMessageTransactionBody& body = transactionBody.consensussubmitmessage();

  if (body.has_topicid())
  {
    mTopicId = TopicId::fromProtobuf(body.topicid());
  }
}

//-----
proto::ConsensusSubmitMessageTransactionBody* TopicMessageSubmitTransaction::build(int chunk) const
{
  auto body = std::make_unique<proto::ConsensusSubmitMessageTransactionBody>();

  if (!(mTopicId == TopicId()))
  {
    body->set_allocated_topicid(mTopicId.toProtobuf().release());
  }

  body->set_message(internal::Utilities::byteVectorToString(
    (chunk >= 0) ? getDataForChunk(static_cast<unsigned int>(chunk)) : getData()));
  return body.release();
}

} // namespace Hedera

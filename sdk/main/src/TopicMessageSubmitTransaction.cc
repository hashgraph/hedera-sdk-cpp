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
#include "TopicMessageSubmitTransaction.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <grpcpp/client_context.h>
#include <proto/consensus_submit_message.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TopicMessageSubmitTransaction::TopicMessageSubmitTransaction(const proto::TransactionBody& transactionBody)
{
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
void TopicMessageSubmitTransaction::createChunk(const std::vector<std::byte>& data, int32_t chunk, int32_t total)
{
  // Set up mData for this chunk.
  ChunkedTransaction<TopicMessageSubmitTransaction>::createChunk(data, chunk, total);

  // Copy the variables.
  if (chunk == 0)
  {
    // If this is 1st chunk, then the initial transaction ID is still in the Transaction's transactionID field.
    mInitialTransactionId = getTransactionId();
  }

  mChunkNum = chunk;
  mTotalNumOfChunks = total;
}

//-----
proto::Transaction TopicMessageSubmitTransaction::makeRequest(const Client& client,
                                                              const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody txBody = generateTransactionBody(&client);
  addToBody(txBody);
  return signTransaction(txBody, client);
}

//-----
grpc::Status TopicMessageSubmitTransaction::submitRequest(const Client& client,
                                                          const std::chrono::system_clock::time_point& deadline,
                                                          const std::shared_ptr<internal::Node>& node,
                                                          proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kConsensusSubmitMessage, makeRequest(client, node), deadline, response);
}

//-----
void TopicMessageSubmitTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_consensussubmitmessage(build());
}

//-----
proto::ConsensusSubmitMessageTransactionBody* TopicMessageSubmitTransaction::build() const
{
  auto body = std::make_unique<proto::ConsensusSubmitMessageTransactionBody>();

  if (!(mTopicId == TopicId()))
  {
    body->set_allocated_topicid(mTopicId.toProtobuf().release());
  }

  body->set_message(internal::Utilities::byteVectorToString(getData()));

  body->mutable_chunkinfo()->set_allocated_initialtransactionid(mInitialTransactionId.toProtobuf().release());
  body->mutable_chunkinfo()->set_total(mTotalNumOfChunks);
  body->mutable_chunkinfo()->set_number(mChunkNum + 1);

  return body.release();
}

} // namespace Hedera

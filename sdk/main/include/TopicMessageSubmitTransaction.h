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
#ifndef HEDERA_SDK_CPP_TOPIC_MESSAGE_SUBMIT_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOPIC_MESSAGE_SUBMIT_TRANSACTION_H_

#include "ChunkedTransaction.h"
#include "TopicId.h"
#include "TransactionId.h"

#include <cstddef>
#include <string_view>
#include <vector>

namespace proto
{
class ConsensusSubmitMessageTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction that submits a topic message to the Hedera network. To access the messages submitted to a topic ID,
 * subscribe to the topic via a mirror node. The mirror node will publish the ordered messages to subscribers. Once the
 * transaction is successfully executed, the receipt of the transaction will include the topic's updated sequence number
 * and topic running hash.
 *
 * Transaction Signing Requirements:
 *  - Anyone can submit a message to a public topic.
 *  - The submit key is required to sign the transaction for a private topic.
 */
class TopicMessageSubmitTransaction : public ChunkedTransaction<TopicMessageSubmitTransaction>
{
public:
  TopicMessageSubmitTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a ConsensusSubmitMessage transaction.
   */
  explicit TopicMessageSubmitTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the topic to which to submit a message.
   *
   * @param topicId The ID of the topic to which to submit a message.
   * @return A reference to this TopicMessageSubmitTransaction object with the newly-set topic ID.
   * @throws IllegalStateException If this TopicMessageSubmitTransaction is frozen.
   */
  TopicMessageSubmitTransaction& setTopicId(const TopicId& topicId);

  /**
   * Set the message to submit.
   *
   * @param message The message to submit.
   * @return A reference to this TopicMessageSubmitTransaction object with the newly-set message.
   * @throws IllegalStateException If this TopicMessageSubmitTransaction is frozen.
   */
  TopicMessageSubmitTransaction& setMessage(const std::vector<std::byte>& message);
  TopicMessageSubmitTransaction& setMessage(std::string_view message);

  /**
   * Get the ID of the topic to which to submit a message.
   *
   * @return The ID of the topic to which to submit a message.
   */
  [[nodiscard]] inline TopicId getTopicId() const { return mTopicId; }

  /**
   * Get the message to submit.
   *
   * @return The message to submit.
   */
  [[nodiscard]] inline std::vector<std::byte> getMessage() const { return getData(); }

private:
  friend class ChunkedTransaction<TopicMessageSubmitTransaction>;
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TopicMessageSubmitTransaction object.
   *
   * @param client The Client trying to construct this TopicMessageSubmitTransaction.
   * @param node   The Node to which this TopicMessageSubmitTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TopicMessageSubmitTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                TopicMessageSubmitTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TopicMessageSubmitTransaction to a Node.
   *
   * @param client   The Client submitting this TopicMessageSubmitTransaction.
   * @param deadline The deadline for submitting this TopicMessageSubmitTransaction.
   * @param node     Pointer to the Node to which this TopicMessageSubmitTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the TopicMessageSubmitTransaction protobuf representation to the
   * Transaction protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Derive from ChunkedTransaction. Perform any needed actions for this ChunkedTransaction after it has been chunked.
   *
   * @param data  The whole entirety of data to be sent.
   * @param chunk The chunk number to create.
   * @param total The total number of chunks to create.
   */
  void onChunk(int32_t chunk, int32_t total) override;

  /**
   * Build a ConsensusSubmitMessageTransactionBody protobuf object from this ConsensusSubmitMessageTransactionBody
   * object.
   *
   * @return A pointer to a TopicMessageSubmitTransactionBody protobuf object filled with this
   *         ConsensusSubmitMessageTransactionBody object's data.
   */
  [[nodiscard]] proto::ConsensusSubmitMessageTransactionBody* build() const;

  /**
   * The ID of the topic to which to send a message.
   */
  TopicId mTopicId;

  /**
   * Used during transaction chunking. This is the ID of the first chunk transaction, which is copied to all other
   * chunks.
   */
  TransactionId mInitialTransactionId;

  /**
   * Used during transaction chunking. The total number of chunks that are being sent as a part of this
   * TopicMessageSubmitTransaction.
   */
  int32_t mTotalNumOfChunks = 0;

  /**
   * Used during transaction chunking. The chunk number of the chunk being submitted (from 1 to mTotalNumOfChunks).
   */
  int32_t mChunkNum = 0;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOPIC_MESSAGE_SUBMIT_TRANSACTION_H_

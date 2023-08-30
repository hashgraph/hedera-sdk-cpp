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
#ifndef HEDERA_SDK_CPP_TOPIC_DELETE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOPIC_DELETE_TRANSACTION_H_

#include "TopicId.h"
#include "Transaction.h"

#include <optional>

namespace proto
{
class ConsensusDeleteTopicTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction that deletes a topic from the Hedera network. Once a topic is deleted, the topic cannot be recovered to
 * receive messages and all submitMessage calls will fail. Older messages can still be accessed, even after the topic is
 * deleted, via the mirror node.
 *
 * Transaction Signing Requirements:
 *  - If the adminKey was set upon the creation of the topic, the adminKey is required to sign to successfully delete
 *    the topic.
 *  - If no adminKey was set upon the creating of the topic, you cannot delete the topic and will receive an
 *    UNAUTHORIZED error.
 */
class TopicDeleteTransaction : public Transaction<TopicDeleteTransaction>
{
public:
  TopicDeleteTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a ConsensusDeleteTopic transaction.
   */
  explicit TopicDeleteTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the topic to delete.
   *
   * @param topicId The ID of the topic to delete.
   * @return A reference to this TopicDeleteTransaction object with the newly-set topic ID.
   * @throws IllegalStateException If this TopicDeleteTransaction is frozen.
   */
  TopicDeleteTransaction& setTopicId(const TopicId& topicId);

  /**
   * Get the ID of the topic this TopicDeleteTransaction is currently configured to delete.
   *
   * @return The ID of the topic this TopicDeleteTransaction is currently configured to delete. Uninitialized if no
   *         topic ID has been set.
   */
  [[nodiscard]] inline std::optional<TopicId> getTopicId() const { return mTopicId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TopicDeleteTransaction object.
   *
   * @param client The Client trying to construct this TopicDeleteTransaction.
   * @param node   The Node to which this TopicDeleteTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TopicDeleteTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                TopicDeleteTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TopicDeleteTransaction to a Node.
   *
   * @param client   The Client submitting this TopicDeleteTransaction.
   * @param deadline The deadline for submitting this TopicDeleteTransaction.
   * @param node     Pointer to the Node to which this TopicDeleteTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the TopicDeleteTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a ConsensusDeleteTopicTransactionBody protobuf object from this TopicDeleteTransaction object.
   *
   * @return A pointer to a ConsensusDeleteTopicTransactionBody protobuf object filled with this TopicDeleteTransaction
   *         object's data.
   */
  [[nodiscard]] proto::ConsensusDeleteTopicTransactionBody* build() const;

  /**
   * The ID of the topic to delete.
   */
  std::optional<TopicId> mTopicId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOPIC_DELETE_TRANSACTION_H_

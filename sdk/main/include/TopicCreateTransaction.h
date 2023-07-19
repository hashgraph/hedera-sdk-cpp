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
#ifndef HEDERA_SDK_CPP_TOPIC_CREATE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOPIC_CREATE_TRANSACTION_H_

#include "AccountId.h"
#include "Defaults.h"
#include "Key.h"
#include "Transaction.h"

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace proto
{
class ConsensusCreateTopicTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction that creates a new topic recognized by the Hedera network. The newly generated topic can be referenced
 * by its topic ID. The topic ID is used to identify a unique topic to which to submit messages. You can obtain the new
 * topic ID by requesting the receipt of the transaction. All messages within a topic are sequenced with respect to one
 * another and are provided a unique sequence number.
 *
 * You can also create a private topic where only authorized parties can submit messages to that topic. To create a
 * private topic you would need to set the mSubmitKey of the transaction. The mSubmitKey value is then shared with the
 * authorized parties and is required to successfully submit messages to the private topic.
 *
 * Transaction Signing Requirements:
 *  - If an admin key is specified, the admin key must sign the transaction.
 *  - If an admin key is not specified, the topic is immutable.
 *  - If an auto-renew account is specified, that account must also sign this transaction.
 */
class TopicCreateTransaction : public Transaction<TopicCreateTransaction>
{
public:
  TopicCreateTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a ConsensusCreateTopic transaction.
   */
  explicit TopicCreateTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the desired memo for the new topic.
   *
   * @param memo The desired memo for the new topic.
   * @return A reference to this TopicCreateTransaction object with the newly-set memo.
   * @throws IllegalStateException If this TopicCreateTransaction is frozen.
   */
  TopicCreateTransaction& setMemo(std::string_view memo);

  /**
   * Set the desired admin key for the new topic.
   *
   * @param key The desired admin key for the new topic.
   * @return A reference to this TopicCreateTransaction object with the newly-set admin key.
   * @throws IllegalStateException If this TopicCreateTransaction is frozen.
   */
  TopicCreateTransaction& setAdminKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired submit key for the new topic.
   *
   * @param key The desired submit key for the new topic.
   * @return A reference to this TopicCreateTransaction object with the newly-set submit key.
   * @throws IllegalStateException If this TopicCreateTransaction is frozen.
   */
  TopicCreateTransaction& setSubmitKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired auto-renew period for the new topic.
   *
   * @param autoRenew The desired auto-renew period for the new topic.
   * @return A reference to this TopicCreateTransaction object with the newly-set auto-renew period.
   * @throws IllegalStateException If this TopicCreateTransaction is frozen.
   */
  TopicCreateTransaction& setAutoRenewPeriod(const std::chrono::duration<double>& autoRenew);

  /**
   * Set the ID of the desired auto-renew account for the new topic.
   *
   * @param accountId The ID of the desired auto-renew account for the new topic.
   * @return A reference to this TopicCreateTransaction object with the newly-set auto-renew account ID.
   * @throws IllegalStateException If this TopicCreateTransaction is frozen.
   */
  TopicCreateTransaction& setAutoRenewAccountId(const AccountId& accountId);

  /**
   * Get the desired memo of the new topic.
   *
   * @return The desired memo of the new topic.
   */
  [[nodiscard]] inline std::string getMemo() const { return mMemo; }

  /**
   * Get the desired admin key for the new topic.
   *
   * @return The desired admin key for the new topic.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getAdminKey() const { return mAdminKey; }

  /**
   * Get the desired submit key for the new topic.
   *
   * @return The desired submit key for the new topic.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getSubmitKey() const { return mSubmitKey; }

  /**
   * Get the desired auto-renew period for the new topic.
   *
   * @return The desired auto-renew period for the new topic.
   */
  [[nodiscard]] inline std::chrono::duration<double> getAutoRenewPeriod() const { return mAutoRenewPeriod; }

  /**
   * Get the ID of the desired auto-renew account for the new topic.
   *
   * @return The ID of the desired auto-renew account for the new topic. Returns uninitialized if the auto-renew account
   *         has not been set.
   */
  [[nodiscard]] inline std::optional<AccountId> getAutoRenewAccountId() const { return mAutoRenewAccountId; }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TopicCreateTransaction object.
   *
   * @param client The Client trying to construct this TopicCreateTransaction.
   * @param node   The Node to which this TopicCreateTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TopicCreateTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this TopicCreateTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TopicCreateTransaction to a Node.
   *
   * @param client   The Client submitting this TopicCreateTransaction.
   * @param deadline The deadline for submitting this TopicCreateTransaction.
   * @param node     Pointer to the Node to which this TopicCreateTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;

  /**
   * Build a ConsensusCreateTopicTransactionBody protobuf object from this TopicCreateTransaction object.
   *
   * @return A pointer to a ConsensusCreateTopicTransactionBody protobuf object filled with this TopicCreateTransaction
   *         object's data.
   */
  [[nodiscard]] proto::ConsensusCreateTopicTransactionBody* build() const;

  /**
   * The publicly visible memo for the new topic.
   */
  std::string mMemo;

  /**
   * The key used for access control to update or delete the new topic. Anyone can increase the topic's mExpirationTime
   * via a TopicUpdateTransaction regardless of the mAdminKey. If no mAdminKey is specified, TopicUpdateTransaction may
   * only be used to extend the topic's mExpirationTime, and TopicDeleteTransaction is disallowed.
   */
  std::shared_ptr<Key> mAdminKey = nullptr;

  /**
   * The key that must sign to submit a message to the new topic (via a TopicMessageSubmitTransaction). If nullptr, all
   * message submissions are allowed on this topic.
   */
  std::shared_ptr<Key> mSubmitKey = nullptr;

  /**
   * The amount of time by which to attempt to extend the new topic's lifetime automatically at its expiration time.
   */
  std::chrono::duration<double> mAutoRenewPeriod = DEFAULT_AUTO_RENEW_PERIOD;

  /**
   * The account that should be charged to extend the lifetime of the new topic at its expiration time.
   */
  std::optional<AccountId> mAutoRenewAccountId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOPIC_CREATE_TRANSACTION_H_

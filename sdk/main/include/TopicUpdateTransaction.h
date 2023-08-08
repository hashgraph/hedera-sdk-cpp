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
#ifndef HEDERA_SDK_CPP_TOPIC_UPDATE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOPIC_UPDATE_TRANSACTION_H_

#include "AccountId.h"
#include "Key.h"
#include "TopicId.h"
#include "Transaction.h"

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace proto
{
class ConsensusUpdateTopicTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction that updates the properties of an existing topic. This includes the topic memo, admin key, submit key,
 * auto-renew account, and auto-renew period.
 *
 * Transaction Signing Requirements:
 *  - If an admin key is updated, the transaction must be signed by the pre-update admin key and post-update admin key.
 *  - If an admin key was set during the creation of the topic, the admin key must sign the transaction to update any
 *    of the topic's properties.
 *  - If no adminKey was defined during the creation of the topic, you can only extend the expirationTime.
 */
class TopicUpdateTransaction : public Transaction<TopicUpdateTransaction>
{
public:
  TopicUpdateTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a ConsensusUpdateTopic transaction.
   */
  explicit TopicUpdateTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the topic to update.
   *
   * @param topicId The ID of the topic to update.
   * @return A reference to this TopicUpdateTransaction object with the newly-set topic ID.
   * @throws IllegalStateException If this TopicUpdateTransaction is frozen.
   */
  TopicUpdateTransaction& setTopicId(const TopicId& topicId);

  /**
   * Set a new memo for the topic.
   *
   * @param memo The new memo for the topic.
   * @return A reference to this TopicUpdateTransaction object with the newly-set memo.
   * @throws IllegalStateException If this TopicUpdateTransaction is frozen.
   */
  TopicUpdateTransaction& setMemo(std::string_view memo);

  /**
   * Set a new expiration time for the topic.
   *
   * @param expiry The new expiration time for the topic.
   * @return A reference to this TopicUpdateTransaction object with the newly-set expiration time.
   * @throws IllegalStateException If this TopicUpdateTransaction is frozen.
   */
  TopicUpdateTransaction& setExpirationTime(const std::chrono::system_clock::time_point& expiry);

  /**
   * Set a new admin key for the topic.
   *
   * @param key The new admin key for the topic.
   * @return A reference to this TopicUpdateTransaction object with the newly-set admin key.
   * @throws IllegalStateException If this TopicUpdateTransaction is frozen.
   */
  TopicUpdateTransaction& setAdminKey(const std::shared_ptr<Key>& key);

  /**
   * Set a new submit key for the topic.
   *
   * @param key The new submit key for the topic.
   * @return A reference to this TopicUpdateTransaction object with the newly-set submit key.
   * @throws IllegalStateException If this TopicUpdateTransaction is frozen.
   */
  TopicUpdateTransaction& setSubmitKey(const std::shared_ptr<Key>& key);

  /**
   * Set a new auto-renew period for the topic.
   *
   * @param autoRenew The new auto-renew period for the topic.
   * @return A reference to this TopicUpdateTransaction object with the newly-set auto-renew period.
   * @throws IllegalStateException If this TopicUpdateTransaction is frozen.
   */
  TopicUpdateTransaction& setAutoRenewPeriod(const std::chrono::duration<double>& autoRenew);

  /**
   * Set the ID of a new auto-renew account for the topic.
   *
   * @param accountId The ID of the new auto-renew account for the topic.
   * @return A reference to this TopicUpdateTransaction object with the newly-set auto-renew account ID.
   * @throws IllegalStateException If this TopicUpdateTransaction is frozen.
   */
  TopicUpdateTransaction& setAutoRenewAccountId(const AccountId& accountId);

  /**
   * Clear the memo for the topic.
   *
   * @return A reference to this TokenUpdateTransaction with the newly-cleared memo.
   */
  TopicUpdateTransaction& clearTopicMemo();

  /**
   * Clear the admin key for the topic.
   *
   * @return A reference to this TokenUpdateTransaction with the newly-cleared admin key.
   */
  TopicUpdateTransaction& clearAdminKey();

  /**
   * Clear the submit key for the topic.
   *
   * @return A reference to this TokenUpdateTransaction with the newly-cleared submit key.
   */
  TopicUpdateTransaction& clearSubmitKey();

  /**
   * Clear the auto-renew account for the topic.
   *
   * @return A reference to this TokenUpdateTransaction with the newly-cleared auto-renew account ID.
   */
  TopicUpdateTransaction& clearAutoRenewAccountId();

  /**
   * Get the ID of the topic to update.
   *
   * @return The ID of the topic to update.
   */
  [[nodiscard]] inline TopicId getTopicId() const { return mTopicId; }

  /**
   * Get the new memo for the topic.
   *
   * @return The new memo for the topic. Returns uninitialized if the memo has not been set.
   */
  [[nodiscard]] inline std::optional<std::string> getMemo() const { return mMemo; }

  /**
   * Get the new expiration time for the topic.
   *
   * @return The new expiration time for the topic. Returns uninitialized if the expiration time has not been set.
   */
  [[nodiscard]] inline std::optional<std::chrono::system_clock::time_point> getExpirationTime() const
  {
    return mExpirationTime;
  }

  /**
   * Get the new admin key for the topic.
   *
   * @return The new admin key for the topic. Returns nullptr if the submit key has not been set.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getAdminKey() const { return mAdminKey; }

  /**
   * Get the new submit key for the topic.
   *
   * @return The new submit key for the topic. Returns nullptr if the submit key has not been set.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getSubmitKey() const { return mSubmitKey; }

  /**
   * Get the new auto-renew period for the topic.
   *
   * @return The new auto-renew period for the topic. Returns uninitialized if the auto-renew period has not been set.
   */
  [[nodiscard]] inline std::optional<std::chrono::duration<double>> getAutoRenewPeriod() const
  {
    return mAutoRenewPeriod;
  }

  /**
   * Get the ID of the new auto-renew account for the topic.
   *
   * @return The ID of the new auto-renew account for the topic. Returns uninitialized if the auto-renew account has not
   *         been set.
   */
  [[nodiscard]] inline std::optional<AccountId> getAutoRenewAccountId() const { return mAutoRenewAccountId; }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TopicUpdateTransaction object.
   *
   * @param client The Client trying to construct this TopicUpdateTransaction.
   * @param node   The Node to which this TopicUpdateTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TopicUpdateTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this TopicUpdateTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TopicUpdateTransaction to a Node.
   *
   * @param client   The Client submitting this TopicUpdateTransaction.
   * @param deadline The deadline for submitting this TopicUpdateTransaction.
   * @param node     Pointer to the Node to which this TopicUpdateTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;

  /**
   * Build a ConsensusUpdateTopicTransactionBody protobuf object from this TopicUpdateTransaction object.
   *
   * @return A pointer to a ConsensusUpdateTopicTransactionBody protobuf object filled with this TopicUpdateTransaction
   *         object's data.
   */
  [[nodiscard]] proto::ConsensusUpdateTopicTransactionBody* build() const;

  /**
   * The ID of the topic to update.
   */
  TopicId mTopicId;

  /**
   * The new publicly visible memo for the topic.
   */
  std::optional<std::string> mMemo;

  /**
   * The new expiration time for the topic.
   */
  std::optional<std::chrono::system_clock::time_point> mExpirationTime;

  /**
   * The new key to be used for access control to update or delete the topic.
   */
  std::shared_ptr<Key> mAdminKey = nullptr;

  /**
   * The new key that must sign to submit a message to the topic (via a TopicMessageSubmitTransaction).
   */
  std::shared_ptr<Key> mSubmitKey = nullptr;

  /**
   * The new amount of time by which to attempt to extend the topic's lifetime automatically at its expiration time.
   */
  std::optional<std::chrono::duration<double>> mAutoRenewPeriod;

  /**
   * The ID of the new account that should be charged to extend the lifetime of the topic at its expiration time.
   */
  std::optional<AccountId> mAutoRenewAccountId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOPIC_UPDATE_TRANSACTION_H_
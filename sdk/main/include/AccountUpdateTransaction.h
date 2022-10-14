/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#ifndef ACCOUNT_UDPATE_TRANSACTION_H_
#define ACCOUNT_UDPATE_TRANSACTION_H_

#include "AccountId.h"
#include "PublicKey.h"
#include "Transaction.h"

#include "helper/InitType.h"

#include <chrono>
#include <string>
#include <unordered_map>

namespace Hedera
{
class TransactionId;
}

namespace proto
{
class TransactionBody;
}

namespace Hedera
{
class AccountUpdateTransaction : public Transaction<AccountUpdateTransaction>
{
public:
  /**
   * Default constructor.
   */
  AccountUpdateTransaction();

  /**
   * Construct from a map of transaction ID's to their corresponding account
   * ID's and protobuf transactions.
   *
   * @param transactions Map of transaction IDs to their corresponding account
   *                     ID's and protobuf transactions.
   */
  explicit AccountUpdateTransaction(
    const std::unordered_map<TransactionId, std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Construct from a protobuf transaction object.
   *
   * @param transaction The protobuf transaction object from which to construct
   *                    this transaction.
   */
  explicit AccountUpdateTransaction(const proto::TransactionBody& transaction);

  /**
   * Sets the account ID for this account update.
   *
   * @param accountId The account ID for this account update.
   * @return          Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setAccountId(const AccountId& accountId);

  /**
   * Sets the proxy account ID for this account update.
   *
   * @param proxyAccountId The proxy account ID for this account update.
   * @return               Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setProxyAccountId(const AccountId& proxyAccountId);

  /**
   * Sets the key for this account update.
   *
   * @param key The key for this account update.
   * @return    Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setKey(const std::shared_ptr<PublicKey> key);

  /**
   * Sets the expiration time for this account update.
   *
   * @param expirationTime The expiration time for this account update.
   * @return               Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setExpirationTime(const std::chrono::nanoseconds& expirationTime);

  /**
   * Sets the auto renew period for this account update.
   *
   * @param autoRenewPeriod The auto renew period for this account update.
   * @return                Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setAutoRenewPeriod(const std::chrono::seconds& autoRenewPeriod);

  /**
   * Set that this account update is requiring receiver signatures.
   *
   * @param sigRequired \c TRUE if this account update should require receiver
   *                    signatures, otherwise \c FALSE.
   * @return            Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setReceiverSignatureRequired(bool sigRequired);

  /**
   * Sets the account memo for this account update.
   *
   * @param accountMemo The account memo for this account update.
   * @return            Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setAccountMemo(const std::string& accountMemo);

  /**
   * Sets the max automatic token associations for this account update.
   *
   * @param maxAssociations The max automatic token associations for this
   *                        account update.
   * @return                Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setMaxAutomaticTokenAssociations(int32_t maxAssociations);

  /**
   * Sets the staked account ID for this account update.
   *
   * @param stakedAccountId The staked account ID for this account update.
   * @return                Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setStakedAccountId(const AccountId& stakedAccountId);

  /**
   * Sets the staked node ID for this account update.
   *
   * @param stakedNodeId The staked node ID of this account update.
   * @return             Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setStakedNodeId(const int64_t& stakedNodeId);

  /**
   * Set that this account update is decling staking rewards.
   *
   * @param declineStakingReward \c TRUE if this account update should decline
   *                             staking rewards, otherwise \c FALSE.
   * @return Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setDeclineStakingReward(bool declineStakingReward);

  /**
   * Extracts the account ID of this account update.
   *
   * @return The account ID of this account update. Invalid if not set.
   */
  inline InitType<AccountId> getAccountId() const { return mAccountId; }

  /**
   * @brief Extract the key of this account update.
   *
   * @return The key of this account update. Invalid if not set.
   */
  inline std::shared_ptr<PublicKey> getKey() const { return mKey; }

  /**
   * @brief Extract the expiration time of this account update.
   *
   * @return The expiration time of this account update. Invalid if not set.
   */
  inline InitType<std::chrono::nanoseconds> getExpirationTime() const { return mExpirationTime; }

  /**
   * @brief Extract the auto renew period of this account update.
   *
   * @return The auto renew period of this account update. Invalid if not set.
   */
  inline InitType<std::chrono::seconds> getAutoRenewPeriod() const { return mAutoRenewPeriod; }

  /**
   * @brief Extract the account memo of this account update.
   *
   * @return The account memo of this account update. Invalid if not set.
   */
  inline InitType<std::string> getAccountMemo() const { return mAccountMemo; }

  /**
   * @brief Extract the max automatic token associations of this account update.
   *
   * @return The max automatic token associations of this account update.
   * Invalid if not set.
   */
  inline InitType<int32_t> getMaxAutomaticTokenAssociations() const { return mMaxAutomaticTokenAssociations; }

  /**
   * @brief Extract the staked account ID of this account update.
   *
   * @return The staked account ID of this account update. Invalid if not set.
   */
  inline InitType<AccountId> getStakedAccountId() const { return mStakedAccountId; }

  /**
   * @brief Extract the staked node ID of this account update.
   *
   * @return The staked node ID of this account update. Invalid if not set.
   */
  inline InitType<int64_t> getStakedNodeId() const { return mStakedNodeId; }

  /**
   * @brief Determine if this account update is declining staking rewards.
   *
   * @return \c TRUE if this account update is declining staking rewards,
   *         otherwise \c FALSE. Invalid if not set.
   */
  inline InitType<bool> getDeclineStakingReward() const { return mDeclineStakingReward; }

private:
  /**
   * Initialize from the transaction body.
   */
  void initFromTransactionBody();

  InitType<AccountId> mAccountId;
  std::shared_ptr<PublicKey> mKey;
  InitType<std::chrono::nanoseconds> mExpirationTime;
  InitType<std::chrono::seconds> mAutoRenewPeriod;
  InitType<std::string> mAccountMemo;
  InitType<int32_t> mMaxAutomaticTokenAssociations;
  InitType<AccountId> mStakedAccountId;
  InitType<int64_t> mStakedNodeId;
  InitType<bool> mDeclineStakingReward;
};

} // namespace Hedera

#endif // ACCOUNT_UDPATE_TRANSACTION_H_
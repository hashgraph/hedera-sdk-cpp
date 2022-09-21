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

/**
 * Library includes
 */
#include "AccountId.h"
#include "Key.h"
#include "Transaction.h"
#include "helper/InitType.h"

/**
 * STL includes
 */
#include <chrono>
#include <string>
#include <unordered_map>

/**
 * Hedera forward declarations
 */
namespace Hedera
{
class TransactionId;
} // namespace Hedera

/**
 * Protobuf forward declarations
 */
namespace proto
{
class TransactionBody;
} // namespace proto

namespace Hedera
{
class AccountUpdateTransaction : public Transaction<AccountUpdateTransaction>
{
public:
  /**
   * Constructor.
   */
  AccountUpdateTransaction();

  /**
   * Construct from a map of TransactionId's to their corresponding AccountId's
   * and Transactions
   *
   * @param transactions  Compound list of transaction ID's list of (AccountId,
   *                      TransactionBody) records
   */
  AccountUpdateTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Constructor from a TransactionBody object
   *
   * @param transaction  protobuf TransactionBody
   */
  AccountUpdateTransaction(const proto::TransactionBody& transaction);

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
  AccountUpdateTransaction& setKey(const Key& key);

  /**
   * Sets the expiration time for this account update.
   *
   * @param expirationTime The expiration time for this account update.
   * @return               Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setExpirationTime(
    const std::chrono::nanoseconds& expirationTime);

  /**
   * Sets the auto renew period for this account update.
   *
   * @param autoRenewPeriod The auto renew period for this account update.
   * @return                Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setAutoRenewPeriod(
    const std::chrono::days& autoRenewPeriod);

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
  AccountUpdateTransaction& setMaxAutomaticTokenAssociations(
    int maxAssociations);

  /**
   * Sets the staked account ID for this account update.
   *
   * @param stakedAccountId The staked account ID for this account update.
   * @return                Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setStakedAccountId(
    const AccountId& stakedAccountId);

  /**
   * Sets the staked node ID for this account update.
   *
   * @param stakedNodeId The staked node ID of this account update.
   * @return             Reference to this AccountUpdateTransaction object.
   */
  AccountUpdateTransaction& setStakedNodeId(const long long& stakedNodeId);

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
  inline InitType<Key> getKey() const { return mKey; }

  /**
   * @brief Extract the expiration time of this account update.
   *
   * @return The expiration time of this account update. Invalid if not set.
   */
  inline InitType<std::chrono::nanoseconds> getExpirationTime() const
  {
    return mExpirationTime;
  }

  /**
   * @brief Extract the auto renew period of this account update.
   *
   * @return The auto renew period of this account update. Invalid if not set.
   */
  inline InitType<std::chrono::days> getAutoRenewPeriod() const
  {
    return mAutoRenewPeriod;
  }

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
  inline InitType<int> getMaxAutomaticTokenAssociations() const
  {
    return mMaxAutomaticTokenAssociations;
  }

  /**
   * @brief Extract the staked account ID of this account update.
   *
   * @return The staked account ID of this account update. Invalid if not set.
   */
  inline InitType<AccountId> getStakedAccountId() const
  {
    return mStakedAccountId;
  }

  /**
   * @brief Extract the staked node ID of this account update.
   *
   * @return The staked node ID of this account update. Invalid if not set.
   */
  inline InitType<long long> getStakedNodeId() const { return mStakedNodeId; }

  /**
   * @brief Determine if this account update is declining staking rewards.
   *
   * @return \c TRUE if this account update is declining staking rewards,
   *         otherwise \c FALSE. Invalid if not set.
   */
  inline InitType<bool> getDeclineStakingReward() const
  {
    return mDeclineStakingReward;
  }

private:
  /**
   * Initialize from the transaction body.
   */
  void initFromTransactionBody();

  InitType<AccountId> mAccountId;
  InitType<Key> mKey;
  InitType<std::chrono::nanoseconds> mExpirationTime;
  InitType<std::chrono::days> mAutoRenewPeriod;
  InitType<std::string> mAccountMemo;
  InitType<int> mMaxAutomaticTokenAssociations;
  InitType<AccountId> mStakedAccountId;
  InitType<long long> mStakedNodeId;
  InitType<bool> mDeclineStakingReward;
};
} // namespace Hedera

#endif // ACCOUNT_UDPATE_TRANSACTION_H_
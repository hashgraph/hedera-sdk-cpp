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
#ifndef CONTRACT_UPDATE_TRANSACTION_H_
#define CONTRACT_UPDATE_TRANSACTION_H_

#include "AccountId.h"
#include "ContractId.h"
#include "Key.h"
#include "Transaction.h"

#include "helper/InitType.h"

#include <chrono>
#include <unordered_map>

namespace Hedera
{
class AccountId;
class Client;
class TransactionId;
}

namespace proto
{
class ContractUpdateTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * At consensus, updates the fields of a smart contract to the given values.
 *
 * If no value is given for a field, that field is left unchanged on the
 * contract. For an immutable smart contract (that is, a contract created
 * without an adminKey), only the expirationTime may be updated; setting any
 * other field in this case will cause the transaction status to resolve to
 * MODIFYING_IMMUTABLE_CONTRACT.
 *
 * --- Signing Requirements ---
 * 1. Whether or not a contract has an admin key, its expiry can be extended
 *    with only the transaction payer's signature.
 * 2. Updating any other field of a mutable contract requires the admin key's
 *    signature.
 * 3. If the update transaction includes a new admin key, this new key must also
 *    sign unless it is exactly an empty KeyList. This special sentinel key
 *    removes the existing admin key and causes the contract to become
 *    immutable. (Other Key structures without a constituent Ed25519 key will be
 *    rejected with INVALID_ADMIN_KEY)
 * 4. If the update transaction sets the AccountID auto_renew_account_id wrapper
 *    field to anything other than the sentinel 0.0.0 value, then the key of the
 *    referenced account must sign.
 */
class ContractUpdateTransaction : public Transaction<ContractUpdateTransaction>
{
  /**
   * Constructor.
   */
  ContractUpdateTransaction();

  /**
   * Construct from a map of transaction ID's to their corresponding account
   * ID's and protobuf transactions.
   *
   * @param transactions Map of transaction IDs to their corresponding account
   *                     ID's and protobuf transactions.
   */
  explicit ContractUpdateTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Construct from a protobuf transaction object.
   *
   * @param transaction The protobuf transaction object from which to construct
   *                    this transaction.
   */
  explicit ContractUpdateTransaction(const proto::TransactionBody& transaction);

  /**
   * Derived from Transaction. Validate the checksums.
   *
   * @param client The client with which to validate the checksums.
   */
  virtual void validateChecksums(const Client& client) const override;

  /**
   * Build a contract update transaction protobuf message based on the data in
   * this class.
   *
   * @return A contract update transaction protobuf message.
   */
  proto::ContractUpdateTransactionBody build() const;

  /**
   * Sets the contract ID instance to update.
   *
   * @param contractId The contract ID to be set.
   * @return Reference to this ContractUpdateTransaction.
   */
  ContractUpdateTransaction& setContractId(const ContractId& contractId);

  /**
   * Sets the expiration time.
   *
   * @param expirationTime The expiration time to be set.
   * @return Reference to this ContractUpdateTransaction.
   */
  ContractUpdateTransaction& setExpirationTime(
    const std::chrono::nanoseconds& expirationTime);

  /**
   * Sets the admin key.
   *
   * @param adminKey The admin key to be set.
   * @return Reference to this ContractUpdateTransaction.
   */
  ContractUpdateTransaction& setAdminKey(const Key& adminKey);

  /**
   * Sets the auto-renew period.
   *
   * @param autoRenewPeriod The auto-renew period to be set.
   * @return Reference to this ContractUpdateTransaction.
   */
  ContractUpdateTransaction& setAutoRenewPeriod(
    const std::chrono::seconds& autoRenewPeriod);

  /**
   * Sets the memo.
   *
   * @param memo The memo to be set.
   * @return Reference to this ContractUpdateTransaction.
   */
  ContractUpdateTransaction& setMemo(const std::string& memo);

  /**
   * Sets the max automatic token associations.
   *
   * @param maxAutomaticTokenAssociations The max automatic token associations
   *                                      to be set.
   * @return Reference to this ContractUpdateTransaction.
   */
  ContractUpdateTransaction& setMaxAutomaticTokenAssociations(
    int32_t maxAutomaticTokenAssociations);

  /**
   * Sets the auto-renew account ID.
   *
   * @param autoRenewAccountId The auto-renew account ID to be set.
   * @return Reference to this ContractUpdateTransaction.
   */
  ContractUpdateTransaction& setAutoRenewAccountId(
    const AccountId& autoRenewAccountId);

  /**
   * Sets the staked account ID.
   *
   * @param stakedAccountId The staked account ID to be set.
   * @return Reference to this ContractUpdateTransaction.
   */
  ContractUpdateTransaction& setStakedAccountId(
    const AccountId& stakedAccountId);

  /**
   * Sets the staked node ID.
   *
   * @param stakedNodeId The staked node ID to be set.
   * @return Reference to this ContractUpdateTransaction.
   */
  ContractUpdateTransaction& setStakedNodeId(const int64_t& stakedNodeId);

  /**
   * Sets the staking rewards policy.
   *
   * @param declineRewards \c TRUE to decline rewards, otherwise \c FALSE.
   * @return Reference to this ContractUpdateTransaction.
   */
  ContractUpdateTransaction& setDeclineRewards(bool declineRewards);

  /**
   * Extract the contract ID.
   *
   * @return The contract ID.
   */
  inline InitType<ContractId> getContractId() { return mContractId; }

  /**
   * Extract the expiration time.
   *
   * @return The expiration time.
   */
  inline InitType<std::chrono::nanoseconds> getExpirationTime()
  {
    return mExpirationTime;
  }

  /**
   * Extract the admin key.
   *
   * @return The admin key.
   */
  inline InitType<Key> getAdminKey() { return mAdminKey; }

  /**
   * Extract the auto-renew period.
   *
   * @return The auto-renew period.
   */
  inline InitType<std::chrono::seconds> getAutoRenewPeriod()
  {
    return mAutoRenewPeriod;
  }

  /**
   * Extract the memo.
   *
   * @return The memo.
   */
  inline InitType<std::string> getMemo() { return mMemo; }

  /**
   * Extract the max automatic token associations.
   *
   * @return The max automatic token associations.
   */
  inline InitType<int32_t> getMaxAutomaticTokenAssociations()
  {
    return mMaxAutomaticTokenAssociations;
  }

  /**
   * Extract the auto-renew account ID.
   *
   * @return The auto-renew account ID.
   */
  inline InitType<AccountId> getAutoRenewAccountId()
  {
    return mAutoRenewAccountId;
  }

  /**
   * Extract the staked account ID.
   *
   * @return The staked account ID.
   */
  inline InitType<AccountId> getStakedAccountId() { return mStakedAccountId; }

  /**
   * Extract the staked node ID.
   *
   * @return The staked node ID.
   */
  inline InitType<int64_t> getStakedNodeId() { return mStakedNodeId; }

  /**
   * Extract the staking rewards policy.
   *
   * @return \c TRUE if this update is set to decline rewards, otherwise
   *         \c FALSE.
   */
  inline InitType<bool> getDeclineRewards() { return mDeclineReward; }

private:
  /**
   * Initialize from the transaction body.
   */
  void initFromTransactionBody();

  /**
   * The ID of the contract to be updated.
   */
  InitType<ContractId> mContractId;

  /**
   * The new expiry of the contract, no earlier than the current expiry
   * (resolves to EXPIRATION_REDUCTION_NOT_ALLOWED otherwise).
   */
  InitType<std::chrono::nanoseconds> mExpirationTime;

  /**
   * The new key to control updates to the contract.
   */
  InitType<Key> mAdminKey;

  /**
   * (NOT YET IMPLEMENTED) The new interval at which the contract will pay to
   * extend its expiry (by the same interval).
   */
  InitType<std::chrono::seconds> mAutoRenewPeriod;

  /**
   * The new contract memo, assumed to be Unicode encoded with UTF-8 (at most
   * 100 bytes)
   */
  InitType<std::string> mMemo;

  /**
   * If set, the new maximum number of tokens that this contract can be
   * automatically associated with (i.e., receive air-drops from).
   */
  InitType<int32_t> mMaxAutomaticTokenAssociations;

  /**
   * If set to the sentinel 0.0.0 account ID, this field removes the contract's
   * auto-renew account. Otherwise it updates the contract's auto-renew account
   * to the referenced account.
   */
  InitType<AccountId> mAutoRenewAccountId;

  /**
   * The ID of the new account to which this contract is staking. If set to the
   * sentinel 0.0.0 account ID, this field removes the contract's staked account
   * ID. Mutually exclusive from mStakedNodeId.
   */
  InitType<AccountId> mStakedAccountId;

  /**
   * The ID of the new node this contract is staked to. If set to the sentinel
   * -1, this field removes the contract's staked node ID.
   */
  InitType<int64_t> mStakedNodeId;

  /**
   * If true, the contract declines receiving a staking reward.
   */
  InitType<bool> mDeclineReward;
};

} // namespace Hedera

#endif // CONTRACT_UPDATE_TRANSACTION_H_
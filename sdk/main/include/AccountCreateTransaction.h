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
#ifndef ACCOUNT_CREATE_TRANSACTION_H_
#define ACCOUNT_CREATE_TRANSACTION_H_

#include "AccountId.h"
#include "Hbar.h"
#include "PublicKey.h"
#include "Transaction.h"

#include <chrono>
#include <optional>

namespace Hedera
{
class TransactionResponse;
}

namespace proto
{
class CryptoCreateTransactionBody;
class Transaction;
class TransactionResponse;
}

namespace Hedera
{
/**
 * Create a new Hedera account.
 */
class AccountCreateTransaction : public Transaction<AccountCreateTransaction>
{
public:
  /**
   * Constructor.
   */
  AccountCreateTransaction();

  /**
   * Set the key for this account. The key that must sign each transfer out of the account. If
   * mReceiverSignatureRequired is true, then it must also sign any transfer into the account.
   *
   * @param key The public key for this account.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setKey(const std::shared_ptr<PublicKey>& key);

  /**
   * Set the initial amount to transfer into this account.
   *
   * @param initialBalance The initial balance to transfer.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setInitialBalance(const Hbar& initialBalance);

  /**
   * Set to true to require this account to sign any transfer of Hbars into this account. All transfers of Hbars from
   * this account must always be signed. This property only affects transfers to this account.
   *
   * @param receiveSignatureRequired \c TRUE to require a signature when receiving Hbars, otherwise \c FALSE.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setReceiverSignatureRequired(bool receiveSignatureRequired);

  /**
   * Set the auto renew period for this account. A Hedera account is charged to extend its expiration date every renew
   * period. If it doesn't have enough balance, it extends as long as possible. If the balance is zero when it expires,
   * then the account is deleted.
   *
   * @param autoRenewPeriod The auto renew period to set.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setAutoRenewPeriod(const std::chrono::duration<double>& autoRenewPeriod);

  /**
   * Assign a memo to the account.
   *
   * @param memo The memo to set.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setAccountMemo(std::string_view memo);

  /**
   * Set the maximum automatic token associations.
   *
   * @param associations The maximum amount of associations to set.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setMaxAutomaticTokenAssociations(int32_t associations);

  /**
   * Set the account to which this account will stake. This will reset the value of the staked node ID if it is set.
   *
   * @param stakedAccountId The ID of the account to which this account will stake.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setStakedAccountId(const AccountId& stakedAccountId);

  /**
   * Set the node to which this account will stake. This will reset the value of the staked account ID if it is set.
   *
   * @param stakedNodeId The ID of the node to which this account will stake.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setStakedNodeId(const int64_t& stakedNodeId);

  /**
   * Set this account's staking reward reception policy.
   *
   * @param declineStakingReward \c TRUE if this account should decline receiving staking rewards, otherwise \c FALSE.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setDeclineStakingReward(bool declineStakingReward);

  /**
   * The key to be used as this account's alias. Currently, only primitive keys and EVM addresses are supported.
   * ThresholdKey, KeyList, ContractID, and delegatable_contract_id are not supported.
   *
   * An EVM address may be either the encoded form of the shard.realm.num, or the keccak-256 hash of a ECDSA_SECP256K1
   * primitive key.
   *
   * A given alias can map to at most one account on the network at a time. This uniqueness will be
   * enforced relative to aliases currently on the network at alias assignment.
   *
   * If a transaction creates an account using an alias, any further crypto transfers to that alias will simply be
   * deposited in that account, without creating anything, and with no creation fee being charged.
   *
   * @param alias The key to be used as the account's alias.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setAlias(const std::shared_ptr<PublicKey>& alias);

  /**
   * Extract the key for this account.
   *
   * @return The key for this account.
   */
  inline std::shared_ptr<PublicKey> getKey() const { return mPublicKey; }

  /**
   * Extract the initial balance to transfer into the account.
   *
   * @return The initial balance to transfer into new account.
   */
  inline Hbar getInitialBalance() const { return mInitialBalance; }

  /**
   * Determine if the receiver is required to sign.
   *
   * @return \c TRUE if the receiver is required to sign, otherwise \c FALSE.
   */
  inline bool getReceiverSignatureRequired() const { return mReceiverSignatureRequired; }

  /**
   * Extract the duration for the auto renew period.
   *
   * @return The duration for the auto renew period.
   */
  inline std::optional<std::chrono::duration<double>> getAutoRenewPeriod() const { return mAutoRenewPeriod; }

  /**
   * Extract the account memo.
   *
   * @return The account memo.
   */
  inline std::string getAccountMemo() const { return mAccountMemo; }

  /**
   * Extract the maximum automatic token associations.
   *
   * @return The maximum automatic token associations.
   */
  inline int32_t getMaxAutomaticTokenAssociations() const { return mMaxAutomaticTokenAssociations; }

  /**
   * Extract the ID of the account to which this account will stake.
   *
   * @return ID of the account to which this account will stake.
   */
  inline std::optional<AccountId> getStakedAccountId() const { return mStakedAccountId; }

  /**
   * Extract the ID of the node to which this account will stake.
   *
   * @return ID of the node to which this account will stake.
   */
  inline std::optional<int64_t> getStakedNodeId() const { return mStakedNodeId; }

  /**
   * Extract if this account declines receiving staking rewards.
   *
   * @return \c TRUE if this account declines receiving a staking reward,
   *         otherwise \c FALSE
   */
  inline bool getDeclineStakingReward() const { return mDeclineStakingReward; }

  /**
   * Extract the key to be used as this account's alias.
   *
   * @return The key to be used as this account's alias.
   */
  inline std::shared_ptr<PublicKey> getAlias() { return mAlias; }

protected:
  /**
   * Derived from Executable. Construct a protobuf Transaction from this AccountCreateTransaction.
   *
   * @param client The Client submitting this transaction.
   * @return A protobuf Transaction that contains this AccountCreateTransaction's data and is signed by the client.
   */
  proto::Transaction makeRequest(const Client& client) const override;

private:
  /**
   * Build a CryptoCreateTransactionBody protobuf message based on the this class data.
   *
   * @return A CryptoCreateTransactionBody protobuf message.
   */
  proto::CryptoCreateTransactionBody* build() const;

  /**
   * The key that must sign each transfer out of the account. If mReceiverSignatureRequired is true, then it must also
   * sign any transfer into the account. Defaults to uninitialized.
   */
  std::shared_ptr<PublicKey> mPublicKey;

  /**
   * The initial amount to transfer into this account. Defaults to 0 Hbar.
   */
  Hbar mInitialBalance = Hbar(0LL);

  /**
   * \c TRUE if a signature is required for transfers into this account, otherwise \c FALSE. Defaults to \c FALSE.
   */
  bool mReceiverSignatureRequired = false;

  /**
   * A Hedera account is charged to extend its expiration date every renew period. If it doesn't have enough balance, it
   * extends as long as possible. If the balance is zero when it expires, then the account is deleted. Defaults to 3
   * months.
   */
  std::optional<std::chrono::duration<double>> mAutoRenewPeriod = std::chrono::months(3);

  /**
   * A memo for this account. Defaults to empty.
   */
  std::string mAccountMemo;

  /**
   * The maximum number of token associations for this account. Defaults to 0.
   */
  int32_t mMaxAutomaticTokenAssociations = 0;

  /**
   * The account to which this account is staked. Mutually exclusive with mStakedNodeId. Defaults to uninitialized.
   */
  std::optional<AccountId> mStakedAccountId;

  /**
   * The node to which this account is staked. Mutually exclusive with mStakedAccountId. Defaults to uninitialized.
   */
  std::optional<int64_t> mStakedNodeId;

  /**
   * \c TRUE if the account should decline staking rewards, otherwise \c FALSE. Defaults to \c FALSE.
   */
  bool mDeclineStakingReward = false;

  /**
   * The native key, or 20-byte EVM address, to be used as this account's alias. Defaults to uninitialized.
   */
  std::shared_ptr<PublicKey> mAlias;
};

} // namespace Hedera

#endif // ACCOUNT_CREATE_TRANSACTION_H_
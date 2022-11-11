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
class Channel;
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
 * The transaction used to create a new account. After the account is created, the AccountID for it is in the receipt.
 * It can also be retrieved with a GetByKey query. Threshold values can be defined, and records are generated and stored
 * for 25 hours for any transfer that exceeds the thresholds. This account is charged for each record generated, so the
 * thresholds are useful for limiting record generation to happen only for large transactions.
 *
 * The mPublicKey field is the key used to sign transactions for this account. If the account has
 * mReceiverSignatureRequired set to true, then all cryptocurrency transfers must be signed by this account's key, both
 * for transfers in and out. If it is false, then only transfers out have to be signed by it. When the account is
 * created, the payer account is charged enough Hbars so that the new account will not expire for the next
 * mAutoRenewPeriod seconds. When it reaches the expiration time, the new account will then be automatically charged to
 * renew for another mAutoRenewPeriod seconds. If it does not have enough Hbars to renew for that long, then the
 * remaining Hbars are used to extend its expiration as long as possible. If it is has a zero balance when it expires,
 * then it is deleted. This transaction must be signed by the payer account. If the mAutoRenewPeriod field is set, the
 * key of the referenced account must sign.
 *
 * An account must be created in a particular realm. If the realmID is left null, then a new realm will be created with
 * the given admin key. If a new realm has a null adminKey, then anyone can create/modify/delete entities in that realm.
 * But if an admin key is given, then any transaction to create/modify/delete an entity in that realm must be signed by
 * that key, though anyone can still call functions on smart contract instances that exist in that realm. A realm ceases
 * to exist when everything within it has expired and no longer exists.
 *
 * The current API ignores shardID, realmID, and newRealmAdminKey, and creates everything in shard 0 and realm 0, with a
 * null key. Future versions of the API will support multiple realms and multiple shards.
 */
class AccountCreateTransaction : public Transaction<AccountCreateTransaction>
{
public:
  /**
   * Constructor.
   */
  AccountCreateTransaction();

  /**
   * Default destructor.
   */
  ~AccountCreateTransaction() override = default;

  /**
   * Set the public key for the new account. The key that must sign each transfer out of the account. If
   * mReceiverSignatureRequired is true, then it must also sign any transfer into the account.
   *
   * @param publicKey The desired public key for the new account.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setKey(const std::shared_ptr<PublicKey>& publicKey);

  /**
   * Set the initial amount to transfer into the new account from the paying account.
   *
   * @param initialBalance The desired balance to transfer into the new account.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setInitialBalance(const Hbar& initialBalance);

  /**
   * Set to true to require the new account to sign any transfer of Hbars into itself. All transfers of Hbars from the
   * account must always be signed. This property only affects transfers to the account.
   *
   * @param receiveSignatureRequired \c TRUE to require the new account to sign any Hbar transfer transactions that
   *                                 involve transferring Hbars into itself, otherwise \c FALSE.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setReceiverSignatureRequired(bool receiveSignatureRequired);

  /**
   * Set the auto renew period for the new account. A Hedera account is charged to extend its expiration date every
   * renew period. If it doesn't have enough balance, it extends as long as possible. If the balance is zero when it
   * expires, then the account is deleted.
   *
   * @param autoRenewPeriod The desired auto renew period for the new account.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setAutoRenewPeriod(const std::chrono::duration<int64_t>& autoRenewPeriod);

  /**
   * Assign a memo to the new account.
   *
   * @param memo The desired memo for the new account.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setAccountMemo(std::string_view memo);

  /**
   * Set the maximum automatic token associations the new account can have.
   *
   * @param associations The desired maximum amount of associations for the new account.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setMaxAutomaticTokenAssociations(uint32_t associations);

  /**
   * Set the account to which the new account will stake. This is mutually exclusive with mStakedNodeId, and will
   * reset the value of the mStakedNodeId if it is set.
   *
   * @param stakedAccountId The ID of the desired account to which the new account will stake.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setStakedAccountId(const AccountId& stakedAccountId);

  /**
   * Set the node to which the new account will stake. This is mutually exclusive with mStakedAccountId, and will
   * reset the value of the mStakedAccountId if it is set.
   *
   * @param stakedNodeId The ID of the desired node to which the new account will stake.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setStakedNodeId(const uint64_t& stakedNodeId);

  /**
   * Set the account's staking reward reception policy.
   *
   * @param declineReward \c TRUE if the new account should decline receiving staking rewards, otherwise \c FALSE.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setDeclineStakingReward(bool declineReward);

  /**
   * Set the key to be used as the new account's alias. Currently, only primitive keys and EVM addresses are supported.
   * ThresholdKey, KeyList, ContractID, and delegatable_contract_id are not supported. An EVM address may be either the
   * encoded form of the <shard>.<realm>.<num>, or the keccak-256 hash of a ECDSA_SECP256K1 primitive key.
   *
   * A given alias can map to at most one account on the network at a time. This uniqueness will be
   * enforced relative to aliases currently on the network at alias assignment.
   *
   * If a transaction creates an account using an alias, any further crypto transfers to that alias will simply be
   * deposited in that account, without creating anything, and with no creation fee being charged.
   *
   * @param alias The desired key to be used as the account's alias.
   * @return Reference to this AccountCreateTransaction object.
   */
  AccountCreateTransaction& setAlias(const std::shared_ptr<PublicKey>& alias);

  /**
   * Extract the desired public key for the new account.
   *
   * @return The desired public key for the new account.
   */
  [[nodiscard]] inline std::shared_ptr<PublicKey> getKey() const { return mKey; }

  /**
   * Extract the desired initial balance to transfer into the new account from the paying account.
   *
   * @return The desired initial balance to transfer into the new account from the paying account.
   */
  [[nodiscard]] inline Hbar getInitialBalance() const { return mInitialBalance; }

  /**
   * Extract the desired Hbar transfer receiver signature policy for the new account.
   *
   * @return \c TRUE if the new account will be required to sign all incoming Hbar transfers, otherwise \c FALSE.
   */
  [[nodiscard]] inline bool getReceiverSignatureRequired() const { return mReceiverSignatureRequired; }

  /**
   * Extract the desired auto renew period for the new account.
   *
   * @return The desired auto renew period for the new account.
   */
  [[nodiscard]] inline std::optional<std::chrono::duration<int64_t>> getAutoRenewPeriod() const
  {
    return mAutoRenewPeriod;
  }

  /**
   * Extract the desired memo for the new account.
   *
   * @return The desired memo for the new account.
   */
  [[nodiscard]] inline std::string getAccountMemo() const { return mAccountMemo; }

  /**
   * Extract the desired maximum automatic token associations for the new account.
   *
   * @return The desired maximum automatic token associations for the new account.
   */
  [[nodiscard]] inline uint32_t getMaxAutomaticTokenAssociations() const { return mMaxAutomaticTokenAssociations; }

  /**
   * Extract the ID of the desired account to which the new account will stake.
   *
   * @return The ID of the desired account to which the new account will stake.
   */
  [[nodiscard]] inline std::unique_ptr<AccountId> getStakedAccountId() const
  {
    return mStakedAccountId ? std::make_unique<AccountId>(*mStakedAccountId) : std::unique_ptr<AccountId>();
  }

  /**
   * Extract the ID of the desired node to which the new account will stake.
   *
   * @return The ID of the desired node to which the new account will stake.
   */
  [[nodiscard]] inline std::unique_ptr<uint64_t> getStakedNodeId() const
  {
    return mStakedNodeId ? std::make_unique<uint64_t>(*mStakedNodeId) : std::unique_ptr<uint64_t>();
  }

  /**
   * Extract the desired staking rewards reception policy for the new account.
   *
   * @return \c TRUE if the new account will decline from receiving staking rewards, otherwise \c FALSE
   */
  [[nodiscard]] inline bool getDeclineStakingReward() const { return mDeclineStakingReward; }

  /**
   * Extract the desired key to be used as the new account's alias.
   *
   * @return The desired key to be used as the new account's alias.
   */
  [[nodiscard]] inline std::shared_ptr<PublicKey> getAlias() const { return mAlias; }

protected:
  /**
   * Derived from Executable. Construct a protobuf Transaction from this AccountCreateTransaction.
   *
   * @param client The Client submitting this transaction.
   * @return A protobuf Transaction that contains this AccountCreateTransaction's data and is signed by the client.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client, const std::shared_ptr<Node>&) const override;

  /**
   * Derived from Executable. Get the gRPC method to call to create a new crypto account.
   *
   * @param node The Node from which to retrieve the function.
   * @return The gRPC method to call to execute this AccountCreateTransaction.
   */
  [[nodiscard]] std::function<
    grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
  getGrpcMethod(const std::shared_ptr<Node>& node) const override;

private:
  /**
   * Build a CryptoCreateTransactionBody protobuf message based on the this class data.
   *
   * @return A CryptoCreateTransactionBody protobuf message.
   */
  [[nodiscard]] proto::CryptoCreateTransactionBody* build() const;

  /**
   * The key that must sign each transfer out of the account. If mReceiverSignatureRequired is \c TRUE, then it must
   * also sign any transfer into the account. Defaults to uninitialized.
   */
  std::shared_ptr<PublicKey> mKey;

  /**
   * The initial amount to transfer into the new account. Defaults to 0 Hbar.
   */
  Hbar mInitialBalance = Hbar(0LL);

  /**
   * If \c TRUE, the new account's key must sign any transaction being deposited into it (in addition to all
   * withdrawals). Defaults to \c FALSE.
   */
  bool mReceiverSignatureRequired = false;

  /**
   * A Hedera account is charged to extend its expiration date every renew period. If it doesn't have enough balance, it
   * extends as long as possible. If the balance is zero when it expires, then the account is deleted. Defaults to 3
   * months.
   */
  std::chrono::duration<int64_t> mAutoRenewPeriod = std::chrono::months(3);

  /**
   * The memo to be associated with the account (UTF-8 encoding max 100 bytes). Defaults to empty.
   */
  std::string mAccountMemo;

  /**
   * The maximum number of tokens with which the new account can be implicitly associated. Defaults to 0 and up to a
   * maximum value of 1000 (attempting to set a value >1000 will just set the value to 1000).
   */
  uint32_t mMaxAutomaticTokenAssociations = 0U;

  /**
   * The ID of the account to which the new account will be staked. Mutually exclusive with mStakedNodeId. Defaults to
   * uninitialized.
   */
  std::unique_ptr<AccountId> mStakedAccountId;

  /**
   * The ID of the node to which the new account will be staked. Mutually exclusive with mStakedAccountId. Defaults to
   * uninitialized.
   */
  std::unique_ptr<uint64_t> mStakedNodeId;

  /**
   * If \c TRUE, the new account will decline receiving staking rewards. Defaults to \c FALSE.
   */
  bool mDeclineStakingReward = false;

  /**
   * The public key to be used as the new account's alias. The bytes will be 1 of 2 options. It will be the
   * serialization of a protobuf Key message for an ED25519/ECDSA_SECP256K1 primitive key type. If the account is
   * ECDSA_SECP256K1 based it may also be the public address, calculated as the last 20 bytes of the keccak-256 hash of
   * the ECDSA_SECP256K1 primitive key. Currently only primitive key bytes are supported as the key for an account with
   * an alias. ThresholdKey, KeyList, ContractID, and delegatable_contract_id are not supported.
   *
   * A given alias can map to at most one account on the network at a time. This uniqueness will be enforced relative to
   * aliases currently on the network at alias assignment.
   *
   * If a transaction creates an account using an alias, any further crypto transfers to that alias will
   * simply be deposited in that account, without creating anything, and with no creation fee being charged.
   */
  std::shared_ptr<PublicKey> mAlias;
};

} // namespace Hedera

#endif // ACCOUNT_CREATE_TRANSACTION_H_
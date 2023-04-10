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
#ifndef HEDERA_SDK_CPP_ACCOUNT_CREATE_TRANSACTION_H_
#define HEDERA_SDK_CPP_ACCOUNT_CREATE_TRANSACTION_H_

#include "AccountId.h"
#include "EvmAddress.h"
#include "Hbar.h"
#include "PublicKey.h"
#include "Transaction.h"

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace proto
{
class CryptoCreateTransactionBody;
class TransactionBody;
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
   * Default constructor. Sets the maximum transaction fee to 5 Hbars.
   */
  AccountCreateTransaction();

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a CryptoCreateAccount transaction.
   */
  explicit AccountCreateTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the public key for the new account. The key that must sign each transfer out of the account. If
   * mReceiverSignatureRequired is true, then it must also sign any transfer into the account.
   *
   * @param publicKey The desired public key for the new account.
   * @return A reference to this AccountCreateTransaction object with the newly-set public key.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   */
  AccountCreateTransaction& setKey(const std::shared_ptr<PublicKey>& publicKey);

  /**
   * Set the initial amount to transfer into the new account from the paying account.
   *
   * @param initialBalance The desired balance to transfer into the new account.
   * @return A reference to this AccountCreateTransaction object with the newly-set initial balance.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   */
  AccountCreateTransaction& setInitialBalance(const Hbar& initialBalance);

  /**
   * Set the new account's transfer receiver signature policy.
   *
   * @param receiveSignatureRequired \c TRUE to require the new account to sign any Hbar transfer transactions that
   *                                 involve transferring Hbars into itself, otherwise \c FALSE.
   * @return A reference to this AccountCreateTransaction object with the newly-set receiver signature policy.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   */
  AccountCreateTransaction& setReceiverSignatureRequired(bool receiveSignatureRequired);

  /**
   * Set the auto renew period for the new account. A Hedera account is charged to extend its expiration date every
   * renew period. If it doesn't have enough balance, it extends as long as possible. If the balance is zero when it
   * expires, then the account is deleted.
   *
   * @param autoRenewPeriod The desired auto renew period for the new account.
   * @return A reference to this AccountCreateTransaction object with the newly-set auto renew period.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   */
  AccountCreateTransaction& setAutoRenewPeriod(const std::chrono::duration<double>& autoRenewPeriod);

  /**
   * Set a memo for the new account.
   *
   * @param memo The desired memo for the new account.
   * @return A reference to this AccountCreateTransaction object with the newly-set memo.
   * @throws std::length_error If the memo is more than 100 characters.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   */
  AccountCreateTransaction& setAccountMemo(std::string_view memo);

  /**
   * Set the maximum automatic token associations the new account can have.
   *
   * @param associations The desired maximum amount of token associations for the new account.
   * @return A reference to this AccountCreateTransaction object with the newly-set maximum automatic token
   *         associations.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   * @throws std::invalid_argument If the desired maximum number of associations is over 5000.
   */
  AccountCreateTransaction& setMaxAutomaticTokenAssociations(uint32_t associations);

  /**
   * Set the account to which the new account should stake. This is mutually exclusive with mStakedNodeId, and will
   * reset the value of the mStakedNodeId if it is set.
   *
   * @param stakedAccountId The ID of the desired account to which the new account will stake.
   * @return A reference to this AccountCreateTransaction object with the newly-set staked account ID.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   */
  AccountCreateTransaction& setStakedAccountId(const AccountId& stakedAccountId);

  /**
   * Set the node to which the new account should stake. This is mutually exclusive with mStakedAccountId, and will
   * reset the value of the mStakedAccountId if it is set.
   *
   * @param stakedNodeId The ID of the desired node to which the new account will stake.
   * @return A reference to this AccountCreateTransaction object with the newly-set staked node ID.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   */
  AccountCreateTransaction& setStakedNodeId(const uint64_t& stakedNodeId);

  /**
   * Set the staking reward reception policy for the new account.
   *
   * @param declineReward \c TRUE if the new account should decline receiving staking rewards, otherwise \c FALSE.
   * @return A reference to this AccountCreateTransaction object with the newly-set staking rewards reception policy.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   */
  AccountCreateTransaction& setDeclineStakingReward(bool declineReward);

  /**
   * Set the key to be used as the new account's alias. Currently, only ED25519/ECDSA_SECP256K1 primitive key types are
   * supported. ThresholdKey, KeyList, ContractID, and delegatable_contract_id are not supported.
   *
   * A given alias can map to at most one account on the network at a time. This uniqueness will be enforced relative to
   * aliases currently on the network at alias assignment.
   *
   * If a transaction creates an account using an alias, any further crypto transfers to that alias will simply be
   * deposited in that account, without creating anything, and with no creation fee being charged.
   *
   * @param alias The desired key to be used as the account's alias.
   * @return A reference to this AccountCreateTransaction object with the newly-set account alias.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   */
  AccountCreateTransaction& setPublicKeyAlias(const std::shared_ptr<PublicKey>& alias);

  /**
   * Set the EOA 20-byte address to create for the new account. This EVM address may be either the encoded form of the
   * <shard>.<realm>.<num>, or the keccak-256 hash of a ECDSA_SECP256K1 primitive key.
   *
   * @param address The desired EVM address for the new account.
   * @return A reference to this AccountCreateTransaction object with the newly-set EVM address.
   * @throws IllegalStateException If this AccountCreateTransaction is frozen.
   */
  AccountCreateTransaction& setEvmAddressAlias(const EvmAddress& address);

  /**
   * Get the public key to be used for the new account.
   *
   * @return A pointer to the public key to be used for the new account. Nullptr if the key has not yet been set.
   */
  [[nodiscard]] inline std::shared_ptr<PublicKey> getKey() const { return mKey; }

  /**
   * Get the initial balance to be transferred into the new account upon creation (from the paying account).
   *
   * @return The initial balance to be transferred into the new account upon creation (from the paying account).
   */
  [[nodiscard]] inline Hbar getInitialBalance() const { return mInitialBalance; }

  /**
   * Get the Hbar transfer receiver signature policy to be used by the new account.
   *
   * @return \c TRUE if the new account should be required to sign all incoming Hbar transfers, otherwise \c FALSE.
   */
  [[nodiscard]] inline bool getReceiverSignatureRequired() const { return mReceiverSignatureRequired; }

  /**
   * Get the auto renew period for the new account.
   *
   * @return The auto renew period for the new account.
   */
  [[nodiscard]] inline std::chrono::duration<double> getAutoRenewPeriod() const { return mAutoRenewPeriod; }

  /**
   * Get the desired memo for the new account.
   *
   * @return The desired memo for the new account.
   */
  [[nodiscard]] inline std::string getAccountMemo() const { return mAccountMemo; }

  /**
   * Get the maximum automatic token associations for the new account.
   *
   * @return The maximum automatic token associations for the new account.
   */
  [[nodiscard]] inline uint32_t getMaxAutomaticTokenAssociations() const { return mMaxAutomaticTokenAssociations; }

  /**
   * Get the ID of the account to which the new account will stake.
   *
   * @return The ID of the desired account to which the new account will stake. Returns uninitialized if a value has not
   *         yet been set, or if a staked node ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<AccountId> getStakedAccountId() const { return mStakedAccountId; }

  /**
   * Get the ID of the desired node to which the new account will stake.
   *
   * @return The ID of the desired node to which the new account will stake. Returns uninitialized if a value has not
   *         yet been set, or if a staked account ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<uint64_t> getStakedNodeId() const { return mStakedNodeId; }

  /**
   * Get the staking rewards reception policy for the new account.
   *
   * @return \c TRUE if the new account should decline from receiving staking rewards, otherwise \c FALSE
   */
  [[nodiscard]] inline bool getDeclineStakingReward() const { return mDeclineStakingReward; }

  /**
   * Get the key to be used as the new account's alias.
   *
   * @return The key to be used as the new account's alias. Nullptr if the alias has not yet been set.
   */
  [[nodiscard]] inline std::shared_ptr<PublicKey> getPublicKeyAlias() const { return mPublicKeyAlias; }

  /**
   * Get the EVM address of the new account.
   *
   * @return The EVM address of the new account. Returns uninitialized if a value has not yet been set.
   */
  [[nodiscard]] inline std::optional<EvmAddress> getEvmAddressAlias() const { return mEvmAddressAlias; }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this AccountCreateTransaction object.
   *
   * @param client The Client trying to construct this AccountCreateTransaction.
   * @param node   The Node to which this AccountCreateTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this AccountCreateTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                AccountCreateTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this AccountCreateTransaction to a Node.
   *
   * @param client   The Client submitting this AccountCreateTransaction.
   * @param deadline The deadline for submitting this AccountCreateTransaction.
   * @param node     Pointer to the Node to which this AccountCreateTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;

  /**
   * Build a CryptoCreateTransactionBody protobuf object from this AccountCreateTransaction object.
   *
   * @return A pointer to a CryptoCreateTransactionBody protobuf object filled with this AccountCreateTransaction
   *         object's data.
   */
  [[nodiscard]] proto::CryptoCreateTransactionBody* build() const;

  /**
   * The key that must sign each transfer out of the account. If mReceiverSignatureRequired is \c TRUE, then it must
   * also sign any transfer into the account.
   */
  std::shared_ptr<PublicKey> mKey = nullptr;

  /**
   * The initial amount to transfer into the new account.
   */
  Hbar mInitialBalance = Hbar(0LL);

  /**
   * If \c TRUE, the new account's key must sign any transaction being deposited into it (in addition to all
   * withdrawals).
   */
  bool mReceiverSignatureRequired = false;

  /**
   * A Hedera account is charged to extend its expiration date every renew period. If it doesn't have enough balance, it
   * extends as long as possible. If the balance is zero when it expires, then the account is deleted. Defaults to 90
   * days (2160 hours).
   */
  std::chrono::duration<double> mAutoRenewPeriod = std::chrono::hours(2160);

  /**
   * The memo to be associated with the account (UTF-8 encoding max 100 bytes).
   */
  std::string mAccountMemo;

  /**
   * The maximum number of tokens with which the new account can be implicitly associated. Only allows values up to a
   * maximum value of 5000.
   */
  uint32_t mMaxAutomaticTokenAssociations = 0U;

  /**
   * The ID of the account to which the new account will be staked. Mutually exclusive with mStakedNodeId.
   */
  std::optional<AccountId> mStakedAccountId;

  /**
   * The ID of the node to which the new account will be staked. Mutually exclusive with mStakedAccountId.
   */
  std::optional<uint64_t> mStakedNodeId;

  /**
   * If \c TRUE, the new account will decline receiving staking rewards.
   */
  bool mDeclineStakingReward = false;

  /**
   * The bytes to be used as the account's alias. It will be the serialization of a protobuf Key message for an
   * ED25519/ECDSA_SECP256K1 primitive key type. Currently only primitive key bytes are supported as the key for an
   * account with an alias. ThresholdKey, KeyList, ContractID, and delegatable_contract_id are not supported.
   *
   * A given alias can map to at most one account on the network at a time. This uniqueness will be enforced relative to
   * aliases currently on the network at alias assignment.
   *
   * If a transaction creates an account using an alias, any further crypto transfers to that alias will
   * simply be deposited in that account, without creating anything, and with no creation fee being charged.
   */
  std::shared_ptr<PublicKey> mPublicKeyAlias = nullptr;

  /**
   * The EOA 20-byte address to create that is derived from the keccak-256 hash of a ECDSA_SECP256K1 primitive key.
   */
  std::optional<EvmAddress> mEvmAddressAlias;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ACCOUNT_CREATE_TRANSACTION_H_

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
#ifndef HEDERA_SDK_CPP_CONTRACT_UPDATE_TRANSACTION_H_
#define HEDERA_SDK_CPP_CONTRACT_UPDATE_TRANSACTION_H_

#include "AccountId.h"
#include "ContractId.h"
#include "PublicKey.h"
#include "Transaction.h"

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace proto
{
class ContractUpdateTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction that allows you to modify the smart contract entity state like admin keys, proxy account, auto-renew
 * period, and memo. This transaction does not update the contract that is tied to the smart contract entity. The
 * contract tied to the entity is immutable. The contract entity is immutable if an admin key is not specified. Once the
 * transaction has been successfully executed on a Hedera network the previous field values will be updated with the new
 * ones. To get a previous state of a smart contract instance, you can query a mirror node for that data. Any null field
 * is ignored (left unchanged).
 *
 * Transaction Signing Requirements:
 *  - If only the expiration time is being modified, then no signature is needed on this transaction other than for the
 *    account paying for the transaction itself.
 *  - If any other smart contract entity property is being modified, the transaction must be signed by the admin key.
 *  - If the admin key is being updated, the new key must sign.
 */
class ContractUpdateTransaction : public Transaction<ContractUpdateTransaction>
{
public:
  ContractUpdateTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a ContractUpdateInstance transaction.
   */
  explicit ContractUpdateTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the contract to update.
   *
   * @param contractId The ID of the contract to update.
   * @return A reference to this ContractUpdateTransaction object with the newly-set contract ID.
   * @throws IllegalStateException If this ContractUpdateTransaction is frozen.
   */
  ContractUpdateTransaction& setContractId(const ContractId& contractId);

  /**
   * Set a new expiration time for the contract.
   *
   * @param expiration The desired new expiration time for the contract.
   * @return A reference to this ContractUpdateTransaction object with the newly-set expiration time.
   * @throws IllegalStateException If this ContractUpdateTransaction is frozen.
   */
  ContractUpdateTransaction& setExpirationTime(const std::chrono::system_clock::time_point& expiration);

  /**
   * Set a new admin key for the contract.
   *
   * @param adminKey The desired new admin key for the contract.
   * @return A reference to this ContractUpdateTransaction object with the newly-set admin key.
   * @throws IllegalStateException If this ContractUpdateTransaction is frozen.
   */
  ContractUpdateTransaction& setAdminKey(const std::shared_ptr<PublicKey>& adminKey);

  /**
   * Set a new auto renew period for the contract.
   *
   * @param autoRenewPeriod The desired new auto renew period for the contract.
   * @return A reference to this ContractUpdateTransaction object with the newly-set auto renew period.
   * @throws IllegalStateException If this ContractUpdateTransaction is frozen.
   */
  ContractUpdateTransaction& setAutoRenewPeriod(const std::chrono::duration<double>& autoRenewPeriod);

  /**
   * Set a new memo for the contract.
   *
   * @param memo The desired new memo for the contract.
   * @return A reference to this ContractUpdateTransaction object with the newly-set memo.
   * @throws IllegalStateException If this ContractUpdateTransaction is frozen.
   * @throws std::length_error If the memo is more than 100 characters.
   */
  ContractUpdateTransaction& setMemo(std::string_view memo);

  /**
   * Set the new maximum automatic token associations the contract can have.
   *
   * @param associations The desired new maximum amount of token associations for the contract.
   * @return A reference to this ContractUpdateTransaction object with the newly-set maximum automatic token
   *         associations.
   * @throws IllegalStateException If this ContractUpdateTransaction is frozen.
   * @throws std::invalid_argument If the desired maximum number of associations is over 5000.
   */
  ContractUpdateTransaction& setMaxAutomaticTokenAssociations(uint32_t associations);

  /**
   * Set the ID of the account that will auto-renew this contract.
   *
   * @param autoRenewAccountId The ID of the desired new account that will auto-renew this contract.
   * @return A reference to this ContractUpdateTransaction object with the newly-set auto-renew account ID.
   * @throws IllegalStateException If this ContractUpdateTransaction is frozen.
   */
  ContractUpdateTransaction& setAutoRenewAccountId(const AccountId& autoRenewAccountId);

  /**
   * Set the new account to which the contract should stake. This is mutually exclusive with mStakedNodeId, and will
   * reset the value of the mStakedNodeId if it is set.
   *
   * @param stakedAccountId The ID of the desired new account to which the contract will stake.
   * @return A reference to this ContractUpdateTransaction object with the newly-set staked account ID.
   * @throws IllegalStateException If this ContractUpdateTransaction is frozen.
   */
  ContractUpdateTransaction& setStakedAccountId(const AccountId& stakedAccountId);

  /**
   * Set the new node to which the contract should stake. This is mutually exclusive with mStakedAccountId, and will
   * reset the value of the mStakedAccountId if it is set.
   *
   * @param stakedNodeId The ID of the desired new node to which the contract will stake.
   * @return A reference to this ContractUpdateTransaction object with the newly-set staked node ID.
   * @throws IllegalStateException If this ContractUpdateTransaction is frozen.
   */
  ContractUpdateTransaction& setStakedNodeId(const uint64_t& stakedNodeId);

  /**
   * Set a new staking reward reception policy for the contract.
   *
   * @param declineReward \c TRUE if the contract should decline receiving staking rewards, otherwise \c FALSE.
   * @return A reference to this ContractUpdateTransaction object with the newly-set staking rewards reception policy.
   * @throws IllegalStateException If this ContractUpdateTransaction is frozen.
   */
  ContractUpdateTransaction& setDeclineStakingReward(bool declineReward);

  /**
   * Get the ID of the contract to update.
   *
   * @return The ID of the contract to update.
   */
  [[nodiscard]] inline ContractId getContractId() const { return mContractId; }

  /**
   * Get the new expiration time for the contract.
   *
   * @return The new expiration time for the contract. Returns uninitialized if a new expiration time has not yet been
   *         set.
   */
  [[nodiscard]] inline std::optional<std::chrono::system_clock::time_point> getExpirationTime() const
  {
    return mExpirationTime;
  }

  /**
   * Get the new admin key to be used for the contract.
   *
   * @return A pointer to the new admin key to be used for the contract. Returns nullptr if the key has not yet been
   *         set.
   */
  [[nodiscard]] inline std::shared_ptr<PublicKey> getAdminKey() const { return mAdminKey; }

  /**
   * Get the new auto renew period for the contract.
   *
   * @return The new auto renew period for the new contract. Returns uninitialized if a new auto-renew period has not
   *         yet been set.
   */
  [[nodiscard]] inline std::optional<std::chrono::duration<double>> getAutoRenewPeriod() const
  {
    return mAutoRenewPeriod;
  }

  /**
   * Get the new memo for the contract.
   *
   * @return The new memo for the contract. Returns uninitialized if a new memo has not yet been set.
   */
  [[nodiscard]] inline std::optional<std::string> getMemo() const { return mMemo; }

  /**
   * Get the new maximum automatic token associations for the contract.
   *
   * @return The new maximum automatic token associations for the contract. Returns uninitialized if a new maximum
   *         automatic token associations amount has not yet been set.
   */
  [[nodiscard]] inline std::optional<uint32_t> getMaxAutomaticTokenAssociations() const
  {
    return mMaxAutomaticTokenAssociations;
  }

  /**
   * Get the ID of the account that will auto-renew this contract.
   *
   * @return The ID of the account that will auto-renew this contract. Returns uninitialized if a new auto-renew account
   *         ID has not yet been set.
   */
  [[nodiscard]] inline std::optional<AccountId> getAutoRenewAccountId() const { return mAutoRenewAccountId; };

  /**
   * Get the ID of the account to which the contract will stake.
   *
   * @return The ID of the desired new account to which the account will stake. Returns uninitialized if a new staked
   *         account ID has not yet been set, or if a staked node ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<AccountId> getStakedAccountId() const { return mStakedAccountId; }

  /**
   * Get the ID of the desired new node to which the contract will stake.
   *
   * @return The ID of the desired new node to which the contract will stake. Returns uninitialized if a new staked node
   *         ID has not yet been set, or if a staked account ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<uint64_t> getStakedNodeId() const { return mStakedNodeId; }

  /**
   * Get the new staking rewards reception policy for the contract.
   *
   * @return \c TRUE if the contract should decline from receiving staking rewards, otherwise \c FALSE. Returns
   *         uninitialized if a new staking rewards reception policy has not yet been set.
   */
  [[nodiscard]] inline std::optional<bool> getDeclineStakingReward() const { return mDeclineStakingReward; }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this ContractUpdateTransaction object.
   *
   * @param client The Client trying to construct this ContractUpdateTransaction.
   * @param node   The Node to which this ContractUpdateTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this ContractUpdateTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                ContractUpdateTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this ContractUpdateTransaction to a Node.
   *
   * @param client   The Client submitting this ContractUpdateTransaction.
   * @param deadline The deadline for submitting this ContractUpdateTransaction.
   * @param node     Pointer to the Node to which this ContractUpdateTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;

  /**
   * Build a ContractUpdateTransactionBody protobuf object from this ContractUpdateTransaction object.
   *
   * @return A pointer to a ContractUpdateTransactionBody protobuf object filled with this ContractUpdateTransaction
   *         object's data.
   */
  [[nodiscard]] proto::ContractUpdateTransactionBody* build() const;

  /**
   * The ID of the contract to update.
   */
  ContractId mContractId;

  /**
   * The new expiration time for the contract.
   */
  std::optional<std::chrono::system_clock::time_point> mExpirationTime;

  /**
   * The new admin key to be used for the contract.
   */
  std::shared_ptr<PublicKey> mAdminKey = nullptr;

  /**
   * The new auto renew period for the contract.
   */
  std::optional<std::chrono::duration<double>> mAutoRenewPeriod;

  /**
   * The new memo for the contract (UTF-8 encoding max 100 bytes).
   */
  std::optional<std::string> mMemo;

  /**
   * The new maximum automatic token associations for the contract. Only allows values up to a maximum value of 5000.
   */
  std::optional<uint32_t> mMaxAutomaticTokenAssociations;

  /**
   * The ID of the account that will auto-renew this contract.
   */
  std::optional<AccountId> mAutoRenewAccountId;

  /**
   * The ID of the new account to which this contract will be staked. Mutually exclusive with mStakedNodeId.
   */
  std::optional<AccountId> mStakedAccountId;

  /**
   * The ID of the new node to which this contract will be staked. Mutually exclusive with mStakedAccountId.
   */
  std::optional<uint64_t> mStakedNodeId;

  /**
   * If \c TRUE, the contract will now decline receiving staking rewards.
   */
  std::optional<bool> mDeclineStakingReward;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_UPDATE_TRANSACTION_H_

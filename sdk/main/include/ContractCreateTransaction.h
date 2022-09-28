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
#ifndef CONTRACT_CREATE_TRANSACTION_H_
#define CONTRACT_CREATE_TRANSACTION_H_

#include "AccountId.h"
#include "FileId.h"
#include "Hbar.h"
#include "Key.h"
#include "Transaction.h"

#include "helper/InitType.h"

#include <chrono>
#include <string>
#include <unordered_map>

namespace Hedera
{
class ContractFunctionParameters;
class TransactionId;
}

namespace proto
{
class ContractCreateTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Start a new smart contract instance. After the instance is created, the
 * ContractID for it is in the receipt, and can be retrieved by the Record or
 * with a GetByKey query. The instance will run the bytecode, either stored in a
 * previously created file or in the transaction body itself for small
 * contracts.
 *
 * The constructor will be executed using the given amount of gas, and any
 * unspent gas will be refunded to the paying account. Constructor inputs come
 * from the given constructorParameters.
 *  - The instance will exist for autoRenewPeriod seconds. When that is reached,
 *    it will renew itself for another autoRenewPeriod seconds by charging its
 *    associated cryptocurrency account (which it creates here). If it has
 *    insufficient cryptocurrency to extend that long, it will extend as long as
 *    it can. If its balance is zero, the instance will be deleted.
 *
 *  - A smart contract instance normally enforces rules, so "the code is law".
 *    For example, an ERC-20 contract prevents a transfer from being undone
 *    without a signature by the recipient of the transfer. This is always
 *    enforced if the contract instance was created with the adminKeys being
 *    null. But for some uses, it might be desirable to create something like an
 *    ERC-20 contract that has a specific group of trusted individuals who can
 *    act as a "supreme court" with the ability to override the normal
 *    operation, when a sufficient number of them agree to do so. If adminKeys
 *    is not null, then they can sign a transaction that can change the state of
 *    the smart contract in arbitrary ways, such as to reverse a transaction
 *    that violates some standard of behavior that is not covered by the code
 *    itself. The admin keys can also be used to change the autoRenewPeriod, and
 *    change the adminKeys field itself. The API currently does not implement
 *    this ability. But it does allow the adminKeys field to be set and queried,
 *    and will in the future implement such admin abilities for any instance
 *    that has a non-null adminKeys.
 *
 *  - If this constructor stores information, it is charged gas to store it.
 *    There is a fee in hbars to maintain that storage until the expiration
 *    time, and that fee is added as part of the transaction fee.
 *
 *  - An entity (account, file, or smart contract instance) must be created in a
 *    particular realm. If the realmID is left null, then a new realm will be
 *    created with the given admin key. If a new realm has a null adminKey, then
 *    anyone can create/modify/delete entities in that realm. But if an admin
 *    key is given, then any transaction to create/modify/delete an entity in
 *    that realm must be signed by that key, though anyone can still call
 *    functions on smart contract instances that exist in that realm. A realm
 *    ceases to exist when everything within it has expired and no longer
 *    exists.
 *
 *  - The current API ignores shardID, realmID, and newRealmAdminKey, and
 *    creates everything in shard 0 and realm 0, with a null key. Future
 *    versions of the API will support multiple realms and multiple shards.
 *
 *  - The optional memo field can contain a string whose length is up to 100
 *    bytes. That is the size after Unicode NFD then UTF-8 conversion. This
 *    field can be used to describe the smart contract. It could also be used
 *    for other purposes. One recommended purpose is to hold a hexadecimal
 *    string that is the SHA-384 hash of a PDF file containing a human-readable
 *    legal contract. Then, if the admin keys are the public keys of human
 *    arbitrators, they can use that legal document to guide their decisions
 *    during a binding arbitration tribunal, convened to consider any changes to
 *    the smart contract in the future. The memo field can only be changed using
 *    the admin keys. If there are no admin keys, then it cannot be changed
 *    after the smart contract is created.
 *
 * Signing requirements: If an admin key is set, it must sign the
 * transaction. If an auto-renew account is set, its key must sign the
 * transaction.
 */
class ContractCreateTransaction : Transaction<ContractCreateTransaction>
{
public:
  /**
   * Constructor.
   */
  ContractCreateTransaction();

  /**
   * Construct from a map of transaction ID's to their corresponding account
   * ID's and protobuf transactions.
   *
   * @param transactions Map of transaction IDs to their corresponding account
   *                     ID's and protobuf transactions.
   */
  explicit ContractCreateTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Construct from a protobuf transaction object.
   *
   * @param transaction The protobuf transaction object from which to construct
   *                    this transaction.
   */
  explicit ContractCreateTransaction(const proto::TransactionBody& transaction);

  /**
   * Derived from Transaction. Validate the checksums.
   *
   * @param client The client with which to validate the checksums.
   */
  virtual void validateChecksums(const Client& client) const override;

  /**
   * Build a contract create transaction protobuf message based on the data in
   * this class.
   *
   * @return A contract create transaction protobuf message.
   */
  proto::ContractCreateTransactionBody build() const;

  /**
   * Sets the file containing the smart contract byte code. A copy will be made
   * and held by the contract instance, and have the same expiration time as the
   * instance. The file must be the ASCII hexadecimal representation of the
   * smart contract bytecode.
   *
   * @param bytecodeFileId The file ID to be set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setInitCodeFileId(const FileId& bytecodeFileId);

  /**
   * Sets the smart contract byte code. The bytes of the smart contract
   * initcode. This is only useful if the smart contract init is less than the
   * hedera transaction limit. In those cases fileID must be used.
   *
   * @param bytecode The bytecode to set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setInitCodeByteCode(const std::string& bytecode);

  /**
   * Sets the state of the instance and its fields can be modified arbitrarily
   * if this key signs a transaction to modify it. If this is null, then such
   * modifications are not possible, and there is no administrator that can
   * override the normal operation of this smart contract instance. Note that if
   * it is created with no admin keys, then there is no administrator to
   * authorize changing the admin keys, so there can never be any admin keys for
   * that instance.
   *
   * @param adminKey The admin key to set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setAdminKey(const Key& adminKey);

  /**
   * Sets the gas to run the constructor.
   *
   * @param gas The gas to set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setGas(const int64_t& gas);

  /**
   * Sets the initial number of Hbars to put into the cryptocurrency account
   * associated with and owned by the smart contract.
   *
   * @param initialBalance The Hbar to be set as the initial balance.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setInitialBalance(const Hbar& initialBalance);

  /**
   * Sets the period that the instance will charge its account every this many
   * seconds to renew.
   *
   * @param autoRenewPeriod The auto renew period to set for auto renewal.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setAutoRenewPeriod(
    const std::chrono::seconds& autoRenewPeriod);

  /**
   * Sets the constructor parameters as their raw bytes.
   *
   * Use this instead of setConstructorParameters(ContractFunctionParameters) if
   * you have already pre-encoded a solidity function call.
   *
   * @param constructorParameters The constructor parameters to set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setConstructorParameters(
    const std::string& constructorParameters);

  /**
   * Sets the parameters to pass to the constructor.
   *
   * @param constructorParameters The contructor parameters to set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setConstructorParameters(
    const ContractFunctionParameters& constructorParameters);

  /**
   * Sets the memo to be associated with this contract.
   *
   * @param memo The memo to set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setContractMemo(const std::string& memo);

  /**
   * Sets the new maximum number of tokens that this contract can be
   * automatically associated with (i.e., receive air-drops from).
   *
   * @param maxAutomaticTokenAssociations The maximum automatic token
   *                                      associations to set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setMaxAutomaticTokenAssociations(
    int32_t maxAutomaticTokenAssociations);

  /**
   * Set the account to charge for auto-renewal of this contract. If not set, or
   * set to an account with zero hbar balance, the contract's own Hbar balance
   * will be used to cover auto-renewal fees.
   *
   * @param autoRenewAccountId The auto renew account ID to set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setAutoRenewAccountId(
    const AccountId& autoRenewAccountId);

  /**
   * Set the account to which this contract will stake.
   *
   * @param stakedAccountId The staked account ID to set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setStakedAccountId(
    const AccountId& stakedAccountId);

  /**
   * Set the node to which this contract will stake.
   *
   * @param stakedNodeId The staked node ID to set.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setStakedNodeId(const int64_t& stakedNodeId);

  /**
   * Set the staking rewards policy.
   *
   * @param declineStakingReward The staking rewards policy to set. \c TRUE to
   *                             decline staking rewards, otherwise \c FALSE.
   * @return Reference to this ContractCreateTransaction object.
   */
  ContractCreateTransaction& setDeclineStakingReward(bool declineStakingReward);

  /**
   * Extract the init code file ID.
   *
   * @return The init code file ID.
   */
  inline InitType<FileId> getInitCodeFileId() const { return mInitCodeFileId; }

  /**
   * Extract the init code byte code.
   *
   * @return The init code byte code.
   */
  inline InitType<std::string> getInitCodeByteCode() const
  {
    return mInitCodeByteCode;
  }

  /**
   * Extract the admin key.
   *
   * @return The admin key.
   */
  inline InitType<Key> getAdminKey() const { return mAdminKey; }

  /**
   * Extract the gas.
   *
   * @return The gas.
   */
  inline int64_t getGas() const { return mGas; }

  /**
   * Extract the initial balance.
   *
   * @return The initial balance in Hbar.
   */
  inline Hbar getInitialBalance() const { return mInitialBalance; }

  /**
   * Extract the auto renew period.
   *
   * @return The auto renew period.
   */
  inline InitType<std::chrono::seconds> getAutoRenewPeriod() const
  {
    return mAutoRenewPeriod;
  }

  /**
   * Extract the constructor parameters.
   *
   * @return The constructor parameters.
   */
  inline std::string getConstructorParameters() const
  {
    return mConstructorParameters;
  }
  /**
   * Extract the memo.
   *
   * @return The memo.
   */
  inline std::string getContractMemo() const { return mMemo; }

  /**
   * Extract the max automatic token associations.
   *
   * @return The max automatic token associations.
   */
  inline int32_t getMaxAutomaticTokenAssociations() const
  {
    return mMaxAutomaticTokenAssociations;
  }

  /**
   * Extract the auto renew account ID.
   *
   * @return The auto renew account ID.
   */
  inline InitType<AccountId> getAutoRenewAccountId() const
  {
    return mAutoRenewAccountId;
  }

  /**
   * Extract the ID of the account to which this contract will stake.
   *
   * @return The ID of the account to which this contract will stake.
   */
  inline InitType<AccountId> getStakedAccountId() const
  {
    return mStakedAccountId;
  }

  /**
   * Extract the ID of the node to which this contract will stake.
   *
   * @return The ID of the node to which this contract will stake.
   */
  inline InitType<int64_t> getStakedNodeId() const { return mStakedNodeId; }

  /**
   * Extract the current staking rewards policy.
   *
   * @return \c TRUE if the contract declines receiving a staking reward,
   *         otherwise \c FALSE.
   */
  inline bool getDeclineReward() const { return mDeclineReward; }

private:
  /**
   * Initialize from the transaction body.
   */
  void initFromTransactionBody();

  /**
   * The file containing the smart contract initcode. A copy will be made and
   * held by the contract instance, and have the same expiration time as the
   * instance.
   */
  InitType<FileId> mInitCodeFileId;

  /**
   * The bytes of the smart contract initcode. This is only useful if the smart
   * contract init is less than the hedera transaction limit. In those cases
   * fileID must be used.
   */
  InitType<std::string> mInitCodeByteCode;

  /**
   * The state of the instance and its fields can be modified arbitrarily if
   * this key signs a transaction to modify it. If this is null, then such
   * modifications are not possible, and there is no administrator that can
   * override the normal operation of this smart contract instance. Note that if
   * it is created with no admin keys, then there is no administrator to
   * authorize changing the admin keys, so there can never be any admin keys for
   * that instance.
   */
  InitType<Key> mAdminKey;

  /**
   * The gas to run the constructor.
   */
  int64_t mGas;

  /**
   * The initial amount of Hbar to put into the cryptocurrency account
   * associated with and owned by the smart contract.
   */
  Hbar mInitialBalance;

  /**
   * The instance will charge its account every this many seconds to renew for
   * this long.
   */
  InitType<std::chrono::seconds> mAutoRenewPeriod;

  /**
   * The parameters to pass to the constructor.
   */
  std::string mConstructorParameters;

  /**
   * The memo that was submitted as part of the contract (max 100 bytes).
   */
  std::string mMemo;

  /**
   * The maximum number of tokens that this contract can be automatically
   * associated with (i.e., receive air-drops from).
   */
  int32_t mMaxAutomaticTokenAssociations;

  /**
   * An account to charge for auto-renewal of this contract. If not set, or set
   * to an account with zero hbar balance, the contract's own hbar balance will
   * be used to cover auto-renewal fees.
   */
  InitType<AccountId> mAutoRenewAccountId;

  /**
   * The account ID to which this contract is staking. Mutually exclusive to
   * mStakedNodeId.
   */
  InitType<AccountId> mStakedAccountId;

  /**
   * The node ID this contract is staked to. Mutually exclusive to
   * mStakedAccountId.
   */
  InitType<int64_t> mStakedNodeId;

  /**
   * If true, the contract declines receiving a staking reward. The default
   * value is false.
   */
  bool mDeclineReward;
};

} // namespace Hedera

#endif // CONTRACT_CREATE_TRANSACTION_H_
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
#ifndef CONTRACT_INFO_H_
#define CONTRACT_INFO_H_

#include "AccountId.h"
#include "ContractId.h"
#include "Hbar.h"
#include "Key.h"
#include "StakingInfo.h"

#include "helper/InitType.h"

#include "contract_get_info.pb.h"

#include <chrono>
#include <string>

namespace Hedera
{
/**
 * Contains information about a smart contract instance. This includes the
 * account that it uses, the file containing its initcode (if a file was used to
 * initialize the contract), and the time when it will expire.
 */
class ContractInfo
{
public:
  /**
   * Constructor.
   */
  ContractInfo();

  /**
   * Extract the contract info from a contract info protobuf object.
   *
   * @param proto The contract info protobuf object.
   * @return The contract info object.
   */
  static ContractInfo fromProtobuf(
    const proto::ContractGetInfoResponse_ContractInfo& proto);

  /**
   * Build a contract info protobuf object from this contract info object.
   *
   * @return The contract info protobuf object.
   */
  proto::ContractGetInfoResponse_ContractInfo toProtobuf() const;

  /**
   * Extract the contract ID of the contract.
   *
   * @return The contract ID of the contract.
   */
  inline InitType<ContractId> getContractId() const { return mContractId; }

  /**
   * Extract the account ID of the account that owns this contract.
   *
   * @return The account ID of the account that owns this contract.
   */
  inline InitType<AccountId> getAccountId() const { return mAccountId; }

  /**
   * Extract the Solidity contract account ID.
   *
   * @return The Solidity contract account ID.
   */
  inline std::string getContractAccountId() const { return mContractAccountId; }

  /**
   * Extract the admin key used to sign transaction modifications.
   *
   * @return The admin key associated with this contract.
   */
  inline InitType<Key> getAdminKey() const { return mAdminKey; }

  /**
   * Extract the expiration time of this contract.
   *
   * @return The expiration time of this contract.
   */
  inline InitType<std::chrono::nanoseconds> getExpirationTime() const
  {
    return mExpirationTime;
  }

  /**
   * Extract the amount of time added to the expiration date when the contract
   * instance automatically renews.
   *
   * @return The amount of time added to the expiration date when the contract
   * instance automatically renews.
   */
  inline InitType<std::chrono::seconds> getAutoRenewPeriod() const
  {
    return mAutoRenewPeriod;
  }

  /**
   * Extract the storage space of this contract, in bytes.
   *
   * @return The storage space of this contract, in bytes.
   */
  inline int64_t getStorage() const { return mStorage; }

  /**
   * Extract the contract memo.
   *
   * @return The contract memo.
   */
  inline std::string getMemo() const { return mMemo; }

  /**
   * Extract the current contract balance.
   *
   * @return The current contract balance.
   */
  inline Hbar getBalance() const { return mBalance; }

  /**
   * Extract the deleted status of this contract.
   *
   * @return \c TRUE if the contract has been deleted, otherwise \c FALSE.
   */
  inline bool getDeleted() const { return mDeleted; }

  /**
   * Extract the ledger ID from which the response was returned.
   *
   * @return The ledger ID from which the response was returned.
   */
  inline std::string getLedgerId() const { return mLedgerId; }

  /**
   * Extract the account ID to charge for auto-renewal.
   *
   * @return The account ID to charge for auto-renewal.
   */
  inline InitType<AccountId> getAutoRenewAccountId() const
  {
    return mAutoRenewAccountId;
  }

  /**
   * Extract the maximum number of token associations for this contract.
   *
   * @return The maximum number of token associations for this contract.
   */
  inline int32_t getMaxAutomaticTokenAssociations() const
  {
    return mMaxAutomaticTokenAssociations;
  }

  /**
   * Extract the staking metadata for this contract.
   *
   * @return The staking metadata for this contract.
   */
  inline InitType<StakingInfo> getStakingInfo() const { return mStakingInfo; }

private:
  /**
   * The ID of the contract instance, in the format used in transactions.
   */
  InitType<ContractId> mContractId;

  /**
   * The ID of the cryptocurrency account owned by the contract instance, in the
   * format used in transactions.
   */
  InitType<AccountId> mAccountId;

  /**
   * The ID of both the contract instance and the cryptocurrency account owned
   * by the contract instance, in the format used by Solidity.
   */
  std::string mContractAccountId;

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
   * The current time at which this contract instance (and its account) is set
   * to expire in nanoseconds from epoch.
   */
  InitType<std::chrono::nanoseconds> mExpirationTime;

  /**
   * The expiration time will extend every this many seconds. If there are
   * insufficient funds, then it extends as long as possible. If the account is
   * empty when it expires, then it is deleted.
   */
  InitType<std::chrono::seconds> mAutoRenewPeriod;

  /**
   * The number of bytes of storage being used by this instance (which affects
   * the cost to extend the expiration time).
   */
  int64_t mStorage;

  /**
   * The memo associated with the contract (max 100 bytes).
   */
  std::string mMemo;

  /**
   * The current balance of the contract.
   */
  Hbar mBalance;

  /**
   * \c TRUE if the contract has been deleted, otherwise \c FALSE
   */
  bool mDeleted;

  /**
   * The ledger ID the response was returned from; please see HIP-198 for the
   * network-specific IDs.
   */
  std::string mLedgerId;

  /**
   * The ID of the an account to charge for auto-renewal of this contract. If
   * not set, or set to an account with zero hbar balance, the contract's own
   * hbar balance will be used to cover auto-renewal fees.
   */
  InitType<AccountId> mAutoRenewAccountId;

  /**
   * The maximum number of tokens with which this contract can be implicitly
   * associated.
   */
  int32_t mMaxAutomaticTokenAssociations;

  /**
   * The staking metadata for this contract.
   */
  InitType<StakingInfo> mStakingInfo;
};

} // namespace Hedera

#endif // CONTRACT_INFO_H_
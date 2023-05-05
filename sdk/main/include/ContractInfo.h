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
#ifndef HEDERA_SDK_CPP_CONTRACT_INFO_H_
#define HEDERA_SDK_CPP_CONTRACT_INFO_H_

#include "AccountId.h"
#include "ContractId.h"
#include "Hbar.h"
#include "LedgerId.h"
#include "PublicKey.h"
#include "StakingInfo.h"

#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace proto
{
class ContractGetInfoResponse_ContractInfo;
}

namespace Hedera
{
/**
 * Contains information about a smart contract instance. This includes the account that it uses, the file containing its
 * initcode (if a file was used to initialize the contract), and the time when it will expire.
 */
class ContractInfo
{
public:
  /**
   * Construct a ContractInfo object from a ContractGetInfoResponse_ContractInfo protobuf object.
   *
   * @param proto The ContractGetInfoResponse_ContractInfo protobuf object from which to construct a ContractInfo
   *              object.
   * @return The constructed ContractInfo object.
   */
  [[nodiscard]] static ContractInfo fromProtobuf(const proto::ContractGetInfoResponse_ContractInfo& proto);

  /**
   * The ID of the contract.
   */
  ContractId mContractId;

  /**
   * The ID of the account that is owned by the contract.
   */
  AccountId mAccountId;

  /**
   * The ID of both the contract and the account owned by the contract, in the format used by Solidity.
   */
  std::string mContractAccountId;

  /**
   * Pointer to the admin key that can be used to modify the state of the contract.
   */
  std::shared_ptr<PublicKey> mAdminKey = nullptr;

  /**
   * The expiration time of the queried contract and its account.
   */
  std::chrono::system_clock::time_point mExpirationTime;

  /**
   * The duration of time the queried contract uses to automatically extend its expiration period.
   */
  std::chrono::duration<double> mAutoRenewPeriod;

  /**
   * Get the number of bytes of storage the queried contract is using (which affects the cost to extend the expiration
   * time).
   */
  uint64_t mStorage = 0ULL;

  /**
   * The contract memo.
   */
  std::string mMemo;

  /**
   * The current contract balance.
   */
  Hbar mBalance = Hbar(0LL);

  /**
   * Has this contract been deleted?
   */
  bool mIsDeleted = false;

  /**
   * The ledger ID from which the response was returned.
   */
  LedgerId mLedgerId;

  /**
   * The ID of the account to charge for auto-renewal. If not set, or set to an account with zero Hbar balance, the
   * contract's own Hbar balance will be used to cover auto-renewal fees.
   */
  std::optional<AccountId> mAutoRenewAccountId;

  /**
   * The maximum number of token associations for this contract.
   */
  int32_t mMaxAutomaticTokenAssociations = 0U;

  /**
   * The staking metadata for this contract.
   */
  StakingInfo mStakingInfo;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_INFO_H_

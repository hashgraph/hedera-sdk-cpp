/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#ifndef HEDERA_TCK_CPP_CREATE_ACCOUNT_PARAMS_H_
#define HEDERA_TCK_CPP_CREATE_ACCOUNT_PARAMS_H_

#include "CommonTransactionParams.h"

#include <optional>
#include <string>

namespace Hedera::TCK::SdkClient::AccountClient
{
/**
 * Struct to hold the arguments for a `createAccount` function call.
 */
struct CreateAccountParams
{
  /**
   * The desired key for the account.
   */
  std::optional<std::string>& key;

  /**
   * The desired initial balance for the account.
   */
  std::optional<int64_t>& initialBalance;

  /**
   * Should the new account require a receiver signature?
   */
  std::optional<bool>& receiverSignatureRequired;

  /**
   * The desired amount of time in seconds to renew the new account.
   */
  std::optional<int64_t>& autoRenewPeriod;

  /**
   * The desired memo for the new account.
   */
  std::optional<std::string>& memo;

  /**
   * The desired maximum number of automatic token associations for the account.
   */
  std::optional<int32_t>& maxAutoTokenAssociations;

  /**
   * The ID of the desired account to which the new account should stake.
   */
  std::optional<std::string>& stakedAccountId;

  /**
   * The ID of the desired node to which the new account should stake.
   */
  std::optional<int64_t>& stakedNodeId;

  /**
   * Should the new account decline staking rewards?
   */
  std::optional<bool>& declineStakingReward;

  /**
   * The desired alias for the new account.
   */
  std::optional<std::string>& alias;

  /**
   * Any parameters common to all transaction types.
   */
  std::optional<CommonTransactionParams>& commonTxParams;
};

} // namespace Hedera::TCK::SdkClient::AccountClient

#endif // HEDERA_TCK_CPP_CREATE_ACCOUNT_PARAMS_H_

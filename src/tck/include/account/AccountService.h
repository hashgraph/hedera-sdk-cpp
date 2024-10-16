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
#ifndef HEDERA_TCK_CPP_ACCOUNT_SERVICE_H_
#define HEDERA_TCK_CPP_ACCOUNT_SERVICE_H_

#include "BaseService.h"
#include "CommonTransactionParams.h"
#include "SdkClient.h"
#include "account/params/CreateAccountParams.h"

#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <vector>

namespace Hedera::TCK
{
/**
 * Class that uses the SDK Client to execute account-based requests on the SDK Client's network.
 */
class AccountService : public BaseService
{
public:
  /**
   * Create an account.
   *
   * @param key                       The desired key for the account.
   * @param initialBalance            The desired initial balance for the account.
   * @param receiverSignatureRequired Should the new account require a receiver signature?
   * @param autoRenewPeriod           The desired amount of time in seconds to renew the new account.
   * @param memo                      The desired memo for the new account.
   * @param maxAutoTokenAssociations  The desired maximum number of automatic token associations for the account.
   * @param stakedAccountId           The ID of the desired account to which the new account should stake.
   * @param stakedNodeId              The ID of the desired node to which the new account should stake.
   * @param declineStakingReward      Should the new account decline staking rewards?
   * @param alias                     The desired alias for the new account.
   * @param commonTxParams            Any parameters common to all transaction types.
   * @return A JSON response containing the created account ID and the status of the account creation.
   */
  nlohmann::json createAccount(const std::optional<std::string>& key,
                               const std::optional<int64_t>& initialBalance,
                               const std::optional<bool>& receiverSignatureRequired,
                               const std::optional<int64_t>& autoRenewPeriod,
                               const std::optional<std::string>& memo,
                               const std::optional<int32_t>& maxAutoTokenAssociations,
                               const std::optional<std::string>& stakedAccountId,
                               const std::optional<int64_t>& stakedNodeId,
                               const std::optional<bool>& declineStakingReward,
                               const std::optional<std::string>& alias,
                               const std::optional<CommonTransactionParams>& commonTxParams);

  /**
   * Delete an account.
   *
   * @param deleteAccountId   The ID of the account to delete.
   * @param transferAccountId The ID of the account to which to transfer remaining balances.
   * @param commonTxParams    Any parameters common to all transaction types.
   * @return A JSON response containing the status of the account deletion.
   */
  nlohmann::json deleteAccount(const std::optional<std::string>& deleteAccountId,
                               const std::optional<std::string>& transferAccountId,
                               const std::optional<CommonTransactionParams>& commonTxParams);

  /**
   * Update an account.
   *
   * @param accountId                 The ID of the account to update.
   * @param key                       The desired key for the account.
   * @param autoRenewPeriod           The new desired amount of time in seconds to renew the account.
   * @param expirationTime            The new desired time for the account to expire.
   * @param receiverSignatureRequired Should the account now require a receiver signature?
   * @param memo                      The new desired memo for the account.
   * @param maxAutoTokenAssociations  The new desired maximum number of automatic token associations for the account.
   * @param stakedAccountId           The ID of the new desired account to which the account should stake.
   * @param stakedNodeId              The ID of the new desired node to which the account should stake.
   * @param declineStakingReward      Should the account now decline staking rewards?
   * @param commonTxParams            Any parameters common to all transaction types.
   * @return A JSON response containing the created account ID and the status of the account creation.
   */
  nlohmann::json updateAccount(const std::optional<std::string>& accountId,
                               const std::optional<std::string>& key,
                               const std::optional<int64_t>& autoRenewPeriod,
                               const std::optional<int64_t>& expirationTime,
                               const std::optional<bool>& receiverSignatureRequired,
                               const std::optional<std::string>& memo,
                               const std::optional<int32_t>& maxAutoTokenAssociations,
                               const std::optional<std::string>& stakedAccountId,
                               const std::optional<int64_t>& stakedNodeId,
                               const std::optional<bool>& declineStakingReward,
                               const std::optional<CommonTransactionParams>& commonTxParams);
};

} // namespace Hedera::TCK::SdkClient

#endif // HEDERA_TCK_CPP_ACCOUNT_SERVICE_H_

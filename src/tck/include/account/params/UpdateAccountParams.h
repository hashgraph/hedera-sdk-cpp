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
#ifndef HEDERA_TCK_CPP_UPDATE_ACCOUNT_PARAMS_H_
#define HEDERA_TCK_CPP_UPDATE_ACCOUNT_PARAMS_H_

#include "CommonTransactionParams.h"
#include "JsonUtils.h"
#include "KeyHelper.h"

#include <AccountId.h>
#include <Key.h>

#include <chrono>
#include <cstdint>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace Hedera::TCK
{
/**
 * Struct to hold the arguments for an `updateAccount` function call.
 */
struct UpdateAccountParams
{
  /**
   * The ID of the account to update.
   */
  std::optional<AccountId> mAccountId;

  /**
   * The desired new key for the account.
   */
  std::shared_ptr<Key> mKey;

  /**
   * The desired new amount of time in seconds to renew the account.
   */
  std::optional<std::chrono::seconds> mAutoRenewPeriod;

  /**
   * The new expiration time to which to extend this account.
   */
  std::optional<std::chrono::system_clock::time_point> mExpirationTime;

  /**
   * Should the account now require a receiver signature?
   */
  std::optional<bool> mReceiverSignatureRequired;

  /**
   * The desired new memo for the account.
   */
  std::optional<std::string> mMemo;

  /**
   * The desired new maximum number of automatic token associations for the account.
   */
  std::optional<int32_t> mMaxAutoTokenAssociations;

  /**
   * The ID of the new desired account to which the account should stake.
   */
  std::optional<AccountId> mStakedAccountId;

  /**
   * The ID of the new desired node to which the account should stake.
   */
  std::optional<int64_t> mStakedNodeId;

  /**
   * Should the account now decline staking rewards?
   */
  std::optional<bool> mDeclineStakingReward;

  /**
   * The new desired alias for the account.
   */
  std::optional<std::string> mAlias;

  /**
   * Any parameters common to all transaction types.
   */
  std::optional<CommonTransactionParams> mCommonTxParams;
};

} // namespace Hedera::TCK::SdkClient::AccountClient

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert CreateAccountParams arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<Hedera::TCK::UpdateAccountParams>
{
  /**
   * Convert a JSON object to a CreateAccountParams.
   *
   * @param jsonFrom The JSON object with which to fill the CreateAccountParams.
   * @param params   The CreateAccountParams to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, Hedera::TCK::UpdateAccountParams& params)
  {
    if (auto accountIdStr = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "accountId");
        accountIdStr.has_value())
    {
      params.mAccountId = Hedera::AccountId::fromString(accountIdStr.value());
    }

    if (auto keyStr = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "key"); keyStr.has_value())
    {
      params.mKey = Hedera::TCK::getHederaKey(keyStr.value());
    }

    if (auto autoRenewPeriodInt = Hedera::TCK::getOptionalJsonParameter<int64_t>(jsonFrom, "autoRenewPeriod");
        autoRenewPeriodInt.has_value())
    {
      params.mAutoRenewPeriod = std::chrono::seconds(autoRenewPeriodInt.value());
    }

    if (auto expirationTimeInt = Hedera::TCK::getOptionalJsonParameter<int64_t>(jsonFrom, "expirationTime");
        expirationTimeInt.has_value())
    {
      params.mExpirationTime =
        std::chrono::system_clock::from_time_t(0) + std::chrono::seconds(expirationTimeInt.value());
    }

    params.mReceiverSignatureRequired =
      Hedera::TCK::getOptionalJsonParameter<bool>(jsonFrom, "receiverSignatureRequired");

    params.mMemo = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "memo");
    params.mMaxAutoTokenAssociations =
      Hedera::TCK::getOptionalJsonParameter<int64_t>(jsonFrom, "maxAutoTokenAssociations");

    if (auto stakedAccountIdStr = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "stakedAccountId");
        stakedAccountIdStr.has_value())
    {
      params.mStakedAccountId = Hedera::AccountId::fromString(stakedAccountIdStr.value());
    }

    params.mStakedNodeId = Hedera::TCK::getOptionalJsonParameter<int64_t>(jsonFrom, "stakedNodeId");
    params.mDeclineStakingReward = Hedera::TCK::getOptionalJsonParameter<bool>(jsonFrom, "declineStakingReward");
    params.mCommonTxParams =
      Hedera::TCK::getOptionalJsonParameter<Hedera::TCK::CommonTransactionParams>(jsonFrom, "commonTransactionParams");
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_UPDATE_ACCOUNT_PARAMS_H_

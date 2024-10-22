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
#ifndef HEDERA_TCK_CPP_DELETE_ACCOUNT_PARAMS_H_
#define HEDERA_TCK_CPP_DELETE_ACCOUNT_PARAMS_H_

#include "CommonTransactionParams.h"
#include "JsonUtils.h"

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
 * Struct to hold the arguments for a `deleteAccount` function call.
 */
struct DeleteAccountParams
{
  /**
   * The ID of the account to delete.
   */
  std::optional<AccountId> mDeleteAccountId;

  /**
   * The ID of the account to which to transfer remaining balances.
   */
  std::optional<AccountId> mTransferAccountId;

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
struct [[maybe_unused]] adl_serializer<Hedera::TCK::DeleteAccountParams>
{
  /**
   * Convert a JSON object to a CreateAccountParams.
   *
   * @param jsonFrom The JSON object with which to fill the CreateAccountParams.
   * @param params   The CreateAccountParams to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, Hedera::TCK::DeleteAccountParams& params)
  {
    if (auto deleteAccountIdStr = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "deleteAccountId");
        deleteAccountIdStr.has_value())
    {
      params.mDeleteAccountId = Hedera::AccountId::fromString(deleteAccountIdStr.value());
    }

    if (auto transferAccountIdStr = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "transferAccountId");
        transferAccountIdStr.has_value())
    {
      params.mTransferAccountId = Hedera::AccountId::fromString(transferAccountIdStr.value());
    }

    params.mCommonTxParams =
      Hedera::TCK::getOptionalJsonParameter<Hedera::TCK::CommonTransactionParams>(jsonFrom, "commonTransactionParams");
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_DELETE_ACCOUNT_PARAMS_H_

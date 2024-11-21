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
#ifndef HIERO_TCK_CPP_ASSOCIATE_TOKEN_PARAMS_H_
#define HIERO_TCK_CPP_ASSOCIATE_TOKEN_PARAMS_H_

#include "common/CommonTransactionParams.h"
#include "json/JsonUtils.h"

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

namespace Hedera::TCK::TokenService
{
/**
 * Struct to hold the arguments for an `associateToken` JSON-RPC method call.
 */
struct AssociateTokenParams
{
  /**
   * The ID of the account with which to associate the tokens.
   */
  std::optional<std::string> mAccountId;

  /**
   * The IDs of the tokens to associate.
   */
  std::optional<std::vector<std::string>> mTokenIds;

  /**
   * Any parameters common to all transaction types.
   */
  std::optional<CommonTransactionParams> mCommonTxParams;
};

} // namespace Hedera::TCK::TokenService

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert AssociateTokenParams arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<Hedera::TCK::TokenService::AssociateTokenParams>
{
  /**
   * Convert a JSON object to a AssociateTokenParams.
   *
   * @param jsonFrom The JSON object with which to fill the AssociateTokenParams.
   * @param params   The AssociateTokenParams to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, Hedera::TCK::TokenService::AssociateTokenParams& params)
  {
    params.mAccountId = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "accountId");
    params.mTokenIds = Hedera::TCK::getOptionalJsonParameter<std::vector<std::string>>(jsonFrom, "tokenIds");
    params.mCommonTxParams =
      Hedera::TCK::getOptionalJsonParameter<Hedera::TCK::CommonTransactionParams>(jsonFrom, "commonTransactionParams");
  }
};

} // namespace nlohmann

#endif // HIERO_TCK_CPP_ASSOCIATE_TOKEN_PARAMS_H_

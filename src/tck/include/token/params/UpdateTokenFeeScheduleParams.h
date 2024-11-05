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
#ifndef HEDERA_TCK_CPP_UPDATE_TOKEN_FEE_SCHEDULE_PARAMS_H_
#define HEDERA_TCK_CPP_UPDATE_TOKEN_FEE_SCHEDULE_PARAMS_H_

#include "common/CommonTransactionParams.h"
#include "token/CustomFeeSerializer.h"
#include "json/JsonUtils.h"

#include <CustomFee.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

namespace Hedera::TCK::TokenService
{
/**
 * Struct to hold the arguments for a `updateTokenFeeSchedule` JSON-RPC method call.
 */
struct UpdateTokenFeeScheduleParams
{
  /**
   * The ID of the token of which to update the fee schedule.
   */
  std::optional<std::string> mTokenId;

  /**
   * The desired new fees to assess for the token.
   */
  std::optional<std::vector<std::shared_ptr<CustomFee>>> mCustomFees;

  /**
   * Any parameters common to all transaction types.
   */
  std::optional<CommonTransactionParams> mCommonTxParams;
};

} // namespace Hedera::TCK::TokenService

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert UpdateTokenFeeScheduleParams arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<Hedera::TCK::TokenService::UpdateTokenFeeScheduleParams>
{
  /**
   * Convert a JSON object to a UpdateTokenFeeScheduleParams.
   *
   * @param jsonFrom The JSON object with which to fill the UpdateTokenFeeScheduleParams.
   * @param params   The UpdateTokenFeeScheduleParams to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, Hedera::TCK::TokenService::UpdateTokenFeeScheduleParams& params)
  {
    params.mTokenId = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "tokenId");
    params.mCustomFees =
      Hedera::TCK::getOptionalJsonParameter<std::vector<std::shared_ptr<Hedera::CustomFee>>>(jsonFrom, "customFees");
    params.mCommonTxParams =
      Hedera::TCK::getOptionalJsonParameter<Hedera::TCK::CommonTransactionParams>(jsonFrom, "commonTransactionParams");
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_UPDATE_TOKEN_FEE_SCHEDULE_PARAMS_H_

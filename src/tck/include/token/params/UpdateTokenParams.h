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
#ifndef HEDERA_TCK_CPP_UPDATE_TOKEN_PARAMS_H_
#define HEDERA_TCK_CPP_UPDATE_TOKEN_PARAMS_H_

#include "common/CommonTransactionParams.h"
#include "token/CustomFeeSerializer.h"
#include "json/JsonUtils.h"

#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace Hedera::TCK::TokenService
{
/**
 * Struct to hold the arguments for a `updateToken` JSON-RPC method call.
 */
struct UpdateTokenParams
{
  /**
   * The ID of the token to update.
   */
  std::optional<std::string> mTokenId;

  /**
   * The desired new symbol of the token.
   */
  std::optional<std::string> mSymbol;

  /**
   * The desired new name of the token.
   */
  std::optional<std::string> mName;

  /**
   * The ID of the desired new treasury account of the token.
   */
  std::optional<std::string> mTreasuryAccountId;

  /**
   * The desired new admin key of the token.
   */
  std::optional<std::string> mAdminKey;

  /**
   * The desired new KYC key of the token.
   */
  std::optional<std::string> mKycKey;

  /**
   * The desired new freeze key of the token.
   */
  std::optional<std::string> mFreezeKey;

  /**
   * The desired new wipe key of the token.
   */
  std::optional<std::string> mWipeKey;

  /**
   * The desired new supply key of the token.
   */
  std::optional<std::string> mSupplyKey;

  /**
   * The ID of the desired account to now pay auto-renewal fees.
   */
  std::optional<std::string> mAutoRenewAccountId;

  /**
   * The desired new interval of time for which to charge the auto-renew account to renew the token.
   *
   */
  std::optional<std::string> mAutoRenewPeriod;

  /**
   * The desired new expiration time of the token.
   */
  std::optional<std::string> mExpirationTime;

  /**
   * The desired new memo of the token.
   */
  std::optional<std::string> mMemo;

  /**
   * The desired new fee schedule key of the token.
   */
  std::optional<std::string> mFeeScheduleKey;

  /**
   * The desired new pause key of the token.
   */
  std::optional<std::string> mPauseKey;

  /**
   * The desired new metadata of the token.
   */
  std::optional<std::string> mMetadata;

  /**
   * The desired new metadata key of the token.
   */
  std::optional<std::string> mMetadataKey;

  /**
   * Any parameters common to all transaction types.
   */
  std::optional<CommonTransactionParams> mCommonTxParams;
};

} // namespace Hedera::TCK::TokenService

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert UpdateTokenParams arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<Hedera::TCK::TokenService::UpdateTokenParams>
{
  /**
   * Convert a JSON object to a UpdateTokenParams.
   *
   * @param jsonFrom The JSON object with which to fill the UpdateTokenParams.
   * @param params   The UpdateTokenParams to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, Hedera::TCK::TokenService::UpdateTokenParams& params)
  {
    params.mTokenId = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "tokenId");
    params.mSymbol = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "symbol");
    params.mName = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "name");
    params.mTreasuryAccountId = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "treasuryAccountId");
    params.mAdminKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "adminKey");
    params.mKycKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "kycKey");
    params.mFreezeKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "freezeKey");
    params.mWipeKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "wipeKey");
    params.mSupplyKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "supplyKey");
    params.mAutoRenewAccountId = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "autoRenewAccountId");
    params.mAutoRenewPeriod = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "autoRenewPeriod");
    params.mExpirationTime = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "expirationTime");
    params.mMemo = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "memo");
    params.mFeeScheduleKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "feeScheduleKey");
    params.mPauseKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "pauseKey");
    params.mMetadata = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "metadata");
    params.mMetadataKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "metadataKey");
    params.mCommonTxParams =
      Hedera::TCK::getOptionalJsonParameter<Hedera::TCK::CommonTransactionParams>(jsonFrom, "commonTransactionParams");
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_UPDATE_TOKEN_PARAMS_H_

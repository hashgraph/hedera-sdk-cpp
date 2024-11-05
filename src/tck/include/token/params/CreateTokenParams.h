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
#ifndef HEDERA_TCK_CPP_CREATE_TOKEN_PARAMS_H_
#define HEDERA_TCK_CPP_CREATE_TOKEN_PARAMS_H_

#include "common/CommonTransactionParams.h"
#include "token/CustomFeeSerializer.h"
#include "json/JsonUtils.h"

#include <CustomFee.h>

#include <cstdint>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

namespace Hedera::TCK::TokenService
{
/**
 * Struct to hold the arguments for a `createToken` JSON-RPC method call.
 */
struct CreateTokenParams
{
  /**
   * The desired name of the token.
   */
  std::optional<std::string> mName;

  /**
   * The desired symbol of the new token.
   */
  std::optional<std::string> mSymbol;

  /**
   * The number of decimal places by which the new token will be divisible.
   */
  std::optional<uint32_t> mDecimals;

  /**
   * The number of tokens to put into circulation upon creation.
   */
  std::optional<std::string> mInitialSupply;

  /**
   * The ID of the account which will act as the new token's treasury and will receive the specified initial supply of
   * the new token.
   */
  std::optional<std::string> mTreasuryAccountId;

  /**
   * The key which can perform update/delete operations on the new token. DER-encoded hex string representation for
   * private or public keys. Keylists and threshold keys are the hex of the serialized protobuf bytes..
   */
  std::optional<std::string> mAdminKey;

  /**
   * The key which can grant/revoke KYC on an account for transactions of the new token. DER-encoded hex string
   * representation for private or public keys. Keylists and threshold keys are the hex of the serialized protobuf
   * bytes.
   */
  std::optional<std::string> mKycKey;

  /**
   * The key which can freeze/unfreeze an account for transactions of the new token. DER-encoded hex string
   * representation for private or public keys. Keylists and threshold keys are the hex of the serialized protobuf
   * bytes.
   */
  std::optional<std::string> mFreezeKey;

  /**
   * The key which can wipe the balance of the new token from an account. DER-encoded hex string representation for
   * private or public keys. Keylists and threshold keys are the hex of the serialized protobuf bytes.
   */
  std::optional<std::string> mWipeKey;

  /**
   * The key which can change the supply of the new token. DER-encoded hex string representation for private or public
   * keys. Keylists and threshold keys are the hex of the serialized protobuf bytes.
   */
  std::optional<std::string> mSupplyKey;

  /**
   * Should accounts initially be frozen with respect to the new token?
   */
  std::optional<bool> mFreezeDefault;

  /**
   * The time at which the new token should expire. Epoch time in seconds.
   */
  std::optional<std::string> mExpirationTime;

  /**
   * The ID of the account that should be charged to renew the new token's expiration.
   */
  std::optional<std::string> mAutoRenewAccountId;

  /**
   * The interval at which the auto renew account will be charged to extend the new token's expiration. Units of
   * seconds.
   */
  std::optional<std::string> mAutoRenewPeriod;

  /**
   * The memo associated with the token.
   */
  std::optional<std::string> mMemo;

  /**
   * The type of the new token. MUST be one of "ft" (fungible token) or "nft" (non-fungible token).
   */
  std::optional<std::string> mTokenType;

  /**
   * The supply type of the new token. MUST be one of "infinite" or "finite".
   */
  std::optional<std::string> mSupplyType;

  /**
   * The maximum amount of the new token that can be in circulation (for fungible types) or minted (for NFTs).
   */
  std::optional<std::string> mMaxSupply;

  /**
   * The key which can change the new token's fee schedule. DER-encoded hex string representation for private or public
   * keys. Keylists and threshold keys are the hex of the serialized protobuf bytes.
   */
  std::optional<std::string> mFeeScheduleKey;

  /**
   * The fees to be assessed during a transfer of the new token.
   */
  std::optional<std::vector<std::shared_ptr<CustomFee>>> mCustomFees;

  /**
   * The key which can pause/unpause the new token. DER-encoded hex string representation for private or public keys.
   * Keylists and threshold keys are the hex of the serialized protobuf bytes.
   */
  std::optional<std::string> mPauseKey;

  /**
   * The metadata of the new token. Hex-encoded bytes of the metadata.
   */
  std::optional<std::string> mMetadata;

  /**
   * The key which can change the metadata of the new token and/or individual NFTs of the new token class. DER-encoded
   * hex string representation for private or public keys. Keylists and threshold keys are the hex of the serialized
   * protobuf bytes.
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
 * JSON serializer template specialization required to convert CreateTokenParams arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<Hedera::TCK::TokenService::CreateTokenParams>
{
  /**
   * Convert a JSON object to a CreateTokenParams.
   *
   * @param jsonFrom The JSON object with which to fill the CreateTokenParams.
   * @param params   The CreateTokenParams to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, Hedera::TCK::TokenService::CreateTokenParams& params)
  {
    params.mName = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "name");
    params.mSymbol = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "symbol");
    params.mDecimals = Hedera::TCK::getOptionalJsonParameter<uint32_t>(jsonFrom, "decimals");
    params.mInitialSupply = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "initialSupply");
    params.mTreasuryAccountId = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "treasuryAccountId");
    params.mAdminKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "adminKey");
    params.mKycKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "kycKey");
    params.mFreezeKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "freezeKey");
    params.mWipeKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "wipeKey");
    params.mSupplyKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "supplyKey");
    params.mFreezeDefault = Hedera::TCK::getOptionalJsonParameter<bool>(jsonFrom, "freezeDefault");
    params.mExpirationTime = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "expirationTime");
    params.mAutoRenewAccountId = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "autoRenewAccountId");
    params.mAutoRenewPeriod = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "autoRenewPeriod");
    params.mMemo = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "memo");
    params.mTokenType = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "tokenType");
    params.mSupplyType = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "supplyType");
    params.mMaxSupply = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "maxSupply");
    params.mFeeScheduleKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "feeScheduleKey");
    params.mCustomFees =
      Hedera::TCK::getOptionalJsonParameter<std::vector<std::shared_ptr<Hedera::CustomFee>>>(jsonFrom, "customFees");
    params.mPauseKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "pauseKey");
    params.mMetadata = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "metadata");
    params.mMetadataKey = Hedera::TCK::getOptionalJsonParameter<std::string>(jsonFrom, "metadataKey");
    params.mCommonTxParams =
      Hedera::TCK::getOptionalJsonParameter<Hedera::TCK::CommonTransactionParams>(jsonFrom, "commonTransactionParams");
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_CREATE_TOKEN_PARAMS_H_

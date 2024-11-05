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
#include "token/TokenService.h"
#include "key/KeyService.h"
#include "sdk/SdkClient.h"
#include "token/params/CreateTokenParams.h"
#include "token/params/UpdateTokenParams.h"
#include "json/JsonErrorType.h"
#include "json/JsonRpcException.h"

#include <AccountId.h>
#include <Status.h>
#include <TokenCreateTransaction.h>
#include <TokenSupplyType.h>
#include <TokenType.h>
#include <TokenUpdateTransaction.h>
#include <TransactionReceipt.h>
#include <TransactionResponse.h>
#include <impl/EntityIdHelper.h>
#include <impl/Utilities.h>

#include <chrono>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

namespace Hedera::TCK::TokenService
{
//-----
nlohmann::json createToken(const CreateTokenParams& params)
{
  TokenCreateTransaction tokenCreateTransaction;
  tokenCreateTransaction.setGrpcDeadline(SdkClient::DEFAULT_TCK_REQUEST_TIMEOUT);

  if (params.mName.has_value())
  {
    tokenCreateTransaction.setTokenName(params.mName.value());
  }

  if (params.mSymbol.has_value())
  {
    tokenCreateTransaction.setTokenSymbol(params.mSymbol.value());
  }

  if (params.mDecimals.has_value())
  {
    tokenCreateTransaction.setDecimals(params.mDecimals.value());
  }

  if (params.mInitialSupply.has_value())
  {
    tokenCreateTransaction.setInitialSupply(
      Hedera::internal::EntityIdHelper::getNum<int64_t>(params.mInitialSupply.value()));
  }

  if (params.mTreasuryAccountId.has_value())
  {
    tokenCreateTransaction.setTreasuryAccountId(AccountId::fromString(params.mTreasuryAccountId.value()));
  }

  if (params.mAdminKey.has_value())
  {
    tokenCreateTransaction.setAdminKey(KeyService::getHederaKey(params.mAdminKey.value()));
  }

  if (params.mKycKey.has_value())
  {
    tokenCreateTransaction.setKycKey(KeyService::getHederaKey(params.mKycKey.value()));
  }

  if (params.mFreezeKey.has_value())
  {
    tokenCreateTransaction.setFreezeKey(KeyService::getHederaKey(params.mFreezeKey.value()));
  }

  if (params.mWipeKey.has_value())
  {
    tokenCreateTransaction.setWipeKey(KeyService::getHederaKey(params.mWipeKey.value()));
  }

  if (params.mSupplyKey.has_value())
  {
    tokenCreateTransaction.setSupplyKey(KeyService::getHederaKey(params.mSupplyKey.value()));
  }

  if (params.mFreezeDefault.has_value())
  {
    tokenCreateTransaction.setFreezeDefault(params.mFreezeDefault.value());
  }

  if (params.mExpirationTime.has_value())
  {
    tokenCreateTransaction.setExpirationTime(
      std::chrono::system_clock::from_time_t(0) +
      std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum<int64_t>(params.mExpirationTime.value())));
  }

  if (params.mAutoRenewAccountId.has_value())
  {
    tokenCreateTransaction.setAutoRenewAccountId(AccountId::fromString(params.mAutoRenewAccountId.value()));
  }

  if (params.mAutoRenewPeriod.has_value())
  {
    tokenCreateTransaction.setAutoRenewPeriod(
      std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum<int64_t>(params.mAutoRenewPeriod.value())));
  }

  if (params.mMemo.has_value())
  {
    tokenCreateTransaction.setTokenMemo(params.mMemo.value());
  }

  if (params.mTokenType.has_value())
  {
    if (params.mTokenType.value() != "ft" && params.mTokenType.value() != "nft")
    {
      throw JsonRpcException(JsonErrorType::INVALID_PARAMS, "invalid params: tokenType MUST be one of ft or nft.");
    }

    tokenCreateTransaction.setTokenType(params.mTokenType.value() == "ft" ? TokenType::FUNGIBLE_COMMON
                                                                          : TokenType::NON_FUNGIBLE_UNIQUE);
  }

  if (params.mSupplyType.has_value())
  {
    if (params.mSupplyType.value() != "infinite" && params.mSupplyType.value() != "finite")
    {
      throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                             "invalid params: supplyType MUST be one of infinite or finite.");
    }

    tokenCreateTransaction.setSupplyType(params.mSupplyType.value() == "finite" ? TokenSupplyType::FINITE
                                                                                : TokenSupplyType::INFINITE);
  }

  if (params.mMaxSupply.has_value())
  {
    tokenCreateTransaction.setMaxSupply(Hedera::internal::EntityIdHelper::getNum(params.mSupplyType.value()));
  }

  if (params.mFeeScheduleKey.has_value())
  {
    tokenCreateTransaction.setFeeScheduleKey(KeyService::getHederaKey(params.mFeeScheduleKey.value()));
  }

  if (params.mCustomFees.has_value())
  {
    tokenCreateTransaction.setCustomFees(params.mCustomFees.value());
  }

  if (params.mPauseKey.has_value())
  {
    tokenCreateTransaction.setPauseKey(KeyService::getHederaKey(params.mPauseKey.value()));
  }

  if (params.mMetadata.has_value())
  {
    tokenCreateTransaction.setMetadata(internal::Utilities::stringToByteVector(params.mMetadata.value()));
  }

  if (params.mMetadataKey.has_value())
  {
    tokenCreateTransaction.setMetadataKey(KeyService::getHederaKey(params.mMetadataKey.value()));
  }

  if (params.mCommonTxParams.has_value())
  {
    params.mCommonTxParams->fillOutTransaction(tokenCreateTransaction, SdkClient::getClient());
  }

  const TransactionReceipt txReceipt =
    tokenCreateTransaction.execute(SdkClient::getClient()).getReceipt(SdkClient::getClient());
  return {
    {"tokenId", txReceipt.mTokenId->toString()       },
    { "status", gStatusToString.at(txReceipt.mStatus)}
  };
}

//-----
nlohmann::json updateToken(const UpdateTokenParams& params)
{
  TokenUpdateTransaction tokenUpdateTransaction;
  tokenUpdateTransaction.setGrpcDeadline(std::chrono::seconds(SdkClient::DEFAULT_TCK_REQUEST_TIMEOUT));

  if (params.mTokenId.has_value())
  {
    tokenUpdateTransaction.setTokenId(TokenId::fromString(params.mTokenId.value()));
  }

  if (params.mSymbol.has_value())
  {
    tokenUpdateTransaction.setTokenSymbol(params.mSymbol.value());
  }

  if (params.mName.has_value())
  {
    tokenUpdateTransaction.setTokenName(params.mName.value());
  }

  if (params.mTreasuryAccountId.has_value())
  {
    tokenUpdateTransaction.setTreasuryAccountId(AccountId::fromString(params.mTreasuryAccountId.value()));
  }

  if (params.mAdminKey.has_value())
  {
    tokenUpdateTransaction.setAdminKey(KeyService::getHederaKey(params.mAdminKey.value()));
  }

  if (params.mKycKey.has_value())
  {
    tokenUpdateTransaction.setKycKey(KeyService::getHederaKey(params.mKycKey.value()));
  }

  if (params.mFreezeKey.has_value())
  {
    tokenUpdateTransaction.setFreezeKey(KeyService::getHederaKey(params.mFreezeKey.value()));
  }

  if (params.mWipeKey.has_value())
  {
    tokenUpdateTransaction.setWipeKey(KeyService::getHederaKey(params.mWipeKey.value()));
  }

  if (params.mSupplyKey.has_value())
  {
    tokenUpdateTransaction.setSupplyKey(KeyService::getHederaKey(params.mSupplyKey.value()));
  }

  if (params.mAutoRenewAccountId.has_value())
  {
    tokenUpdateTransaction.setAutoRenewAccountId(AccountId::fromString(params.mAutoRenewAccountId.value()));
  }

  if (params.mAutoRenewPeriod.has_value())
  {
    tokenUpdateTransaction.setAutoRenewPeriod(
      std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum<int64_t>(params.mAutoRenewPeriod.value())));
  }

  if (params.mExpirationTime.has_value())
  {
    tokenUpdateTransaction.setExpirationTime(
      std::chrono::system_clock::from_time_t(0) +
      std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum<int64_t>(params.mExpirationTime.value())));
  }

  if (params.mMemo.has_value())
  {
    tokenUpdateTransaction.setTokenMemo(params.mMemo.value());
  }

  if (params.mFeeScheduleKey.has_value())
  {
    tokenUpdateTransaction.setFeeScheduleKey(KeyService::getHederaKey(params.mFeeScheduleKey.value()));
  }

  if (params.mPauseKey.has_value())
  {
    tokenUpdateTransaction.setPauseKey(KeyService::getHederaKey(params.mPauseKey.value()));
  }

  if (params.mMetadata.has_value())
  {
    tokenUpdateTransaction.setMetadata(internal::Utilities::stringToByteVector(params.mMetadata.value()));
  }

  if (params.mMetadataKey.has_value())
  {
    tokenUpdateTransaction.setMetadataKey(KeyService::getHederaKey(params.mMetadataKey.value()));
  }

  if (params.mCommonTxParams.has_value())
  {
    params.mCommonTxParams->fillOutTransaction(tokenUpdateTransaction, SdkClient::getClient());
  }

  return {
    {"status",
     gStatusToString.at(
        tokenUpdateTransaction.execute(SdkClient::getClient()).getReceipt(SdkClient::getClient()).mStatus)}
  };
}

} // namespace Hedera::TCK::TokenService

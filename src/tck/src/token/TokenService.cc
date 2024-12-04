// SPDX-License-Identifier: Apache-2.0
#include "token/TokenService.h"
#include "key/KeyService.h"
#include "sdk/SdkClient.h"
#include "token/params/AssociateTokenParams.h"
#include "token/params/CreateTokenParams.h"
#include "token/params/DeleteTokenParams.h"
#include "token/params/GrantTokenKycParams.h"
#include "token/params/UpdateTokenParams.h"
#include "json/JsonErrorType.h"
#include "json/JsonRpcException.h"

#include <AccountId.h>
#include <Status.h>
#include <TokenAssociateTransaction.h>
#include <TokenCreateTransaction.h>
#include <TokenDeleteTransaction.h>
#include <TokenGrantKycTransaction.h>
#include <TokenId.h>
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
#include <vector>

namespace Hiero::TCK::TokenService
{
//-----
nlohmann::json associateToken(const AssociateTokenParams& params)
{
  TokenAssociateTransaction tokenAssociateTransaction;
  tokenAssociateTransaction.setGrpcDeadline(SdkClient::DEFAULT_TCK_REQUEST_TIMEOUT);

  if (params.mAccountId.has_value())
  {
    tokenAssociateTransaction.setAccountId(AccountId::fromString(params.mAccountId.value()));
  }

  if (params.mTokenIds.has_value())
  {
    std::vector<TokenId> tokenIds;
    for (const std::string& tokenId : params.mTokenIds.value())
    {
      tokenIds.push_back(TokenId::fromString(tokenId));
    }

    tokenAssociateTransaction.setTokenIds(tokenIds);
  }

  if (params.mCommonTxParams.has_value())
  {
    params.mCommonTxParams->fillOutTransaction(tokenAssociateTransaction, SdkClient::getClient());
  }

  return {
    {"status",
     gStatusToString.at(
        tokenAssociateTransaction.execute(SdkClient::getClient()).getReceipt(SdkClient::getClient()).mStatus)}
  };
}

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
      Hiero::internal::EntityIdHelper::getNum<int64_t>(params.mInitialSupply.value()));
  }

  if (params.mTreasuryAccountId.has_value())
  {
    tokenCreateTransaction.setTreasuryAccountId(AccountId::fromString(params.mTreasuryAccountId.value()));
  }

  if (params.mAdminKey.has_value())
  {
    tokenCreateTransaction.setAdminKey(KeyService::getHieroKey(params.mAdminKey.value()));
  }

  if (params.mKycKey.has_value())
  {
    tokenCreateTransaction.setKycKey(KeyService::getHieroKey(params.mKycKey.value()));
  }

  if (params.mFreezeKey.has_value())
  {
    tokenCreateTransaction.setFreezeKey(KeyService::getHieroKey(params.mFreezeKey.value()));
  }

  if (params.mWipeKey.has_value())
  {
    tokenCreateTransaction.setWipeKey(KeyService::getHieroKey(params.mWipeKey.value()));
  }

  if (params.mSupplyKey.has_value())
  {
    tokenCreateTransaction.setSupplyKey(KeyService::getHieroKey(params.mSupplyKey.value()));
  }

  if (params.mFreezeDefault.has_value())
  {
    tokenCreateTransaction.setFreezeDefault(params.mFreezeDefault.value());
  }

  if (params.mExpirationTime.has_value())
  {
    tokenCreateTransaction.setExpirationTime(
      std::chrono::system_clock::from_time_t(0) +
      std::chrono::seconds(Hiero::internal::EntityIdHelper::getNum<int64_t>(params.mExpirationTime.value())));
  }

  if (params.mAutoRenewAccountId.has_value())
  {
    tokenCreateTransaction.setAutoRenewAccountId(AccountId::fromString(params.mAutoRenewAccountId.value()));
  }

  if (params.mAutoRenewPeriod.has_value())
  {
    tokenCreateTransaction.setAutoRenewPeriod(
      std::chrono::seconds(Hiero::internal::EntityIdHelper::getNum<int64_t>(params.mAutoRenewPeriod.value())));
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
    tokenCreateTransaction.setMaxSupply(Hiero::internal::EntityIdHelper::getNum<int64_t>(params.mMaxSupply.value()));
  }

  if (params.mFeeScheduleKey.has_value())
  {
    tokenCreateTransaction.setFeeScheduleKey(KeyService::getHieroKey(params.mFeeScheduleKey.value()));
  }

  if (params.mCustomFees.has_value())
  {
    tokenCreateTransaction.setCustomFees(params.mCustomFees.value());
  }

  if (params.mPauseKey.has_value())
  {
    tokenCreateTransaction.setPauseKey(KeyService::getHieroKey(params.mPauseKey.value()));
  }

  if (params.mMetadata.has_value())
  {
    tokenCreateTransaction.setMetadata(internal::Utilities::stringToByteVector(params.mMetadata.value()));
  }

  if (params.mMetadataKey.has_value())
  {
    tokenCreateTransaction.setMetadataKey(KeyService::getHieroKey(params.mMetadataKey.value()));
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
nlohmann::json deleteToken(const DeleteTokenParams& params)
{
  TokenDeleteTransaction tokenDeleteTransaction;
  tokenDeleteTransaction.setGrpcDeadline(std::chrono::seconds(SdkClient::DEFAULT_TCK_REQUEST_TIMEOUT));

  if (params.mTokenId.has_value())
  {
    tokenDeleteTransaction.setTokenId(TokenId::fromString(params.mTokenId.value()));
  }

  if (params.mCommonTxParams.has_value())
  {
    params.mCommonTxParams->fillOutTransaction(tokenDeleteTransaction, SdkClient::getClient());
  }

  return {
    {"status",
     gStatusToString.at(
        tokenDeleteTransaction.execute(SdkClient::getClient()).getReceipt(SdkClient::getClient()).mStatus)}
  };
}

//-----
nlohmann::json grantTokenKyc(const GrantTokenKycParams& params)
{
  TokenGrantKycTransaction tokenGrantKycTransaction;
  tokenGrantKycTransaction.setGrpcDeadline(std::chrono::seconds(SdkClient::DEFAULT_TCK_REQUEST_TIMEOUT));

  if (params.mTokenId.has_value())
  {
    tokenGrantKycTransaction.setTokenId(TokenId::fromString(params.mTokenId.value()));
  }

  if (params.mAccountId.has_value())
  {
    tokenGrantKycTransaction.setAccountId(AccountId::fromString(params.mAccountId.value()));
  }

  if (params.mCommonTxParams.has_value())
  {
    params.mCommonTxParams->fillOutTransaction(tokenGrantKycTransaction, SdkClient::getClient());
  }

  return {
    {"status",
     gStatusToString.at(
        tokenGrantKycTransaction.execute(SdkClient::getClient()).getReceipt(SdkClient::getClient()).mStatus)}
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
    tokenUpdateTransaction.setAdminKey(KeyService::getHieroKey(params.mAdminKey.value()));
  }

  if (params.mKycKey.has_value())
  {
    tokenUpdateTransaction.setKycKey(KeyService::getHieroKey(params.mKycKey.value()));
  }

  if (params.mFreezeKey.has_value())
  {
    tokenUpdateTransaction.setFreezeKey(KeyService::getHieroKey(params.mFreezeKey.value()));
  }

  if (params.mWipeKey.has_value())
  {
    tokenUpdateTransaction.setWipeKey(KeyService::getHieroKey(params.mWipeKey.value()));
  }

  if (params.mSupplyKey.has_value())
  {
    tokenUpdateTransaction.setSupplyKey(KeyService::getHieroKey(params.mSupplyKey.value()));
  }

  if (params.mAutoRenewAccountId.has_value())
  {
    tokenUpdateTransaction.setAutoRenewAccountId(AccountId::fromString(params.mAutoRenewAccountId.value()));
  }

  if (params.mAutoRenewPeriod.has_value())
  {
    tokenUpdateTransaction.setAutoRenewPeriod(
      std::chrono::seconds(Hiero::internal::EntityIdHelper::getNum<int64_t>(params.mAutoRenewPeriod.value())));
  }

  if (params.mExpirationTime.has_value())
  {
    tokenUpdateTransaction.setExpirationTime(
      std::chrono::system_clock::from_time_t(0) +
      std::chrono::seconds(Hiero::internal::EntityIdHelper::getNum<int64_t>(params.mExpirationTime.value())));
  }

  if (params.mMemo.has_value())
  {
    tokenUpdateTransaction.setTokenMemo(params.mMemo.value());
  }

  if (params.mFeeScheduleKey.has_value())
  {
    tokenUpdateTransaction.setFeeScheduleKey(KeyService::getHieroKey(params.mFeeScheduleKey.value()));
  }

  if (params.mPauseKey.has_value())
  {
    tokenUpdateTransaction.setPauseKey(KeyService::getHieroKey(params.mPauseKey.value()));
  }

  if (params.mMetadata.has_value())
  {
    tokenUpdateTransaction.setMetadata(internal::Utilities::stringToByteVector(params.mMetadata.value()));
  }

  if (params.mMetadataKey.has_value())
  {
    tokenUpdateTransaction.setMetadataKey(KeyService::getHieroKey(params.mMetadataKey.value()));
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

} // namespace Hiero::TCK::TokenService

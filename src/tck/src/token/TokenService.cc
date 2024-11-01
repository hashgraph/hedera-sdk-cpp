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

#include <AccountCreateTransaction.h>
#include <AccountDeleteTransaction.h>
#include <AccountId.h>
#include <AccountUpdateTransaction.h>
#include <EvmAddress.h>
#include <HbarUnit.h>
#include <Status.h>
#include <TransactionReceipt.h>
#include <TransactionResponse.h>

#include <chrono>
#include <nlohmann/json.hpp>
#include <string>

namespace Hedera::TCK::TokenService
{
//-----
nlohmann::json createToken(const std::optional<std::string>& name,
                           const std::optional<std::string>& symbol,
                           const std::optional<uint32_t>& decimals,
                           const std::optional<std::string>& initialSupply,
                           const std::optional<std::string>& treasuryAccountId,
                           const std::optional<std::string>& adminKey,
                           const std::optional<std::string>& kycKey,
                           const std::optional<std::string>& freezeKey,
                           const std::optional<std::string>& wipeKey,
                           const std::optional<std::string>& supplyKey,
                           const std::optional<bool>& freezeDefault,
                           const std::optional<std::string>& expirationTime,
                           const std::optional<std::string>& autoRenewAccountId,
                           const std::optional<std::string>& autoRenewPeriod,
                           const std::optional<std::string>& memo,
                           const std::optional<std::string>& tokenType,
                           const std::optional<std::string>& supplyType,
                           const std::optional<std::string>& maxSupply,
                           const std::optional<std::string>& feeScheduleKey,
                           const std::optional<std::vector<std::shared_ptr<CustomFee>>>& customFees,
                           const std::optional<std::string>& pauseKey,
                           const std::optional<std::string>& metadata,
                           const std::optional<std::string>& metadataKey,
                           const std::optional<CommonTransactionParams>& commonTxParams)
{
  TokenCreateTransaction tokenCreateTransaction;
  tokenCreateTransaction.setGrpcDeadline(DEFAULT_TCK_REQUEST_TIMEOUT);

  if (name.has_value())
  {
    tokenCreateTransaction.setTokenName(name.value());
  }

  if (symbol.has_value())
  {
    tokenCreateTransaction.setTokenSymbol(symbol.value());
  }

  if (decimals.has_value())
  {
    tokenCreateTransaction.setDecimals(decimals.value());
  }

  if (initialSupply.has_value())
  {
    try
    {
      tokenCreateTransaction.setInitialSupply(Hedera::internal::EntityIdHelper::getNum<int64_t>(initialSupply.value()));
    }
    catch (const std::invalid_argument&)
    {
      tokenCreateTransaction.setInitialSupply(Hedera::internal::EntityIdHelper::getNum(initialSupply.value()));
    }
  }

  if (treasuryAccountId.has_value())
  {
    tokenCreateTransaction.setTreasuryAccountId(AccountId::fromString(treasuryAccountId.value()));
  }

  if (adminKey.has_value())
  {
    tokenCreateTransaction.setAdminKey(getHederaKey(adminKey.value()));
  }

  if (kycKey.has_value())
  {
    tokenCreateTransaction.setKycKey(getHederaKey(kycKey.value()));
  }

  if (freezeKey.has_value())
  {
    tokenCreateTransaction.setFreezeKey(getHederaKey(freezeKey.value()));
  }

  if (wipeKey.has_value())
  {
    tokenCreateTransaction.setWipeKey(getHederaKey(wipeKey.value()));
  }

  if (supplyKey.has_value())
  {
    tokenCreateTransaction.setSupplyKey(getHederaKey(supplyKey.value()));
  }

  if (freezeDefault.has_value())
  {
    tokenCreateTransaction.setFreezeDefault(freezeDefault.value());
  }

  if (expirationTime.has_value())
  {
    try
    {
      tokenCreateTransaction.setExpirationTime(
        std::chrono::system_clock::from_time_t(0) +
        std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum<int64_t>(expirationTime.value())));
    }
    catch (const std::invalid_argument&)
    {
      tokenCreateTransaction.setExpirationTime(
        std::chrono::system_clock::from_time_t(0) +
        std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum(expirationTime.value())));
    }
  }

  if (autoRenewAccountId.has_value())
  {
    tokenCreateTransaction.setAutoRenewAccountId(AccountId::fromString(autoRenewAccountId.value()));
  }

  if (autoRenewPeriod.has_value())
  {
    try
    {
      tokenCreateTransaction.setAutoRenewPeriod(
        std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum<int64_t>(autoRenewPeriod.value())));
    }
    catch (const std::invalid_argument&)
    {
      tokenCreateTransaction.setAutoRenewPeriod(
        std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum(autoRenewPeriod.value())));
    }
  }

  if (memo.has_value())
  {
    tokenCreateTransaction.setTokenMemo(memo.value());
  }

  if (tokenType.has_value())
  {
    if (tokenType.value() != "ft" && tokenType.value() != "nft")
    {
      throw JsonRpcException(JsonErrorType::INVALID_PARAMS, "invalid params: tokenType MUST be one of ft or nft.");
    }

    tokenCreateTransaction.setTokenType(tokenType.value() == "ft" ? TokenType::FUNGIBLE_COMMON
                                                                  : TokenType::NON_FUNGIBLE_UNIQUE);
  }

  if (supplyType.has_value())
  {
    if (supplyType.value() != "infinite" && supplyType.value() != "finite")
    {
      throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                             "invalid params: supplyType MUST be one of infinite or finite.");
    }

    tokenCreateTransaction.setSupplyType(supplyType.value() == "finite" ? TokenSupplyType::FINITE
                                                                        : TokenSupplyType::INFINITE);
  }

  if (maxSupply.has_value())
  {
    try
    {
      tokenCreateTransaction.setMaxSupply(Hedera::internal::EntityIdHelper::getNum<int64_t>(maxSupply.value()));
    }
    catch (const std::invalid_argument&)
    {
      tokenCreateTransaction.setMaxSupply(Hedera::internal::EntityIdHelper::getNum(maxSupply.value()));
    }
  }

  if (feeScheduleKey.has_value())
  {
    tokenCreateTransaction.setFeeScheduleKey(getHederaKey(feeScheduleKey.value()));
  }

  if (customFees.has_value())
  {
    tokenCreateTransaction.setCustomFees(customFees.value());
  }

  if (pauseKey.has_value())
  {
    tokenCreateTransaction.setPauseKey(getHederaKey(pauseKey.value()));
  }

  if (metadata.has_value())
  {
    tokenCreateTransaction.setMetadata(internal::Utilities::stringToByteVector(metadata.value()));
  }

  if (metadataKey.has_value())
  {
    tokenCreateTransaction.setMetadataKey(getHederaKey(metadataKey.value()));
  }

  if (commonTxParams.has_value())
  {
    commonTxParams->fillOutTransaction(tokenCreateTransaction, mClient);
  }

  const TransactionReceipt txReceipt = tokenCreateTransaction.execute(mClient).getReceipt(mClient);
  return {
    {"tokenId", txReceipt.mTokenId->toString()       },
    { "status", gStatusToString.at(txReceipt.mStatus)}
  };
}

} // namespace Hedera::TCK::TokenService

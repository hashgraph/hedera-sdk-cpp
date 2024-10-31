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
#include "SdkClient.h"
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountId.h"
#include "AccountUpdateTransaction.h"
#include "Client.h"
#include "EvmAddress.h"
#include "HbarUnit.h"
#include "JsonErrorType.h"
#include "JsonRpcException.h"
#include "KeyHelper.h"
#include "PrivateKey.h"
#include "Status.h"
#include "TokenCreateTransaction.h"
#include "TokenSupplyType.h"
#include "TokenType.h"
#include "TokenUpdateTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "impl/EntityIdHelper.h"
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

#include <algorithm>
#include <chrono>
#include <memory>
#include <nlohmann/json.hpp>
#include <proto/basic_types.pb.h>
#include <stdexcept>
#include <string>

namespace Hedera::TCK
{
namespace
{
// The default amount of time the SdkClient should wait for a gRPC request.
constexpr auto DEFAULT_TCK_REQUEST_TIMEOUT = std::chrono::seconds(30);

// The SDK Client to use to submit requests to a Hedera network.
Client mClient; // NOLINT
} // namespace

//-----
nlohmann::json SdkClient::createAccount(const std::optional<std::string>& key,
                                        const std::optional<int64_t>& initialBalance,
                                        const std::optional<bool>& receiverSignatureRequired,
                                        const std::optional<int64_t>& autoRenewPeriod,
                                        const std::optional<std::string>& memo,
                                        const std::optional<int32_t>& maxAutoTokenAssociations,
                                        const std::optional<std::string>& stakedAccountId,
                                        const std::optional<int64_t>& stakedNodeId,
                                        const std::optional<bool>& declineStakingReward,
                                        const std::optional<std::string>& alias,
                                        const std::optional<CommonTransactionParams>& commonTxParams)
{
  AccountCreateTransaction accountCreateTransaction;
  accountCreateTransaction.setGrpcDeadline(DEFAULT_TCK_REQUEST_TIMEOUT);

  if (key.has_value())
  {
    accountCreateTransaction.setKey(getHederaKey(key.value()));
  }

  if (initialBalance.has_value())
  {
    accountCreateTransaction.setInitialBalance(Hbar(initialBalance.value(), HbarUnit::TINYBAR()));
  }

  if (receiverSignatureRequired.has_value())
  {
    accountCreateTransaction.setReceiverSignatureRequired(receiverSignatureRequired.value());
  }

  if (autoRenewPeriod.has_value())
  {
    accountCreateTransaction.setAutoRenewPeriod(std::chrono::seconds(autoRenewPeriod.value()));
  }

  if (memo.has_value())
  {
    accountCreateTransaction.setAccountMemo(memo.value());
  }

  if (maxAutoTokenAssociations.has_value())
  {
    accountCreateTransaction.setMaxAutomaticTokenAssociations(maxAutoTokenAssociations.value());
  }

  if (stakedAccountId.has_value())
  {
    accountCreateTransaction.setStakedAccountId(AccountId::fromString(stakedAccountId.value()));
  }

  if (stakedNodeId.has_value())
  {
    accountCreateTransaction.setStakedNodeId(stakedNodeId.value());
  }

  if (declineStakingReward.has_value())
  {
    accountCreateTransaction.setDeclineStakingReward(declineStakingReward.value());
  }

  if (alias.has_value())
  {
    accountCreateTransaction.setAlias(EvmAddress::fromString(alias.value()));
  }

  if (commonTxParams.has_value())
  {
    commonTxParams->fillOutTransaction(accountCreateTransaction, mClient);
  }

  const TransactionReceipt txReceipt = accountCreateTransaction.execute(mClient).getReceipt(mClient);
  return {
    {"accountId", txReceipt.mAccountId->toString()     },
    { "status",   gStatusToString.at(txReceipt.mStatus)}
  };
}

//-----
nlohmann::json SdkClient::createToken(const std::optional<std::string>& name,
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

//-----
nlohmann::json SdkClient::deleteAccount(const std::optional<std::string>& deleteAccountId,
                                        const std::optional<std::string>& transferAccountId,
                                        const std::optional<CommonTransactionParams>& commonTxParams)
{
  AccountDeleteTransaction accountDeleteTransaction;
  accountDeleteTransaction.setGrpcDeadline(std::chrono::seconds(DEFAULT_TCK_REQUEST_TIMEOUT));

  if (deleteAccountId.has_value())
  {
    accountDeleteTransaction.setDeleteAccountId(AccountId::fromString(deleteAccountId.value()));
  }

  if (transferAccountId.has_value())
  {
    accountDeleteTransaction.setTransferAccountId(AccountId::fromString(transferAccountId.value()));
  }

  if (commonTxParams.has_value())
  {
    commonTxParams->fillOutTransaction(accountDeleteTransaction, mClient);
  }

  return {
    {"status", gStatusToString.at(accountDeleteTransaction.execute(mClient).getReceipt(mClient).mStatus)}
  };
}

//-----
nlohmann::json SdkClient::generateKey(const std::string& type,
                                      const std::optional<std::string>& fromKey,
                                      const std::optional<int>& threshold,
                                      const std::optional<std::vector<KeyRequest>>& keys)
{
  nlohmann::json response;
  std::string key = processKeyRequest({ type, fromKey, threshold, keys }, response);
  std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
  response["key"] = key;
  return response;
}

//-----
nlohmann::json SdkClient::reset()
{
  mClient.close();
  return {
    {"status", "SUCCESS"}
  };
}

//-----
nlohmann::json SdkClient::setup(const std::string& operatorAccountId,
                                const std::string& operatorPrivateKey,
                                const std::optional<std::string>& nodeIp,
                                const std::optional<std::string>& nodeAccountId,
                                const std::optional<std::string>& mirrorNetworkIp)
{
  std::string clientType;

  if (nodeIp.has_value() && nodeAccountId.has_value() && mirrorNetworkIp.has_value())
  {
    mClient = Client::forNetwork({
      {nodeIp.value(), Hedera::AccountId::fromString(nodeAccountId.value())}
    });
    mClient.setMirrorNetwork({ mirrorNetworkIp.value() });
    clientType = "custom";
  }
  else
  {
    mClient = Client::forTestnet();
    clientType = "testnet";
  }

  mClient.setOperator(AccountId::fromString(operatorAccountId), PrivateKey::fromStringDer(operatorPrivateKey));
  mClient.setRequestTimeout(DEFAULT_TCK_REQUEST_TIMEOUT);

  return {
    {"message", "Successfully setup " + clientType + " client."},
    { "status", "SUCCESS"                                      }
  };
}

//-----
nlohmann::json SdkClient::updateAccount(const std::optional<std::string>& accountId,
                                        const std::optional<std::string>& key,
                                        const std::optional<int64_t>& autoRenewPeriod,
                                        const std::optional<int64_t>& expirationTime,
                                        const std::optional<bool>& receiverSignatureRequired,
                                        const std::optional<std::string>& memo,
                                        const std::optional<int32_t>& maxAutoTokenAssociations,
                                        const std::optional<std::string>& stakedAccountId,
                                        const std::optional<int64_t>& stakedNodeId,
                                        const std::optional<bool>& declineStakingReward,
                                        const std::optional<CommonTransactionParams>& commonTxParams)
{
  AccountUpdateTransaction accountUpdateTransaction;
  accountUpdateTransaction.setGrpcDeadline(std::chrono::seconds(DEFAULT_TCK_REQUEST_TIMEOUT));

  if (accountId.has_value())
  {
    accountUpdateTransaction.setAccountId(AccountId::fromString(accountId.value()));
  }

  if (key.has_value())
  {
    accountUpdateTransaction.setKey(getHederaKey(key.value()));
  }

  if (autoRenewPeriod.has_value())
  {
    accountUpdateTransaction.setAutoRenewPeriod(std::chrono::seconds(autoRenewPeriod.value()));
  }

  if (expirationTime.has_value())
  {
    accountUpdateTransaction.setExpirationTime(std::chrono::system_clock::from_time_t(0) +
                                               std::chrono::seconds(expirationTime.value()));
  }

  if (receiverSignatureRequired.has_value())
  {
    accountUpdateTransaction.setReceiverSignatureRequired(receiverSignatureRequired.value());
  }

  if (memo.has_value())
  {
    accountUpdateTransaction.setAccountMemo(memo.value());
  }

  if (maxAutoTokenAssociations.has_value())
  {
    accountUpdateTransaction.setMaxAutomaticTokenAssociations(maxAutoTokenAssociations.value());
  }

  if (stakedAccountId.has_value())
  {
    accountUpdateTransaction.setStakedAccountId(AccountId::fromString(stakedAccountId.value()));
  }

  if (stakedNodeId.has_value())
  {
    accountUpdateTransaction.setStakedNodeId(stakedNodeId.value());
  }

  if (declineStakingReward.has_value())
  {
    accountUpdateTransaction.setDeclineStakingReward(declineStakingReward.value());
  }

  if (commonTxParams.has_value())
  {
    commonTxParams->fillOutTransaction(accountUpdateTransaction, mClient);
  }

  const TransactionReceipt txReceipt = accountUpdateTransaction.execute(mClient).getReceipt(mClient);
  return {
    {"status", gStatusToString.at(txReceipt.mStatus)}
  };
}

//-----
nlohmann::json SdkClient::updateToken(const std::optional<std::string>& tokenId,
                                      const std::optional<std::string>& symbol,
                                      const std::optional<std::string>& name,
                                      const std::optional<std::string>& treasuryAccountId,
                                      const std::optional<std::string>& adminKey,
                                      const std::optional<std::string>& kycKey,
                                      const std::optional<std::string>& freezeKey,
                                      const std::optional<std::string>& wipeKey,
                                      const std::optional<std::string>& supplyKey,
                                      const std::optional<std::string>& autoRenewAccountId,
                                      const std::optional<std::string>& autoRenewPeriod,
                                      const std::optional<std::string>& expirationTime,
                                      const std::optional<std::string>& memo,
                                      const std::optional<std::string>& feeScheduleKey,
                                      const std::optional<std::string>& pauseKey,
                                      const std::optional<std::string>& metadata,
                                      const std::optional<std::string>& metadataKey,
                                      const std::optional<CommonTransactionParams>& commonTxParams)
{
  TokenUpdateTransaction tokenUpdateTransaction;
  tokenUpdateTransaction.setGrpcDeadline(std::chrono::seconds(DEFAULT_TCK_REQUEST_TIMEOUT));

  if (tokenId.has_value())
  {
    tokenUpdateTransaction.setTokenId(TokenId::fromString(tokenId.value()));
  }

  if (symbol.has_value())
  {
    tokenUpdateTransaction.setTokenSymbol(symbol.value());
  }

  if (name.has_value())
  {
    tokenUpdateTransaction.setTokenName(name.value());
  }

  if (treasuryAccountId.has_value())
  {
    tokenUpdateTransaction.setTreasuryAccountId(AccountId::fromString(treasuryAccountId.value()));
  }

  if (adminKey.has_value())
  {
    tokenUpdateTransaction.setAdminKey(getHederaKey(adminKey.value()));
  }

  if (kycKey.has_value())
  {
    tokenUpdateTransaction.setKycKey(getHederaKey(kycKey.value()));
  }

  if (freezeKey.has_value())
  {
    tokenUpdateTransaction.setFreezeKey(getHederaKey(freezeKey.value()));
  }

  if (wipeKey.has_value())
  {
    tokenUpdateTransaction.setWipeKey(getHederaKey(wipeKey.value()));
  }

  if (supplyKey.has_value())
  {
    tokenUpdateTransaction.setSupplyKey(getHederaKey(supplyKey.value()));
  }

  if (autoRenewAccountId.has_value())
  {
    tokenUpdateTransaction.setAutoRenewAccountId(AccountId::fromString(autoRenewAccountId.value()));
  }

  if (autoRenewPeriod.has_value())
  {
    try
    {
      tokenUpdateTransaction.setAutoRenewPeriod(
        std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum<int64_t>(autoRenewPeriod.value())));
    }
    catch (const std::invalid_argument&)
    {
      tokenUpdateTransaction.setAutoRenewPeriod(
        std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum(autoRenewPeriod.value())));
    }
  }

  if (expirationTime.has_value())
  {
    try
    {
      tokenUpdateTransaction.setExpirationTime(
        std::chrono::system_clock::from_time_t(0) +
        std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum<int64_t>(expirationTime.value())));
    }
    catch (const std::invalid_argument&)
    {
      tokenUpdateTransaction.setExpirationTime(
        std::chrono::system_clock::from_time_t(0) +
        std::chrono::seconds(Hedera::internal::EntityIdHelper::getNum(expirationTime.value())));
    }
  }

  if (memo.has_value())
  {
    tokenUpdateTransaction.setTokenMemo(memo.value());
  }

  if (feeScheduleKey.has_value())
  {
    tokenUpdateTransaction.setFeeScheduleKey(getHederaKey(feeScheduleKey.value()));
  }

  if (pauseKey.has_value())
  {
    tokenUpdateTransaction.setPauseKey(getHederaKey(pauseKey.value()));
  }

  if (metadata.has_value())
  {
    tokenUpdateTransaction.setMetadata(internal::HexConverter::hexToBytes(metadata.value()));
  }

  if (metadataKey.has_value())
  {
    tokenUpdateTransaction.setMetadataKey(getHederaKey(metadataKey.value()));
  }

  if (commonTxParams.has_value())
  {
    commonTxParams->fillOutTransaction(tokenUpdateTransaction, mClient);
  }

  const TransactionReceipt txReceipt = tokenUpdateTransaction.execute(mClient).getReceipt(mClient);
  return {
    {"status", gStatusToString.at(txReceipt.mStatus)}
  };
}

} // namespace Hedera::TCK::SdkClient

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
#include "account/AccountService.h"
#include "CommonTransactionParams.h"
#include "KeyHelper.h"
#include "SdkClient.h"
#include "account/params/CreateAccountParams.h"

#include <AccountCreateTransaction.h>
#include <AccountDeleteTransaction.h>
#include <AccountId.h>
#include <AccountUpdateTransaction.h>
#include <Status.h>
#include <TransactionReceipt.h>
#include <TransactionResponse.h>

#include <chrono>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace Hedera::TCK
{
//-----
nlohmann::json AccountService::createAccount(const CreateAccountParams& params)
{
  AccountCreateTransaction accountCreateTransaction;
  accountCreateTransaction.setGrpcDeadline(SdkClient::DEFAULT_TCK_REQUEST_TIMEOUT);

  if (params.mKey.has_value())
  {
    accountCreateTransaction.setKey(getHederaKey(params.mKey.value()));
  }

  if (params.mInitialBalance.has_value())
  {
    accountCreateTransaction.setInitialBalance(Hbar(params.mInitialBalance.value(), HbarUnit::TINYBAR()));
  }

  if (params.mReceiverSignatureRequired.has_value())
  {
    accountCreateTransaction.setReceiverSignatureRequired(params.mReceiverSignatureRequired.value());
  }

  if (params.mAutoRenewPeriod.has_value())
  {
    accountCreateTransaction.setAutoRenewPeriod(std::chrono::seconds(params.mAutoRenewPeriod.value()));
  }

  if (params.mMemo.has_value())
  {
    accountCreateTransaction.setAccountMemo(params.mMemo.value());
  }

  if (params.mMaxAutoTokenAssociations.has_value())
  {
    accountCreateTransaction.setMaxAutomaticTokenAssociations(params.mMaxAutoTokenAssociations.value());
  }

  if (params.mStakedAccountId.has_value())
  {
    accountCreateTransaction.setStakedAccountId(AccountId::fromString(params.mStakedAccountId.value()));
  }

  if (params.mStakedNodeId.has_value())
  {
    accountCreateTransaction.setStakedNodeId(params.mStakedNodeId.value());
  }

  if (params.mDeclineStakingReward.has_value())
  {
    accountCreateTransaction.setDeclineStakingReward(params.mDeclineStakingReward.value());
  }

  if (params.mAlias.has_value())
  {
    accountCreateTransaction.setAlias(EvmAddress::fromString(params.mAlias.value()));
  }

  if (params.mCommonTxParams.has_value())
  {
    params.mCommonTxParams->fillOutTransaction(accountCreateTransaction, getSdkClient()->getClient());
  }

  const TransactionReceipt txReceipt =
    accountCreateTransaction.execute(getSdkClient()->getClient()).getReceipt(getSdkClient()->getClient());
  return {
    {"accountId", txReceipt.mAccountId->toString()     },
    { "status",   gStatusToString.at(txReceipt.mStatus)}
  };
}

//-----
nlohmann::json AccountService::deleteAccount(const std::optional<std::string>& deleteAccountId,
                                             const std::optional<std::string>& transferAccountId,
                                             const std::optional<CommonTransactionParams>& commonTxParams)
{
  AccountDeleteTransaction accountDeleteTransaction;
  accountDeleteTransaction.setGrpcDeadline(SdkClient::DEFAULT_TCK_REQUEST_TIMEOUT);

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
    commonTxParams->fillOutTransaction(accountDeleteTransaction, getSdkClient()->getClient());
  }

  return {
    {"status",
     gStatusToString.at(
        accountDeleteTransaction.execute(getSdkClient()->getClient()).getReceipt(getSdkClient()->getClient()).mStatus)}
  };
}

//-----
nlohmann::json AccountService::updateAccount(const std::optional<std::string>& accountId,
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
  accountUpdateTransaction.setGrpcDeadline(SdkClient::DEFAULT_TCK_REQUEST_TIMEOUT);

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
    commonTxParams->fillOutTransaction(accountUpdateTransaction, getSdkClient()->getClient());
  }

  const TransactionReceipt txReceipt =
    accountUpdateTransaction.execute(getSdkClient()->getClient()).getReceipt(getSdkClient()->getClient());
  return {
    {"status", gStatusToString.at(txReceipt.mStatus)}
  };
}

} // namespace Hedera::TCK

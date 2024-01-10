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
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "Status.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"

#include <nlohmann/json.hpp>

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
nlohmann::json SdkClient::createAccount(const std::string& publicKey,
                                        const std::optional<int64_t>& initialBalance,
                                        const std::optional<bool>& receiverSignatureRequired,
                                        const std::optional<uint32_t>& maxAutomaticTokenAssociations,
                                        const std::optional<std::string>& stakedAccountId,
                                        const std::optional<uint64_t>& stakedNodeId,
                                        const std::optional<bool>& declineStakingReward,
                                        const std::optional<std::string>& accountMemo)
{
  AccountCreateTransaction accountCreateTransaction;
  accountCreateTransaction.setGrpcDeadline(std::chrono::seconds(30));
  accountCreateTransaction.setKey(PublicKey::fromStringDer(publicKey));

  if (initialBalance.has_value())
  {
    accountCreateTransaction.setInitialBalance(Hbar(initialBalance.value(), HbarUnit::TINYBAR()));
  }

  if (receiverSignatureRequired.has_value())
  {
    accountCreateTransaction.setReceiverSignatureRequired(receiverSignatureRequired.value());
  }

  if (maxAutomaticTokenAssociations.has_value())
  {
    accountCreateTransaction.setMaxAutomaticTokenAssociations(maxAutomaticTokenAssociations.value());
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

  if (accountMemo.has_value())
  {
    accountCreateTransaction.setAccountMemo(accountMemo.value());
  }

  const TransactionReceipt txReceipt = accountCreateTransaction.execute(mClient).getReceipt(mClient);
  return {
    { "accountId", txReceipt.mAccountId->toString()      },
    { "status",    gStatusToString.at(txReceipt.mStatus) }
  };
}

//-----
std::string SdkClient::generatePrivateKey()
{
  return ED25519PrivateKey::generatePrivateKey()->toStringDer();
}

//-----
std::string SdkClient::generatePublicKey(const std::string& privateKey)
{
  return ED25519PrivateKey::fromString(privateKey)->getPublicKey()->toStringDer();
}

//-----
nlohmann::json SdkClient::reset()
{
  mClient.close();
  return {
    { "status", "SUCCESS" }
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
      { nodeIp.value(), Hedera::AccountId::fromString("0.0." + nodeAccountId.value()) }
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
    { "message", "Successfully setup " + clientType + " client." },
    { "status",  "SUCCESS"                                       }
  };
}

} // namespace Hedera::TCK::SdkClient

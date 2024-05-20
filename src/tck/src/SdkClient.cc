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
#include "ECDSAsecp256k1PrivateKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PrivateKey.h"
#include "EvmAddress.h"
#include "HbarUnit.h"
#include "JsonErrorType.h"
#include "JsonRpcException.h"
#include "Key.h"
#include "KeyList.h"
#include "PrivateKey.h"
#include "Status.h"
#include "TckKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/BadKeyException.h"
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

// Generate a Key object from an undetermined key DER-encoded hex string.
std::shared_ptr<Key> getKey(const std::string& key)
{
  try
  {
    return PublicKey::fromStringDer(key);
  }
  catch (const BadKeyException&)
  {
    try
    {
      return PrivateKey::fromStringDer(key);
    }
    catch (const BadKeyException&)
    {
      proto::Key protoKey;
      protoKey.ParseFromString(internal::Utilities::byteVectorToString(internal::HexConverter::hexToBytes(key)));
      return Key::fromProtobuf(protoKey);
    }
  }
}

// Helper function used to generate a Key.
std::string generateKeyHelper(const TckKey& key)
{
  // No type indicates a random ED25519/ECDSAsecp256k1 private/public key.
  if (!key.mType.has_value())
  {
    // If no type is specified, nothing else should be specified.
    if (key.mFromKey.has_value() || key.mThreshold.has_value() || key.mKeys.has_value())
    {
      throw JsonRpcException(JsonErrorType::INVALID_REQUEST,
                             "invalid request: no other parameters should be specified if type isn't specified.");
    }

    // Generate a random key.
    switch (internal::Utilities::getRandomNumber(0, 3))
    {
      case 0:
        return ED25519PrivateKey::generatePrivateKey()->toStringDer();
      case 1:
        return ED25519PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer();
      case 2:
        return ECDSAsecp256k1PrivateKey::generatePrivateKey()->toStringDer();
      case 3:
        return ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer();
      default:
        // This should never happen.
        throw std::range_error("Random number generated outside specified range.");
    }
  }

  if (key.mType.value() == "ed25519PrivateKey")
  {
    return ED25519PrivateKey::generatePrivateKey()->toStringDer();
  }

  if (key.mType.value() == "ecdsaSecp256k1PrivateKey")
  {
    return ECDSAsecp256k1PrivateKey::generatePrivateKey()->toStringDer();
  }

  if (key.mType.value() == "ed25519PublicKey")
  {
    return (key.mFromKey.has_value())
             ? ED25519PrivateKey::fromStringDer(key.mFromKey.value())->getPublicKey()->toStringDer()
             : ED25519PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer();
  }

  if (key.mType.value() == "ecdsaSecp256k1PublicKey")
  {
    return (key.mFromKey.has_value())
             ? ECDSAsecp256k1PrivateKey::fromStringDer(key.mFromKey.value())->getPublicKey()->toStringDer()
             : ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer();
  }

  if (const bool isThreshold = key.mType.value() == "thresholdKey"; key.mType.value() == "keyList" || isThreshold)
  {
    if (!key.mKeys.has_value())
    {
      throw JsonRpcException(JsonErrorType::INVALID_REQUEST,
                             "invalid request: keys list is required for generating a KeyList type.");
    }

    if (isThreshold && !key.mThreshold.has_value())
    {
      throw JsonRpcException(JsonErrorType::INVALID_REQUEST,
                             "invalid request: threshold is required for generating a ThresholdKey type.");
    }

    KeyList keyList;
    std::for_each(key.mKeys->cbegin(),
                  key.mKeys->cend(),
                  [&keyList](const TckKey& key) { keyList.push_back(getKey(generateKeyHelper(key))); });

    if (isThreshold)
    {
      keyList.setThreshold(key.mThreshold.value());
    }

    return internal::HexConverter::bytesToHex(
      internal::Utilities::stringToByteVector(keyList.toProtobufKey()->SerializeAsString()));
  }

  if (key.mType.value() == "privateKey")
  {
    return internal::Utilities::getRandomNumber(0, 1) > 0
             ? ED25519PrivateKey::generatePrivateKey()->toStringDer()
             : ECDSAsecp256k1PrivateKey::generatePrivateKey()->toStringDer();
  }

  if (key.mType.value() == "publicKey")
  {
    if (key.mFromKey.has_value())
    {
      try
      {
        return ED25519PrivateKey::fromStringDer(key.mFromKey.value())->getPublicKey()->toStringDer();
      }
      catch (const BadKeyException&)
      {
        return ECDSAsecp256k1PrivateKey::fromStringDer(key.mFromKey.value())->getPublicKey()->toStringDer();
      }
    }

    return (internal::Utilities::getRandomNumber(0, 1) > 0)
             ? ED25519PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer()
             : ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer();
  }

  if (key.mType.value() == "evmAddress")
  {
    return std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(
             key.mFromKey.has_value() ? ECDSAsecp256k1PrivateKey::fromStringDer(key.mFromKey.value())->getPublicKey()
                                      : ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey())
      ->toEvmAddress()
      .toString();
  }

  throw JsonRpcException(JsonErrorType::INVALID_REQUEST, "invalid request: key type not recognized.");
}

} // namespace

//-----
nlohmann::json SdkClient::createAccount(const std::optional<std::string>& key,
                                        const std::optional<int64_t>& initialBalance,
                                        const std::optional<bool>& receiverSignatureRequired,
                                        const std::optional<int64_t>& autoRenewPeriod,
                                        const std::optional<std::string>& memo,
                                        const std::optional<uint32_t>& maxAutoTokenAssociations,
                                        const std::optional<std::string>& stakedAccountId,
                                        const std::optional<uint64_t>& stakedNodeId,
                                        const std::optional<bool>& declineStakingReward,
                                        const std::optional<std::string>& alias,
                                        const std::optional<std::vector<std::string>>& signerKeys)
{
  AccountCreateTransaction accountCreateTransaction;
  accountCreateTransaction.setGrpcDeadline(std::chrono::seconds(DEFAULT_TCK_REQUEST_TIMEOUT));

  if (key.has_value())
  {
    accountCreateTransaction.setKey(getKey(key.value()));
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

  if (signerKeys.has_value())
  {
    accountCreateTransaction.freezeWith(&mClient);
    std::for_each(signerKeys->cbegin(),
                  signerKeys->cend(),
                  [&accountCreateTransaction](const std::string& signerKey)
                  { accountCreateTransaction.sign(std::dynamic_pointer_cast<PrivateKey>(getKey(signerKey))); });
  }

  const TransactionReceipt txReceipt = accountCreateTransaction.execute(mClient).getReceipt(mClient);
  return {
    {"accountId", txReceipt.mAccountId->toString()     },
    { "status",   gStatusToString.at(txReceipt.mStatus)}
  };
}

//-----
nlohmann::json SdkClient::generateKey(const std::optional<std::string>& type,
                                      const std::optional<std::string>& fromKey,
                                      const std::optional<int>& threshold,
                                      const std::optional<std::vector<TckKey>>& keys)
{
  return {
    {"key", generateKeyHelper({ type, fromKey, threshold, keys })}
  };
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

} // namespace Hedera::TCK::SdkClient

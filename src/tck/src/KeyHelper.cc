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
#include "KeyHelper.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PrivateKey.h"
#include "EvmAddress.h"
#include "JsonErrorType.h"
#include "JsonRpcException.h"
#include "Key.h"
#include "KeyList.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "exceptions/BadKeyException.h"
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

#include <algorithm>
#include <memory>
#include <optional>
#include <proto/basic_types.pb.h>
#include <stdexcept>
#include <string>
#include <vector>

namespace Hedera::TCK
{
//-----
const std::unordered_map<std::string, KeyType> gStringToKeyType = {
  {"ed25519PrivateKey",         KeyType::ED25519_PRIVATE_KEY_TYPE        },
  { "ed25519PublicKey",         KeyType::ED25519_PUBLIC_KEY_TYPE         },
  { "ecdsaSecp256k1PrivateKey", KeyType::ECDSA_SECP256k1_PRIVATE_KEY_TYPE},
  { "ecdsaSecp256k1PublicKey",  KeyType::ECDSA_SECP256k1_PUBLIC_KEY_TYPE },
  { "keyList",                  KeyType::LIST_KEY_TYPE                   },
  { "thresholdKey",             KeyType::THRESHOLD_KEY_TYPE              },
  { "evmAddress",               KeyType::EVM_ADDRESS_KEY_TYPE            }
};

//-----
const std::unordered_map<KeyType, std::string> gKeyTypeToString = {
  {KeyType::ED25519_PRIVATE_KEY_TYPE,          "ed25519PrivateKey"       },
  { KeyType::ED25519_PUBLIC_KEY_TYPE,          "ed25519PublicKey"        },
  { KeyType::ECDSA_SECP256k1_PRIVATE_KEY_TYPE, "ecdsaSecp256k1PrivateKey"},
  { KeyType::ECDSA_SECP256k1_PUBLIC_KEY_TYPE,  "ecdsaSecp256k1PublicKey" },
  { KeyType::LIST_KEY_TYPE,                    "keyList"                 },
  { KeyType::THRESHOLD_KEY_TYPE,               "thresholdKey"            },
  { KeyType::EVM_ADDRESS_KEY_TYPE,             "evmAddress"              }
};

//-----
KeyRequest::KeyRequest(const std::string& type,
                       const std::optional<std::string>& fromKey,
                       const std::optional<int>& threshold,
                       const std::optional<std::vector<KeyRequest>>& keys)
  : mType(gStringToKeyType.at(type))
  , mFromKey(fromKey)
  , mThreshold(threshold)
  , mKeys(keys)
{
}

//-----
std::shared_ptr<Hedera::Key> getHederaKey(const std::string& key)
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
      return Hedera::Key::fromProtobuf(protoKey);
    }
  }
}

//-----
std::string processKeyRequest(const KeyRequest& request, nlohmann::json& response, bool isList)
{
  // Make sure fromKey is only provided for ED25519_PUBLIC_KEY_TYPE, ECDSA_SECP256k1_PUBLIC_KEY_TYPE, or
  // EVM_ADDRESS_KEY_TYPE.
  if (request.mFromKey.has_value() && request.mType != KeyType::ED25519_PUBLIC_KEY_TYPE &&
      request.mType != KeyType::ECDSA_SECP256k1_PUBLIC_KEY_TYPE && request.mType != KeyType::EVM_ADDRESS_KEY_TYPE)
  {
    throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                           "invalid parameters: fromKey should only be provided for ed25519PublicKey, "
                           "ecdsaSecp256k1PublicKey, or evmAddress types.");
  }

  // Make sure threshold is only provided for THRESHOLD_KEY_TYPE.
  if (request.mThreshold.has_value() && request.mType != KeyType::THRESHOLD_KEY_TYPE)
  {
    throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                           "invalid parameters: threshold should only be provided for thresholdKey types.");
  }

  // Make sure keys is only provided for LIST_KEY_TYPE or THRESHOLD_KEY_TYPE.
  if (request.mKeys.has_value() && request.mType != KeyType::LIST_KEY_TYPE &&
      request.mType != KeyType::THRESHOLD_KEY_TYPE)
  {
    throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                           "invalid parameters: keys should only be provided for keyList or thresholdKey types.");
  }

  switch (request.mType)
  {
    case KeyType::ED25519_PRIVATE_KEY_TYPE:
    {
      const std::string key = ED25519PrivateKey::generatePrivateKey()->toStringDer();
      if (isList)
      {
        response["privateKeys"].push_back(key);
      }

      return key;
    }

    case KeyType::ECDSA_SECP256k1_PRIVATE_KEY_TYPE:
    {
      const std::string key = ECDSAsecp256k1PrivateKey::generatePrivateKey()->toStringDer();
      if (isList)
      {
        response["privateKeys"].push_back(key);
      }

      return key;
    }

    case KeyType::ED25519_PUBLIC_KEY_TYPE:
    {
      if (request.mFromKey.has_value())
      {
        return ED25519PrivateKey::fromString(request.mFromKey.value())->getPublicKey()->toStringDer();
      }

      const std::unique_ptr<PrivateKey> key = ED25519PrivateKey::generatePrivateKey();
      if (isList)
      {
        response["privateKeys"].push_back(key->toStringDer());
      }

      return key->getPublicKey()->toStringDer();
    }

    case KeyType::ECDSA_SECP256k1_PUBLIC_KEY_TYPE:
    {
      if (request.mFromKey.has_value())
      {
        return ECDSAsecp256k1PrivateKey::fromString(request.mFromKey.value())->getPublicKey()->toStringDer();
      }

      const std::unique_ptr<PrivateKey> key = ECDSAsecp256k1PrivateKey::generatePrivateKey();
      if (isList)
      {
        response["privateKeys"].push_back(key->toStringDer());
      }

      return key->getPublicKey()->toStringDer();
    }

    case KeyType::LIST_KEY_TYPE:
    case KeyType::THRESHOLD_KEY_TYPE:
    {
      if (!request.mKeys.has_value())
      {
        throw JsonRpcException(JsonErrorType::INVALID_REQUEST,
                               "invalid request: keys list is required for generating a KeyList type.");
      }

      const bool isThreshold = request.mType == KeyType::THRESHOLD_KEY_TYPE;
      if (isThreshold && !request.mThreshold.has_value())
      {
        throw JsonRpcException(JsonErrorType::INVALID_REQUEST,
                               "invalid request: threshold is required for generating a ThresholdKey type.");
      }

      KeyList keyList;
      std::for_each(request.mKeys->cbegin(),
                    request.mKeys->cend(),
                    [&keyList, &response](const KeyRequest& key)
                    { keyList.push_back(getHederaKey(processKeyRequest(key, response, true))); });

      if (isThreshold)
      {
        keyList.setThreshold(request.mThreshold.value());
      }

      return internal::HexConverter::bytesToHex(
        internal::Utilities::stringToByteVector(keyList.toProtobufKey()->SerializeAsString()));
    }

    case KeyType::EVM_ADDRESS_KEY_TYPE:
    {
      if (request.mFromKey.has_value())
      {
        const std::shared_ptr<Key> key = getHederaKey(request.mFromKey.value());
        if (const std::shared_ptr<ECDSAsecp256k1PrivateKey> privateKey =
              std::dynamic_pointer_cast<ECDSAsecp256k1PrivateKey>(key);
            privateKey)
        {
          return std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(privateKey->getPublicKey())
            ->toEvmAddress()
            .toString();
        }

        if (const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKey =
              std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(key);
            publicKey)
        {
          return publicKey->toEvmAddress().toString();
        }

        throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                               "invalid parameters: fromKey for evmAddress is not ECDSAsecp256k1.");
      }

      return std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(
               ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey())
        ->toEvmAddress()
        .toString();
    }

    default:
    {
      throw JsonRpcException(JsonErrorType::INVALID_REQUEST, "invalid request: key type not recognized.");
    }
  }
}

} // namespace Hedera::TCK

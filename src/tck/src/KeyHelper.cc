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
KeyRequest::KeyRequest(const std::optional<std::string>& type,
                       const std::optional<std::string>& fromKey,
                       const std::optional<int>& threshold,
                       const std::optional<std::vector<KeyRequest>>& keys)
  : mType(getKeyTypeEnum(type))
  , mFromKey(fromKey)
  , mThreshold(threshold)
  , mKeys(keys)
{
}

//-----
std::optional<KeyType> getKeyTypeEnum(const std::optional<std::string>& type)
{
  // clang-format off
  if (!type.has_value())                          { return std::nullopt; }
  if (type.value() == "ed25519PrivateKey")        { return KeyType::ED25519_PRIVATE_KEY_TYPE; }
  if (type.value() == "ed25519PublicKey")         { return KeyType::ED25519_PUBLIC_KEY_TYPE; }
  if (type.value() == "ecdsaSecp256k1PrivateKey") { return KeyType::ECDSA_SECP256k1_PRIVATE_KEY_TYPE; }
  if (type.value() == "ecdsaSecp256k1PublicKey")  { return KeyType::ECDSA_SECP256k1_PUBLIC_KEY_TYPE; }
  if (type.value() == "keyList")                  { return KeyType::LIST_KEY_TYPE; }
  if (type.value() == "thresholdKey")             { return KeyType::THRESHOLD_KEY_TYPE; }
  if (type.value() == "privateKey")               { return KeyType::PRIVATE_KEY_TYPE; }
  if (type.value() == "publicKey")                { return KeyType::PUBLIC_KEY_TYPE; }
  if (type.value() == "evmAddress")               { return KeyType::EVM_ADDRESS_KEY_TYPE; }
  // clang-format on

  throw std::invalid_argument("unrecognized key type string " + type.value());
};

//-----
std::optional<std::string> getKeyTypeString(const std::optional<KeyType>& type)
{
  // clang-format off
  if (!type.has_value())                                         { return std::nullopt; }
  if (type.value() == KeyType::ED25519_PRIVATE_KEY_TYPE)         { return "ed25519PrivateKey"; }
  if (type.value() == KeyType::ED25519_PUBLIC_KEY_TYPE)          { return "ed25519PublicKey"; }
  if (type.value() == KeyType::ECDSA_SECP256k1_PRIVATE_KEY_TYPE) { return "ecdsaSecp256k1PrivateKey"; }
  if (type.value() == KeyType::ECDSA_SECP256k1_PUBLIC_KEY_TYPE)  { return "ecdsaSecp256k1PublicKey"; }
  if (type.value() == KeyType::LIST_KEY_TYPE)                    { return "keyList"; }
  if (type.value() == KeyType::THRESHOLD_KEY_TYPE)               { return "thresholdKey"; }
  if (type.value() == KeyType::PRIVATE_KEY_TYPE)                 { return "privateKey"; }
  if (type.value() == KeyType::PUBLIC_KEY_TYPE)                  { return "publicKey"; }
  if (type.value() == KeyType::EVM_ADDRESS_KEY_TYPE)             { return "evmAddress"; }
  // clang-format on

  throw std::invalid_argument("input key type is KeyType::KEY_TYPE_SIZE and does not represent a valid KeyType");
};

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
std::string processKeyRequest(const KeyRequest& request)
{
  // No type indicates a random ED25519/ECDSAsecp256k1 private/public key.
  if (!request.mType.has_value())
  {
    // If no type is specified, nothing else should be specified.
    if (request.mFromKey.has_value() || request.mThreshold.has_value() || request.mKeys.has_value())
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

  switch (request.mType.value())
  {
    case KeyType::ED25519_PRIVATE_KEY_TYPE:
    {
      return ED25519PrivateKey::generatePrivateKey()->toStringDer();
    }

    case KeyType::ECDSA_SECP256k1_PRIVATE_KEY_TYPE:
    {
      return ECDSAsecp256k1PrivateKey::generatePrivateKey()->toStringDer();
    }

    case KeyType::ED25519_PUBLIC_KEY_TYPE:
    {
      return (request.mFromKey.has_value())
               ? ED25519PrivateKey::fromStringDer(request.mFromKey.value())->getPublicKey()->toStringDer()
               : ED25519PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer();
    }

    case KeyType::ECDSA_SECP256k1_PUBLIC_KEY_TYPE:
    {
      return (request.mFromKey.has_value())
               ? ECDSAsecp256k1PrivateKey::fromStringDer(request.mFromKey.value())->getPublicKey()->toStringDer()
               : ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer();
    }

    case KeyType::LIST_KEY_TYPE:
    case KeyType::THRESHOLD_KEY_TYPE:
    {
      if (!request.mKeys.has_value())
      {
        throw JsonRpcException(JsonErrorType::INVALID_REQUEST,
                               "invalid request: keys list is required for generating a KeyList type.");
      }

      const bool isThreshold = request.mType.value() == KeyType::THRESHOLD_KEY_TYPE;
      if (isThreshold && !request.mThreshold.has_value())
      {
        throw JsonRpcException(JsonErrorType::INVALID_REQUEST,
                               "invalid request: threshold is required for generating a ThresholdKey type.");
      }

      KeyList keyList;
      std::for_each(request.mKeys->cbegin(),
                    request.mKeys->cend(),
                    [&keyList](const KeyRequest& key) { keyList.push_back(getHederaKey(processKeyRequest(key))); });

      if (isThreshold)
      {
        keyList.setThreshold(request.mThreshold.value());
      }

      return internal::HexConverter::bytesToHex(
        internal::Utilities::stringToByteVector(keyList.toProtobufKey()->SerializeAsString()));
    }

    case KeyType::PRIVATE_KEY_TYPE:
    {
      return internal::Utilities::getRandomNumber(0, 1) > 0
               ? ED25519PrivateKey::generatePrivateKey()->toStringDer()
               : ECDSAsecp256k1PrivateKey::generatePrivateKey()->toStringDer();
    }

    case KeyType::PUBLIC_KEY_TYPE:
    {
      if (request.mFromKey.has_value())
      {
        try
        {
          return ED25519PrivateKey::fromStringDer(request.mFromKey.value())->getPublicKey()->toStringDer();
        }
        catch (const BadKeyException&)
        {
          return ECDSAsecp256k1PrivateKey::fromStringDer(request.mFromKey.value())->getPublicKey()->toStringDer();
        }
      }

      return (internal::Utilities::getRandomNumber(0, 1) > 0)
               ? ED25519PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer()
               : ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer();
    }

    case KeyType::EVM_ADDRESS_KEY_TYPE:
    {
      return std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(
               request.mFromKey.has_value()
                 ? ECDSAsecp256k1PrivateKey::fromStringDer(request.mFromKey.value())->getPublicKey()
                 : ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey())
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

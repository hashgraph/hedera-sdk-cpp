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
std::optional<Key::Type> Key::getTypeEnum(const std::optional<std::string>& type)
{
  // clang-format off
  if (!type.has_value())                          { return std::nullopt; }
  if (type.value() == "ed25519PrivateKey")        { return ED25519_PRIVATE; }
  if (type.value() == "ed25519PublicKey")         { return ED25519_PUBLIC; }
  if (type.value() == "ecdsaSecp256k1PrivateKey") { return ECDSA_SECP256k1_PRIVATE; }
  if (type.value() == "ecdsaSecp256k1PublicKey")  { return ECDSA_SECP256k1_PUBLIC; }
  if (type.value() == "keyList")                  { return LIST; }
  if (type.value() == "thresholdKey")             { return THRESHOLD; }
  if (type.value() == "privateKey")               { return PRIVATE; }
  if (type.value() == "publicKey")                { return PUBLIC; }
  if (type.value() == "evmAddress")               { return EVM_ADDRESS; }
  // clang-format on

  throw std::invalid_argument("unrecognized key type " + type.value());
};

//-----
std::optional<std::string> Key::getTypeString(const std::optional<Key::Type>& type)
{
  // clang-format off
  if (!type.has_value())                       { return std::nullopt; }
  if (type.value() == ED25519_PRIVATE)         { return "ed25519PrivateKey"; }
  if (type.value() == ED25519_PUBLIC)          { return "ed25519PublicKey"; }
  if (type.value() == ECDSA_SECP256k1_PRIVATE) { return "ecdsaSecp256k1PrivateKey"; }
  if (type.value() == ECDSA_SECP256k1_PUBLIC)  { return "ecdsaSecp256k1PublicKey"; }
  if (type.value() == LIST)                    { return "keyList"; }
  if (type.value() == THRESHOLD)               { return "thresholdKey"; }
  if (type.value() == PRIVATE)                 { return "privateKey"; }
  if (type.value() == PUBLIC)                  { return "publicKey"; }
  if (type.value() == EVM_ADDRESS)             { return "evmAddress"; }
  return std::nullopt;
  // clang-format on
};

//-----
Key::Key(const std::optional<std::string>& type,
         const std::optional<std::string>& fromKey,
         const std::optional<int>& threshold,
         const std::optional<std::vector<Key>>& keys)
  : mType(getTypeEnum(type))
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
std::string tckKeyToHex(const Hedera::TCK::Key& key)
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

  switch (key.mType.value())
  {
    case Key::Type::ED25519_PRIVATE:
    {
      return ED25519PrivateKey::generatePrivateKey()->toStringDer();
    }

    case Key::Type::ECDSA_SECP256k1_PRIVATE:
    {
      return ECDSAsecp256k1PrivateKey::generatePrivateKey()->toStringDer();
    }

    case Key::Type::ED25519_PUBLIC:
    {
      return (key.mFromKey.has_value())
               ? ED25519PrivateKey::fromStringDer(key.mFromKey.value())->getPublicKey()->toStringDer()
               : ED25519PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer();
    }

    case Key::Type::ECDSA_SECP256k1_PUBLIC:
    {
      return (key.mFromKey.has_value())
               ? ECDSAsecp256k1PrivateKey::fromStringDer(key.mFromKey.value())->getPublicKey()->toStringDer()
               : ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey()->toStringDer();
    }

    case Key::Type::LIST:
    case Key::Type::THRESHOLD:
    {
      if (!key.mKeys.has_value())
      {
        throw JsonRpcException(JsonErrorType::INVALID_REQUEST,
                               "invalid request: keys list is required for generating a KeyList type.");
      }

      const bool isThreshold = key.mType.value() == Key::Type::THRESHOLD;
      if (isThreshold && !key.mThreshold.has_value())
      {
        throw JsonRpcException(JsonErrorType::INVALID_REQUEST,
                               "invalid request: threshold is required for generating a ThresholdKey type.");
      }

      KeyList keyList;
      std::for_each(key.mKeys->cbegin(),
                    key.mKeys->cend(),
                    [&keyList](const Hedera::TCK::Key& key) { keyList.push_back(getHederaKey(tckKeyToHex(key))); });

      if (isThreshold)
      {
        keyList.setThreshold(key.mThreshold.value());
      }

      return internal::HexConverter::bytesToHex(
        internal::Utilities::stringToByteVector(keyList.toProtobufKey()->SerializeAsString()));
    }

    case Key::Type::PRIVATE:
    {
      return internal::Utilities::getRandomNumber(0, 1) > 0
               ? ED25519PrivateKey::generatePrivateKey()->toStringDer()
               : ECDSAsecp256k1PrivateKey::generatePrivateKey()->toStringDer();
    }

    case Key::Type::PUBLIC:
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

    case Key::Type::EVM_ADDRESS:
    {
      return std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(
               key.mFromKey.has_value() ? ECDSAsecp256k1PrivateKey::fromStringDer(key.mFromKey.value())->getPublicKey()
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

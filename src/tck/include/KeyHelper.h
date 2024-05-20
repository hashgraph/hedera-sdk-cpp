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
#ifndef HEDERA_TCK_CPP_KEY_HELPER_H_
#define HEDERA_TCK_CPP_KEY_HELPER_H_

#include "JsonErrorType.h"
#include "JsonRpcException.h"
#include "Key.h"

#include <algorithm>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace Hedera::TCK
{
/**
 * Helper struct used to assist in generating Keys for the TCK. Since generating KeyLists and ThresholdKeys can be
 * recursive, this helps with converting the nlohmann::json objects and checking types.
 */
struct Key
{
  /**
   * Enumeration of the possible types of Keys.
   */
  enum Type : uint8_t
  {
    ED25519_PRIVATE,
    ED25519_PUBLIC,
    ECDSA_SECP256k1_PRIVATE,
    ECDSA_SECP256k1_PUBLIC,
    LIST,
    THRESHOLD,
    PRIVATE,
    PUBLIC,
    EVM_ADDRESS
  };

  /**
   * Get the corresponding Type of an input type string. This function acts as a map from a Type string representation
   * to the actual Type the string represents, except it supports empty optional arguments.
   *
   * @param type The type as a string.
   * @return The corresponding Type. Uninitialized if the input type string was uninitialized.
   * @throws std::invalid_argument If the input string does not map to a Type enum.
   */
  static std::optional<Type> getTypeEnum(const std::optional<std::string>& type);

  /**
   * Get the corresponding type string of an input Type. This function acts as a map from a Type to that Type's
   * string representation, except it supports empty optional arguments.
   *
   * @param type The Type from which to get the string representation.
   * @return The corresponding string representation of the input Type. Uninitialized if the Type string was
   *         uninitialized.
   */
  static std::optional<std::string> getTypeString(const std::optional<Type>& type);

  Key() = default;

  /**
   * Construct with values for all fields.
   *
   * @param type      The type of Key to generate.
   * @param fromKey   The associated private key for a public key, or ECDSAsecp256k1 private or public key for an EVM
   *                  address.
   * @param threshold The threshold of keys to sign for ThresholdKeys.
   * @param keys      The key information for KeyLists and ThresholdKeys.
   */
  Key(const std::optional<std::string>& type,
      const std::optional<std::string>& fromKey,
      const std::optional<int>& threshold,
      const std::optional<std::vector<Key>>& keys);

  /**
   * The type of Key to generate. If not provided, the returned key will be of type ED25519Private, ED25519Public,
   * ECDSAsecp256k1Private, or ECDSAsecp256k1Public. Private and Public types should be used when any private or public
   * key type is required (respectively) but the specific type (ED25519 or ECDSAsecp256k1) doesn't matter.
   */
  std::optional<Type> mType;

  /**
   * For ED25519Public and ECDSAsecp256k1Public types, the DER-encoded hex string private key from which to generate the
   * public key. No value means a random ED25519Public or ECDSAsecp256k1Public will be generated, respectively. For
   * EvmAddress, the DER-encoded hex string of an ECDSAsecp256k1Private or ECDSAsecp256k1Public from which to generate
   * the EVM address. An ECDSAsecp256k1Private will first generate its respective ECDSAsecp256k1Public, and then
   * generate the EVM address from that public key. No value means a random EVM address will be generated.
   */
  std::optional<std::string> mFromKey;

  /**
   * Required for Threshold types. The number of keys that must sign for a threshold key.
   */
  std::optional<int> mThreshold;

  /**
   * Required for List and Threshold types. Specify the types of keys to be generated and put in the List or Threshold.
   */
  std::optional<std::vector<TCK::Key>> mKeys;
};

/**
 * Generate a Hedera Key from a key hex string. The string must be either the DER-encoding of an ED25519 or
 * ECDSAsecp256k1 private or public key, or the serialized Key protobuf of a KeyList or ThresholdKey.
 *
 * @param key The hex string from which to get the Hedera Key.
 * @return The Hedera Key of the input hex string.
 */
std::shared_ptr<Hedera::Key> getHederaKey(const std::string& key);

/**
 * Helper function used to generate the hex string of an input TCK Key. For ED25519 or ECDSAsecp256k1 private or
 * public key types, this will be the DER-encoding of the key. For KeyList of ThresholdKey types, this will be the
 * serialized Key protobuf of the key.
 *
 * @param key The TCK Key to encode.
 * @return The hex encoding of the TCK Key.
 */
std::string tckKeyToHex(const Hedera::TCK::Key& key);

} // namespace Hedera::TCK

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert TckKey arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<Hedera::TCK::Key>
{
  /**
   * Convert a TckKey to a JSON object.
   *
   * @param jsonTo The JSON object to fill with the TckKey.
   * @param opt    The TckKey with which to fill the JSON object.
   */
  static void to_json(json& jsonTo, const Hedera::TCK::Key& key)
  {
    if (key.mType.has_value())
    {
      jsonTo["type"] = Hedera::TCK::Key::getTypeString(key.mType).value(); // NOLINT
    }

    if (key.mFromKey.has_value())
    {
      jsonTo["fromKey"] = key.mFromKey.value();
    }

    if (key.mThreshold.has_value())
    {
      jsonTo["threshold"] = key.mThreshold.value();
    }

    if (key.mKeys.has_value())
    {
      std::for_each(key.mKeys->cbegin(),
                    key.mKeys->cend(),
                    [&jsonTo](const Hedera::TCK::Key& key) // NOLINT
                    {
                      jsonTo["keys"].push_back(json());
                      to_json(jsonTo["keys"].back(), key);
                    });
    }
  }

  /**
   * Convert a JSON object to a TckKey.
   *
   * @param jsonFrom The JSON object with which to fill the Key.
   * @param opt      The TckKey to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, Hedera::TCK::Key& key)
  {
    if (jsonFrom.contains("type"))
    {
      if (!jsonFrom["type"].is_string())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_REQUEST,
                                            "invalid request: type should be a string");
      }

      try
      {
        key.mType = Hedera::TCK::Key::getTypeEnum(jsonFrom["type"].get<std::string>());
      }
      catch (const std::invalid_argument& ex)
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: " + std::string(ex.what()));
      }
    }

    if (jsonFrom.contains("fromKey"))
    {
      if (!jsonFrom["fromKey"].is_string())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_REQUEST,
                                            "invalid request: fromKey should be a string");
      }

      key.mFromKey = jsonFrom["fromKey"];
    }

    if (jsonFrom.contains("threshold"))
    {
      if (!jsonFrom["threshold"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_REQUEST,
                                            "invalid request: threshold should be an integer");
      }

      key.mThreshold = jsonFrom["threshold"];
    }

    if (jsonFrom.contains("keys"))
    {
      if (!jsonFrom["keys"].is_array())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_REQUEST,
                                            "invalid request: keys should be a list");
      }

      key.mKeys = std::vector<Hedera::TCK::Key>();
      key.mKeys->reserve(jsonFrom["keys"].size());
      std::for_each(jsonFrom["keys"].cbegin(),
                    jsonFrom["keys"].cend(),
                    [&key](const json& jsonKey)
                    {
                      key.mKeys->push_back(Hedera::TCK::Key());
                      from_json(jsonKey, key.mKeys->back());
                    });
    }
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_KEY_HELPER_H_
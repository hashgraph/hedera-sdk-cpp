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
#ifndef HEDERA_TCK_CPP_TCK_KEY_H_
#define HEDERA_TCK_CPP_TCK_KEY_H_

#include "JsonErrorType.h"
#include "JsonRpcException.h"

#include <algorithm>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

namespace Hedera::TCK
{
/**
 * Helper struct used to assist in generating Keys for the TCK. Since generating KeyLists and ThresholdKeys can be
 * recursive, this helps with converting the nlohmann::json objects and checking types.
 */
struct TckKey
{
  TckKey() = default;

  /**
   * Construct with values for all fields.
   *
   * @param type      The type of Key to generate.
   * @param fromKey   The associated private key for a public key, or ECDSAsecp256k1 private or public key for an EVM
   *                  address.
   * @param threshold The threshold of keys to sign for ThresholdKeys.
   * @param keys      The key information for KeyLists and ThresholdKeys.
   */
  TckKey(const std::optional<std::string>& type,
         const std::optional<std::string>& fromKey,
         const std::optional<int>& threshold,
         const std::optional<std::vector<TckKey>>& keys)
    : mType(type)
    , mFromKey(fromKey)
    , mThreshold(threshold)
    , mKeys(keys)
  {
  }

  /**
   * The type of Key to generate. If provided, it MUST be one of `ed25519PrivateKey`, `ed25519PublicKey`,
   * `ecdsaSecp256k1PrivateKey`, `ecdsaSecp256k1PublicKey`, `keyList`, `thresholdKey`, `privateKey`, `publicKey`, or
   * `evmAddress`. If not provided, the returned key will be of type `ed25519PrivateKey`, `ed25519PublicKey`,
   * `ecdsaSecp256k1PrivateKey`, or `ecdsaSecp256k1PublicKey`. `privateKey` and `publicKey` types should be used when
   * any private or public key type is required (respectively) but the specific type (ED25519 or ECDSAsecp256k1) doesn't
   * matter.
   */
  std::optional<std::string> mType;

  /**
   * For `ed25519PublicKey` and `ecdsaSecp256k1PublicKey` types, the DER-encoded hex string private key from which to
   * generate the public key. No value means a random `ed25519PublicKey` or `ecdsaSecp256k1PublicKey` will be generated,
   * respectively. For the `evmAddress` type, the DER-encoded hex string of an `ecdsaSecp256k1PrivateKey` or
   * `ecdsaSecp256k1PublicKey` from which to generate the EVM address. An `ecdsaSecp256k1PrivateKey` will first generate
   * its respective `ecdsaSecp256k1PublicKey`, and then generate the EVM address from that public key. No value means a
   * random EVM address will be generated.
   */
  std::optional<std::string> mFromKey;

  /**
   * Required for `thresholdKey` types. The number of keys that must sign for a threshold key.
   */
  std::optional<int> mThreshold;

  /**
   * Required for `keyList` and `thresholdKey` types. Specify the types of keys to be generated and put in the `keyList`
   * or `thresholdKey`. All keys should contain the same parameters as this `generateKey` method (see examples below),
   * if required.
   */
  std::optional<std::vector<TckKey>> mKeys;
};

} // namespace Hedera::TCK

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert TckKey arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<Hedera::TCK::TckKey>
{
  /**
   * Convert a TckKey to a JSON object.
   *
   * @param jsonTo The JSON object to fill with the TckKey.
   * @param opt    The TckKey with which to fill the JSON object.
   */
  static void to_json(json& jsonTo, const Hedera::TCK::TckKey& key)
  {
    if (key.mType.has_value())
    {
      jsonTo["type"] = key.mType.value();
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
                    [&jsonTo](const Hedera::TCK::TckKey& key)
                    {
                      jsonTo["keys"].push_back(json());
                      to_json(jsonTo["keys"].back(), key);
                    });
    }
  }

  /**
   * Convert a JSON object to a TckKey.
   *
   * @param jsonFrom The JSON object with which to fill the TckKey.
   * @param opt      The TckKey to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, Hedera::TCK::TckKey& key)
  {
    if (jsonFrom.contains("type"))
    {
      if (!jsonFrom["type"].is_string())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_REQUEST,
                                            "invalid request: type should be a string");
      }

      key.mType = jsonFrom["type"];
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

      key.mKeys = std::vector<Hedera::TCK::TckKey>(jsonFrom["keys"].size());
      std::for_each(jsonFrom["keys"].cbegin(),
                    jsonFrom["keys"].cend(),
                    [&key](const json& jsonKey)
                    {
                      key.mKeys->push_back(Hedera::TCK::TckKey());
                      from_json(jsonKey, key.mKeys->back());
                    });
    }
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_TCK_KEY_H_

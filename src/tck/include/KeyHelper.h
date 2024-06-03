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
#include <unordered_map>
#include <vector>

namespace Hedera::TCK
{
/**
 * Enumeration of the possible types of Keys the SDK server can generate.
 */
enum class KeyType : uint8_t
{
  ED25519_PRIVATE_KEY_TYPE,
  ED25519_PUBLIC_KEY_TYPE,
  ECDSA_SECP256k1_PRIVATE_KEY_TYPE,
  ECDSA_SECP256k1_PUBLIC_KEY_TYPE,
  LIST_KEY_TYPE,
  THRESHOLD_KEY_TYPE,
  EVM_ADDRESS_KEY_TYPE,
  KEY_TYPE_SIZE [[maybe_unused]]
};

/**
 * Map of KeyTypes to their string representation.
 */
extern const std::unordered_map<std::string, KeyType> gStringToKeyType;

/**
 * Map of KeyType string representations to KeyType.
 */
extern const std::unordered_map<KeyType, std::string> gKeyTypeToString;

/**
 * Helper struct used to contain information about a key the TCK would like the SDK server to generate. Since
 * generating KeyLists and ThresholdKeys can be recursive (KeyLists that contain a list of KeyLists, etc.), a struct
 * that can contain itself is needed to help with converting the nlohmann::json objects and checking types.
 */
struct KeyRequest
{
  KeyRequest() = default;

  /**
   * Construct with values for all fields.
   *
   * @param type      The type of Key to generate.
   * @param fromKey   The associated private key for a public key, or ECDSAsecp256k1 private or public key for an EVM
   *                  address.
   * @param threshold The threshold of keys to sign for ThresholdKeys.
   * @param keys      The KeyRequest information for KeyLists and ThresholdKeys.
   */
  KeyRequest(const std::string& type,
             const std::optional<std::string>& fromKey,
             const std::optional<int>& threshold,
             const std::optional<std::vector<KeyRequest>>& keys);

  /**
   * The type of Key to generate.
   */
  KeyType mType;

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
   * Required for List and Threshold types. Specify the keys to be generated and put in the List or Threshold.
   */
  std::optional<std::vector<KeyRequest>> mKeys;
};

/**
 * Generate a Key from a key hex string. The string must be either the DER-encoding of an ED25519 or ECDSAsecp256k1
 * private or public key, or the serialized Key protobuf of a KeyList or ThresholdKey.
 *
 * @param key The hex string from which to get the Key.
 * @return The Key of the input hex string.
 */
std::shared_ptr<Key> getHederaKey(const std::string& key);

/**
 * Process a KeyRequest and return the generated key. For ED25519 or ECDSAsecp256k1 private or public key types, this
 * will be the DER-encoding of the key. For KeyList of ThresholdKey types, this will be the serialized Key protobuf of
 * the key, as well as the private keys contained in the list.
 *
 * @param request  The KeyRequest to process.
 * @param response The JSON object to possibly fill with a list of private keys for KeyList and ThresholdKey types.
 * @param isList   If the current KeyRequest is a KeyList or ThresholdKey type and should add private keys to the
 *                 response.
 * @return The hex encoding of the generated key.
 */
std::string processKeyRequest(const KeyRequest& request, nlohmann::json& response, bool isList = false);

} // namespace Hedera::TCK

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert KeyRequest arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<Hedera::TCK::KeyRequest>
{
  /**
   * Convert a KeyRequest to a JSON object.
   *
   * @param jsonTo  The JSON object to fill with the KeyRequest.
   * @param request The KeyRequest with which to fill the JSON object.
   */
  static void to_json(json& jsonTo, const Hedera::TCK::KeyRequest& request)
  {
    jsonTo["type"] = Hedera::TCK::gKeyTypeToString.at(request.mType);

    if (request.mFromKey.has_value())
    {
      jsonTo["fromKey"] = request.mFromKey.value();
    }

    if (request.mThreshold.has_value())
    {
      jsonTo["threshold"] = request.mThreshold.value();
    }

    if (request.mKeys.has_value())
    {
      std::for_each(request.mKeys->cbegin(),
                    request.mKeys->cend(),
                    [&jsonTo](const Hedera::TCK::KeyRequest& request)
                    {
                      jsonTo["keys"].push_back(json());
                      to_json(jsonTo["keys"].back(), request);
                    });
    }
  }

  /**
   * Convert a JSON object to a KeyRequest.
   *
   * @param jsonFrom The JSON object with which to fill the KeyRequest.
   * @param request  The KeyRequest to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, Hedera::TCK::KeyRequest& request)
  {
    if (!jsonFrom.contains("type"))
    {
      throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                          "invalid parameters: type is required");
    }

    if (!jsonFrom["type"].is_string())
    {
      throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                          "invalid parameters: type should be a string");
    }

    try
    {
      request.mType = Hedera::TCK::gStringToKeyType.at(jsonFrom["type"].get<std::string>());
    }
    catch (const std::invalid_argument& ex)
    {
      throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                          "invalid parameters: " + std::string(ex.what()));
    }

    if (jsonFrom.contains("fromKey"))
    {
      if (!jsonFrom["fromKey"].is_string())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: fromKey should be a string");
      }

      request.mFromKey = jsonFrom["fromKey"];
    }

    if (jsonFrom.contains("threshold"))
    {
      if (!jsonFrom["threshold"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: threshold should be an integer");
      }

      request.mThreshold = jsonFrom["threshold"];
    }

    if (jsonFrom.contains("keys"))
    {
      if (!jsonFrom["keys"].is_array())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: keys should be a list");
      }

      request.mKeys = std::vector<Hedera::TCK::KeyRequest>();
      request.mKeys->reserve(jsonFrom["keys"].size());
      std::for_each(jsonFrom["keys"].cbegin(),
                    jsonFrom["keys"].cend(),
                    [&request](const json& jsonKey)
                    {
                      request.mKeys->push_back(Hedera::TCK::KeyRequest());
                      from_json(jsonKey, request.mKeys->back());
                    });
    }
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_KEY_HELPER_H_

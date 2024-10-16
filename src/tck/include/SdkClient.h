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
#ifndef HEDERA_TCK_CPP_SDK_CLIENT_H_
#define HEDERA_TCK_CPP_SDK_CLIENT_H_

#include "KeyHelper.h"

#include <Client.h>

#include <chrono>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <vector>

namespace Hedera::TCK
{
/**
 * Class used to handle SDK client interactions.
 */
class SdkClient
{
public:
  /**
   * The default amount of time the SdkClient should wait for a gRPC request.
   */
  constexpr static std::chrono::seconds DEFAULT_TCK_REQUEST_TIMEOUT = std::chrono::seconds(30);

  /**
   * Generate a Key.
   *
   * @param type      The type of Key to generate. It MUST be one of `ed25519PrivateKey`, `ed25519PublicKey`,
   *                  `ecdsaSecp256k1PrivateKey`, `ecdsaSecp256k1PublicKey`, `keyList`, `thresholdKey`, or `evmAddress`.
   * @param fromKey   For ED25519Public and ECDSAsecp256k1Public types, the DER-encoded hex string private key from
   *                  which to generate the public key. No value means a random ED25519Public or ECDSAsecp256k1Public
   *                  will be generated, respectively. For EvmAddress, the DER-encoded hex string of an
   *                  ECDSAsecp256k1Private or ECDSAsecp256k1Public from which to generate the EVM address. An
   *                  ECDSAsecp256k1Private will first generate its respective ECDSAsecp256k1Public, and then generate
   *                  the EVM address from that public key. No value means a random EVM address will be generated.
   * @param threshold Required for Threshold types. The number of keys that must sign for a threshold key.
   * @param keys      Required for List and Threshold types. Specify the types of keys to be generated and put in the
   *                  List or Threshold.
   * @return The JSON object which contains the generated Key.
   */
  nlohmann::json generateKey(const std::string& type,
                             const std::optional<std::string>& fromKey,
                             const std::optional<int>& threshold,
                             const std::optional<std::vector<KeyRequest>>& keys);

  /**
   * Reset the SDK client.
   *
   * @return The JSON response.
   */
  nlohmann::json reset();

  /**
   * Setup the SDK client.
   *
   * @param operatorAccountId  The ID of the operator account to use.
   * @param operatorPrivateKey The private key of the operator to use.
   * @param nodeIp             The IP of the node with which the Client should communicate.
   * @param nodeAccountId      The ID of the node account running the node with which this Client should communicate.
   * @param mirrorNetworkIp    The IP of the mirror node with which the Client should communicate.
   * @return The JSON response.
   */
  nlohmann::json setup(const std::string& operatorAccountId,
                       const std::string& operatorPrivateKey,
                       const std::optional<std::string>& nodeIp,
                       const std::optional<std::string>& nodeAccountId,
                       const std::optional<std::string>& mirrorNetworkIp);

  /**
   * Get the Hedera C++ SDK client this SdkClient is using to execute requests.
   *
   * @return Reference to the Hedera C++ SDK client this SdkClient is using to execute requests.
   */
  [[nodiscard]] const Client& getClient() const { return mClient; }

private:
  /**
   * The Hedera C++ SDK client to use to execute requests.
   */
  Client mClient;
};

} // namespace Hedera::TCK

#endif // HEDERA_TCK_CPP_SDK_CLIENT_H_

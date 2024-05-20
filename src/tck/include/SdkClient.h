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

#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <vector>

namespace Hedera::TCK::SdkClient
{
/**
 * Create an account.
 *
 * @param key                       The desired key for the account.
 * @param initialBalance            The desired initial balance for the account.
 * @param receiverSignatureRequired Should the new account require a receiver signature?
 * @param autoRenewPeriod           The desired amount of time in seconds to renew the new account.
 * @param memo                      The desired memo for the new account.
 * @param maxAutoTokenAssociations  The desired maximum number of automatic token associations for the account.
 * @param stakedAccountId           The ID of the desired account to which the new account should stake.
 * @param stakedNodeId              The ID of the desired node to which the new account should stake.
 * @param declineStakingReward      Should the new account decline staking rewards?
 * @param alias                     The desired alias for the new account.
 * @param signerKeys                Any keys that should sign the transaction (excluding fee-payer).
 * @return A JSON response containing the created account ID and the status of the account creation.
 */
nlohmann::json createAccount(const std::optional<std::string>& key,
                             const std::optional<int64_t>& initialBalance,
                             const std::optional<bool>& receiverSignatureRequired,
                             const std::optional<int64_t>& autoRenewPeriod,
                             const std::optional<std::string>& memo,
                             const std::optional<uint32_t>& maxAutoTokenAssociations,
                             const std::optional<std::string>& stakedAccountId,
                             const std::optional<uint64_t>& stakedNodeId,
                             const std::optional<bool>& declineStakingReward,
                             const std::optional<std::string>& alias,
                             const std::optional<std::vector<std::string>>& signerKeys);

/**
 * Generate a Key.
 *
 * @param type      The type of Key to generate. If not provided, the returned key will be of type ED25519Private,
 *                  ED25519Public, ECDSAsecp256k1Private, or ECDSAsecp256k1Public. Private and Public types should be
 *                  used when any private or public key type is required (respectively) but the specific type (ED25519
 *                  or ECDSAsecp256k1) doesn't matter.
 * @param fromKey   For ED25519Public and ECDSAsecp256k1Public types, the DER-encoded hex string private key from which
 *                  to generate the public key. No value means a random ED25519Public or ECDSAsecp256k1Public will be
 *                  generated, respectively. For EvmAddress, the DER-encoded hex string of an ECDSAsecp256k1Private or
 *                  ECDSAsecp256k1Public from which to generate the EVM address. An ECDSAsecp256k1Private will first
 *                  generate its respective ECDSAsecp256k1Public, and then generate the EVM address from that public
 *                  key. No value means a random EVM address will be generated.
 * @param threshold Required for Threshold types. The number of keys that must sign for a threshold key.
 * @param keys      Required for List and Threshold types. Specify the types of keys to be generated and put in the List
 *                  or Threshold.
 * @return The JSON object which contains the generated Key.
 */
nlohmann::json generateKey(const std::optional<std::string>& type,
                           const std::optional<std::string>& fromKey,
                           const std::optional<int>& threshold,
                           const std::optional<std::vector<Hedera::TCK::Key>>& keys);

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

} // namespace Hedera::TCK::SdkClient

#endif // HEDERA_TCK_CPP_SDK_CLIENT_H_

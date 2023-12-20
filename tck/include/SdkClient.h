/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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

#include "Client.h"

#include <nlohmann/json_fwd.hpp>
#include <string>
#include <string_view>

namespace Hedera::TCK::SdkClient
{
/**
 * Create an account.
 *
 * @param publicKey                     The desired public key for the account.
 * @param initialBalance                The desired initial balance for the account.
 * @param receiverSignatureRequired     Should the new account require a receiver signature?
 * @param maxAutomaticTokenAssociations The desired maximum number of automatic token associations for the account.
 * @param stakedAccountId               The ID of the desired account to which the new account should stake.
 * @param stakedNodeId                  The ID of the desired node to which the new account should stake.
 * @param declineStakingReward          Should the new account decline staking rewards?
 * @param accountMemo                   The desired memo for the new account.
 * @return A JSON response containing the created account ID and the status of the account creation.
 */
nlohmann::json createAccount(const std::string& publicKey,
                             const std::optional<int64_t>& initialBalance,
                             const std::optional<bool>& receiverSignatureRequired,
                             const std::optional<uint32_t>& maxAutomaticTokenAssociations,
                             const std::optional<std::string>& stakedAccountId,
                             const std::optional<uint64_t>& stakedNodeId,
                             const std::optional<bool>& declineStakingReward,
                             const std::optional<std::string>& accountMemo);

/**
 * Generate an ED25519PrivateKey.
 *
 * @return The generated ED25519PrivateKey, as a DER-encoded hex string.
 */
std::string generatePrivateKey();

/**
 * Generate an ED25519PublicKey.
 *
 * @param privateKey The private key from which to derive the public key, as a DER-encoded hex string.
 * @return The generated ED25519PrivateKey, as a DER-encoded hex string.
 */
std::string generatePublicKey(const std::string& privateKey);

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

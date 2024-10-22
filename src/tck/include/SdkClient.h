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

#include "CommonTransactionParams.h"
#include "CustomFee.h"
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
 * @param commonTxParams            Any parameters common to all transaction types.
 * @return A JSON response containing the created account ID and the status of the account creation.
 */
nlohmann::json createAccount(const std::optional<std::string>& key,
                             const std::optional<int64_t>& initialBalance,
                             const std::optional<bool>& receiverSignatureRequired,
                             const std::optional<int64_t>& autoRenewPeriod,
                             const std::optional<std::string>& memo,
                             const std::optional<int32_t>& maxAutoTokenAssociations,
                             const std::optional<std::string>& stakedAccountId,
                             const std::optional<int64_t>& stakedNodeId,
                             const std::optional<bool>& declineStakingReward,
                             const std::optional<std::string>& alias,
                             const std::optional<CommonTransactionParams>& commonTxParams);

/**
 * Create a token.
 *
 * @param name               The name of the new token.
 * @param symbol             The symbol of the new token.
 * @param decimals           The number of decimal places by which the new token will be divisible.
 * @param initialSupply      The number of tokens to put into circulation upon creation.
 * @param treasureAccountId  The ID of the account which will act as the new token's treasury and will receive the
 *                           specified initial supply of the new token.
 * @param adminKey           The key which can perform update/delete operations on the new token.
 * @param kycKey             The key which can grant/revoke KYC on an account for transactions of the new token.
 * @param freezeKey          The key which can freeze/unfreeze an account for transactions of the new token.
 * @param wipeKey            The key which can wipe the balance of the new token from an account.
 * @param supplyKey          The key which can change the supply of the new token.
 * @param freezeDefault      Should accounts initially be frozen with respect to the new token?
 * @param expirationTime     The time at which the new token should expire.
 * @param autoRenewAccountId The ID of the account that should be charged to renew the new token's expiration.
 * @param autoRenewPeriod    The interval at which the auto renew account will be charged to extend the new token's
 *                           expiration.
 * @param memo               The memo associated with the token.
 * @param tokenType          The type of the new token.
 * @param supplyType         The supply type of the new token.
 * @param maxSupply          The maximum amount of the new token that can be in circulation (for fungible types) or
 *                           minted (for NFTs).
 * @param feeScheduleKey     The key which can change the new token's fee schedule.
 * @param customFees         The fees to be assessed during a transfer of the new token.
 * @param pauseKey           The key which can pause/unpause the new token.
 * @param metadata           The metadata of the new token.
 * @param metadataKey        The key which can change the metadata of the new token and/or individual NFTs of the new
 *                           token class.
 * @param commonTxParams     Any parameters common to all transaction types.
 * @return A JSON response containing the created token ID and the status of the token creation.
 */
nlohmann::json createToken(const std::optional<std::string>& name,
                           const std::optional<std::string>& symbol,
                           const std::optional<uint32_t>& decimals,
                           const std::optional<std::string>& initialSupply,
                           const std::optional<std::string>& treasuryAccountId,
                           const std::optional<std::string>& adminKey,
                           const std::optional<std::string>& kycKey,
                           const std::optional<std::string>& freezeKey,
                           const std::optional<std::string>& wipeKey,
                           const std::optional<std::string>& supplyKey,
                           const std::optional<bool>& freezeDefault,
                           const std::optional<std::string>& expirationTime,
                           const std::optional<std::string>& autoRenewAccountId,
                           const std::optional<std::string>& autoRenewPeriod,
                           const std::optional<std::string>& memo,
                           const std::optional<std::string>& tokenType,
                           const std::optional<std::string>& supplyType,
                           const std::optional<std::string>& maxSupply,
                           const std::optional<std::string>& feeScheduleKey,
                           const std::optional<std::vector<std::shared_ptr<CustomFee>>>& customFees,
                           const std::optional<std::string>& pauseKey,
                           const std::optional<std::string>& metadata,
                           const std::optional<std::string>& metadataKey,
                           const std::optional<CommonTransactionParams>& commonTxParams);

/**
 * Delete an account.
 *
 * @param deleteAccountId   The ID of the account to delete.
 * @param transferAccountId The ID of the account to which to transfer remaining balances.
 * @param commonTxParams    Any parameters common to all transaction types.
 * @return A JSON response containing the status of the account deletion.
 */
nlohmann::json deleteAccount(const std::optional<std::string>& deleteAccountId,
                             const std::optional<std::string>& transferAccountId,
                             const std::optional<CommonTransactionParams>& commonTxParams);

/**
 * Generate a Key.
 *
 * @param type      The type of Key to generate. It MUST be one of `ed25519PrivateKey`, `ed25519PublicKey`,
 *                  `ecdsaSecp256k1PrivateKey`, `ecdsaSecp256k1PublicKey`, `keyList`, `thresholdKey`, or `evmAddress`.
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
 * Update an account.
 *
 * @param accountId                 The ID of the account to update.
 * @param key                       The desired key for the account.
 * @param autoRenewPeriod           The new desired amount of time in seconds to renew the account.
 * @param expirationTime            The new desired time for the account to expire.
 * @param receiverSignatureRequired Should the account now require a receiver signature?
 * @param memo                      The new desired memo for the account.
 * @param maxAutoTokenAssociations  The new desired maximum number of automatic token associations for the account.
 * @param stakedAccountId           The ID of the new desired account to which the account should stake.
 * @param stakedNodeId              The ID of the new desired node to which the account should stake.
 * @param declineStakingReward      Should the account now decline staking rewards?
 * @param commonTxParams            Any parameters common to all transaction types.
 * @return A JSON response containing the created account ID and the status of the account creation.
 */
nlohmann::json updateAccount(const std::optional<std::string>& accountId,
                             const std::optional<std::string>& key,
                             const std::optional<int64_t>& autoRenewPeriod,
                             const std::optional<int64_t>& expirationTime,
                             const std::optional<bool>& receiverSignatureRequired,
                             const std::optional<std::string>& memo,
                             const std::optional<int32_t>& maxAutoTokenAssociations,
                             const std::optional<std::string>& stakedAccountId,
                             const std::optional<int64_t>& stakedNodeId,
                             const std::optional<bool>& declineStakingReward,
                             const std::optional<CommonTransactionParams>& commonTxParams);

} // namespace Hedera::TCK::SdkClient

#endif // HEDERA_TCK_CPP_SDK_CLIENT_H_

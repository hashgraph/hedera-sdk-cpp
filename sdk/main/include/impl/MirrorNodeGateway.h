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
#ifndef HEDERA_SDK_CPP_MIRRORNODEGATEWAY_H
#define HEDERA_SDK_CPP_MIRRORNODEGATEWAY_H

#include "impl/HttpClient.h"
#include <nlohmann/json.hpp>
#include <string_view>

using json = nlohmann::json;

namespace Hedera::internal::MirrorNodeGateway
{
/**
 * @brief Query account information from the Mirror Node.
 *
 * This function queries account information from the Mirror Node based on
 * the provided account ID.
 *
 * @param mirrorNodeUrl Mirror Node URL fetched from Client
 * @param accountId The account ID for which to query information.
 * @return A JSON object containing the account information.
 * @throws IllegalStateException If an error occurs while querying the Mirror Node.
 */
json AccountInfoQuery(std::string_view mirrorNodeUrl, std::string_view accountId);

/**
 * @brief Query account balance from the Mirror Node.
 *
 * This function queries the account balance from the Mirror Node based on
 * the provided account ID.
 * @param mirrorNodeUrl Mirror Node URL fetched from Client
 * @param accountId The account ID for which to query the balance.
 * @return A JSON object containing the account balance.
 * @throws IllegalStateException If an error occurs while querying the Mirror Node.
 */
json AccountBalanceQuery(std::string_view mirrorNodeUrl, std::string_view accountId);

/**
 * @brief Query contract information from the Mirror Node.
 *
 * This function queries contract information from the Mirror Node based on
 * the provided contract ID.
 *
 * @param mirrorNodeUrl Mirror Node URL fetched from Client
 * @param contractId The contract ID for which to query information.
 * @return A JSON object containing the contract information.
 * @throws IllegalStateException If an error occurs while querying the Mirror Node.
 */
json ContractInfoQuery(std::string_view mirrorNodeUrl, std::string_view contractId);

/**
 * @brief Query token relationship information from the Mirror Node.
 *
 * This function queries token relationship information from the Mirror Node based on
 * the provided account ID.
 *
 * @param mirrorNodeUrl Mirror Node URL fetched from Client.
 * @param accountId The account ID for which to query token relationships.
 * @return A JSON object containing the token relationship information.
 * @throws IllegalStateException If an error occurs while querying the Mirror Node.
 */
json TokenAccountRelationshipQuery(std::string_view mirrorNodeUrl, std::string_view accountId);

/**
 * @brief Query token balances relationship information from the Mirror Node.
 *
 * This function queries token relationship information from the Mirror Node based on
 * the provided account ID.
 *
 * @param mirrorNodeUrl Mirror Node URL fetched from Client.
 * @param accountId The account ID for which to query token relationships.
 * @return A JSON object containing the token relationship information.
 * @throws IllegalStateException If an error occurs while querying the Mirror Node.
 */
json TokensBalancesRelationshipQuery(std::string_view mirrorNodeUrl, std::string_view accountId);

/**
 * @brief Replaces all occurrences of a substring in a string.
 *
 * This function replaces all occurrences of the substring specified by 'search'
 * with the string specified by 'replace' in the original string.
 *
 * @param original The original string to be modified.
 * @param search The substring to search for.
 * @param replace The string to replace the occurrences of 'search'.
 */
void replaceParameters(std::string& original, const std::string& search, const std::string& replace);

/**
 * @brief Builds a URL based on a mirror node URL, query type, and parameters.
 *
 * This function constructs a URL by combining a mirror node URL, a query type,
 * and a list of parameters. It replaces occurrences of "$" in the route with
 * the provided parameters.
 *
 * @param mirrorNodeUrl The mirror node URL.
 * @param queryType The type of the query.
 * @param params A vector of strings representing parameters.
 * @return The constructed URL.
 */
std::string buildUrl(std::string_view mirrorNodeUrl,
                     std::string_view queryType,
                     const std::vector<std::string>& params);
} // namespace Hedera::internal::MirrorNodeGateway
#endif // HEDERA_SDK_CPP_MIRRORNODEGATEWAY_H
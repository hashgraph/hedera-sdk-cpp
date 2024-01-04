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
 * @brief Build a URL based on the provided parameters.
 *
 * This function constructs a URL using the given components, such as an optional identifier (`id`),
 * network type, and query type.
 *
 * @param mirrorNodeUrl Mirror Node URL fetched from Client
 * @param id An optional identifier to be included in the URL (default is an empty string).
 * @param queryType The type of query for which the URL is constructed.
 * @return A string representing the constructed URL.
 */
std::string buildUrl(std::string_view mirrorNodeUrl = "", std::string_view id = "", std::string_view queryType = "");
} // namespace Hedera::internal::MirrorNodeGateway
#endif // HEDERA_SDK_CPP_MIRRORNODEGATEWAY_H
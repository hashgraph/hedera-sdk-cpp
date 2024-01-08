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
#include "impl/MirrorNodeRouter.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Hedera::internal::MirrorNodeGateway
{
/**
 * @brief Perform a mirror node query.
 *
 * This function constructs a URL based on the provided mirror node URL, query type,
 * and parameters. It then sends a REST request to the constructed URL using an
 * HttpClient, retrieves the response, and parses it into a JSON object.
 *
 * @param mirrorNodeUrl The mirror node URL.
 * @param params A vector of strings representing parameters for the query.
 * @param queryType The type of the query.
 * @return A JSON object representing the response of the mirror node query.
 *
 * @throws IllegalStateException If an error occurs during the mirror node query or JSON parsing.
 */
json MirrorNodeQuery(const std::string& mirrorNodeUrl,
                     const std::vector<std::string>& params,
                     const std::string& queryType);

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
std::string buildUrl(const std::string& mirrorNodeUrl,
                     const std::string& queryType,
                     const std::vector<std::string>& params);
} // namespace Hedera::internal::MirrorNodeGateway
#endif // HEDERA_SDK_CPP_MIRRORNODEGATEWAY_H
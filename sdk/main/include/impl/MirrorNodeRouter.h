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
 */

#ifndef HEDERA_SDK_CPP_MIRRORNODEROUTER_H
#define HEDERA_SDK_CPP_MIRRORNODEROUTER_H

#include <string>
#include <string_view>
#include <unordered_map>

/**
 * @brief Namespace for the internal classes related to the Mirror Node Gateway.
 */
namespace Hedera::internal::MirrorNodeGateway
{

/**
 * @brief Represents different mirror node query types.
 */
constexpr std::string_view ACCOUNT_INFO_QUERY = "accountInfoQuery";
constexpr std::string_view CONTRACT_INFO_QUERY = "contractInfoQuery";
constexpr std::string_view TOKEN_RELATIONSHIPS_QUERY = "tokenRelationshipsQuery";
constexpr std::string_view TOKEN_BALANCES_QUERY = "tokenBalancesQuery";

/**
 * @brief Class responsible for routing requests to different mirror node routes.
 */
class MirrorNodeRouter
{
public:
  /**
   * @brief Retrieves the route for a specific mirror node query type.
   *
   * @param queryType The type of the mirror node query (e.g., "accountInfoQuery").
   * @return A string view representing the route for the specified mirror node query.
   */
  [[nodiscard]] std::string getRoute(const std::string& queryType) const;

private:
  /**
   * @brief Internal mapping of mirror node query types to their respective routes.
   */
  std::unordered_map<std::string_view, std::string> routes = {
    {ACCOUNT_INFO_QUERY,         "/api/v1/accounts/$"       },
    { CONTRACT_INFO_QUERY,       "/api/v1/contracts/$"      },
    { TOKEN_RELATIONSHIPS_QUERY, "/api/v1/accounts/$/tokens"},
    { TOKEN_BALANCES_QUERY,      "/api/v1/tokens/$/balances"}
  };
};

} // namespace Hedera::internal::MirrorNodeGateway

#endif // HEDERA_SDK_CPP_MIRRORNODEROUTER_H

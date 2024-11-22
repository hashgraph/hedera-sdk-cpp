/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hiero
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

#ifndef HIERO_SDK_CPP_MIRRORNODEROUTER_H
#define HIERO_SDK_CPP_MIRRORNODEROUTER_H

#include <string>
#include <string_view>
#include <unordered_map>

/**
 * Namespace for the internal classes related to the Mirror Node Gateway.
 */
namespace Hiero::internal::MirrorNodeGateway
{
/**
 * Represents different mirror node query types.
 */
static const std::string& ACCOUNT_INFO_QUERY = "accountInfoQuery";
static const std::string& CONTRACT_INFO_QUERY = "contractInfoQuery";
static const std::string& TOKEN_RELATIONSHIPS_QUERY = "tokenRelationshipsQuery";
static const std::string& TOKEN_BALANCES_QUERY = "tokenBalancesQuery";

/**
 * Class responsible for routing requests to different mirror node routes.
 */
class MirrorNodeRouter
{
public:
  /**
   * Retrieves the route for a specific mirror node query type.
   *
   * @param queryType The type of the mirror node query (e.g., "accountInfoQuery").
   * @return A string view representing the route for the specified mirror node query.
   */
  [[nodiscard]] std::string getRoute(std::string_view queryType) const;

private:
  /**
   * Internal mapping of mirror node query types to their respective routes.
   */
  const std::unordered_map<std::string, std::string> routes = {
    {ACCOUNT_INFO_QUERY,         "/api/v1/accounts/$"       },
    { CONTRACT_INFO_QUERY,       "/api/v1/contracts/$"      },
    { TOKEN_RELATIONSHIPS_QUERY, "/api/v1/accounts/$/tokens"},
    { TOKEN_BALANCES_QUERY,      "/api/v1/tokens/$/balances"}
  };
};

} // namespace Hiero::internal::MirrorNodeGateway

#endif // HIERO_SDK_CPP_MIRRORNODEROUTER_H

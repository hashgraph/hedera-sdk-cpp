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

#include <iostream>

#include "impl/MirrorNodeGateway.h"
#include "impl/MirrorNodeRouter.h"

namespace Hedera::internal::MirrorNodeGateway
{
  json AccountInfoQuery(std::string_view accountId)
  {
    const json routes = MirrorNodeRouter::getRoutes();
    const std::string url = routes["testnet"]["accountInfoQuery"];
    HttpClient httpClient;
    std::string response = httpClient.invokeREST(url,"GET");
    return json::parse(response);
  }

  json AccountBalanceQuery(std::string_view accountId)
  {
    return AccountInfoQuery(accountId)["balance"];
  }

  json ContractInfoQuery(std::string_view contractId)
  {
    const json routes = MirrorNodeRouter::getRoutes();
    const std::string url = routes["testnet"]["contractInfoQuery"];
    HttpClient httpClient;
    std::string response = httpClient.invokeREST(url,"GET");
    return json::parse(response);
  }
}
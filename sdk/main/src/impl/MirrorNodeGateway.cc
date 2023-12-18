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
#include "exceptions/CURLException.h"
#include "exceptions/IllegalStateException.h"

#include <iostream>

#include "impl/MirrorNodeGateway.h"
#include "impl/MirrorNodeRouter.h"

namespace Hedera::internal::MirrorNodeGateway
{
json AccountInfoQuery(std::string_view accountId, std::string_view networkType)
{
  std::string response;
  try
  {
    const std::string mirrorNodeUrl =
      MirrorNodeRouter::getRoutes()[networkType]["accountInfoQuery"].get<std::string>() + accountId.data();
    HttpClient httpClient;
    response = httpClient.invokeREST(mirrorNodeUrl);
  }
  catch (const std::exception& e)
  {
    throw IllegalStateException(std::string(e.what() + std::string(" Illegal json state!")));
  }
  return json::parse(response);
}

json AccountBalanceQuery(std::string_view accountId, std::string_view networkType)
{
  // the account balance is stored in a balance object
  return AccountInfoQuery(accountId, networkType)["balance"]["balance"];
}

json ContractInfoQuery(std::string_view contractId, std::string_view networkType)
{
  std::string response;
  try
  {
    const std::string mirrorNodeUrl =
      MirrorNodeRouter::getRoutes()[networkType]["contractInfoQuery"].get<std::string>() + contractId.data();
    HttpClient httpClient;
    response = httpClient.invokeREST(mirrorNodeUrl);
  }
  catch (const std::exception& e)
  {
    throw IllegalStateException(std::string(e.what() + std::string(" Illegal json state!")));
  }
  return json::parse(response);
}
}
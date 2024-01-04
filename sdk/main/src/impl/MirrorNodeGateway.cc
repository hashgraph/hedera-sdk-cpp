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
#include <string>

#include "impl/MirrorNodeGateway.h"
#include "impl/MirrorNodeRouter.h"

namespace Hedera::internal::MirrorNodeGateway
{
json AccountInfoQuery(std::string_view mirrorNodeUrl, std::string_view accountId)
{
  std::string response;
  try
  {
    const std::string url = buildUrl(mirrorNodeUrl, "accountInfoQuery", { accountId.data() });
    HttpClient httpClient;
    response = httpClient.invokeREST(url);
  }
  catch (const std::exception& e)
  {
    throw IllegalStateException(std::string(e.what() + std::string("Illegal json state!")));
  }
  return json::parse(response);
}

json AccountBalanceQuery(std::string_view mirrorNodeUrl, std::string_view accountId)
{
  // the account balance is stored in a balance object
  return AccountInfoQuery(mirrorNodeUrl, accountId)["balance"]["balance"];
}

json ContractInfoQuery(std::string_view mirrorNodeUrl, std::string_view contractId)
{
  std::string response;
  try
  {
    const std::string url = buildUrl(mirrorNodeUrl, "contractInfoQuery", { contractId.data() });
    HttpClient httpClient;
    response = httpClient.invokeREST(url);
  }
  catch (const std::exception& e)
  {
    throw IllegalStateException(std::string(e.what() + std::string("Illegal json state!")));
  }
  return json::parse(response);
}

json TokenAccountRelationshipQuery(std::string_view mirrorNodeUrl, std::string_view accountId)
{
  std::string response;
  try
  {
    const std::string url = buildUrl(mirrorNodeUrl, "tokenAccountRelationshipQuery", { accountId.data() });
    HttpClient httpClient;
    response = httpClient.invokeREST(url);
  }
  catch (const std::exception& e)
  {
    throw IllegalStateException(std::string(e.what() + std::string("Illegal json state!")));
  }
  return json::parse(response);
}

json TokensBalancesRelationshipQuery(std::string_view mirrorNodeUrl, std::string_view accountId)
{
  std::string response;
  try
  {
    const std::string url = buildUrl(mirrorNodeUrl, "tokensBalancesRelationshipQuery", { accountId.data() });
    HttpClient httpClient;
    response = httpClient.invokeREST(url);
  }
  catch (const std::exception& e)
  {
    throw IllegalStateException(std::string(e.what() + std::string("Illegal json state!")));
  }
  return json::parse(response);
}

void replaceParameters(std::string& original, const std::string& search, const std::string& replace)
{
  size_t startPos = 0;

  while ((startPos = original.find(search, startPos)) != std::string::npos)
  {
    original.replace(startPos, search.length(), replace);
    startPos += replace.length();
  }
}

std::string buildUrl(std::string_view mirrorNodeUrl, std::string_view queryType, const std::vector<std::string>& params)
{
  std::string prefix = "http://";
  std::string url = mirrorNodeUrl.data();
  if (url.compare(0, prefix.length(), prefix) != 0)
  {
    url = "https://" + url;
  }
  MirrorNodeRouter router;
  std::string route = router.getRoute(queryType).data();
  for_each(
    params.begin(), params.end(), [&route](const std::string& replace) { replaceParameters(route, "$", replace); });
  url += route;
  return url;
}
}
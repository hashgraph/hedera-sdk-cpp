/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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

#include "impl/HttpClient.h"

#include <httplib.h>
#include <stdexcept>
#include <string>
#include <string_view>

namespace Hiero::internal
{
namespace
{
// The index in a URL to begin searching for the path after the end of the URL scheme ("http://" or "https://").
const int SCHEME_END_INDEX = 8;

//
// Perform an HTTP request.
//
// @param url    The URL to which to send the request.
// @param method The HTTP method type of this request.
// @param body   The body of the request.
// @return The response of the request.
//
[[nodiscard]] std::string performRequest(std::string_view url, std::string_view method, std::string_view body)
{
  // Create an HTTP client to communicate with the given URL.
  httplib::Client client(std::string(url.substr(0, url.find('/', SCHEME_END_INDEX))));
  const std::string path = url.substr(url.find('/', SCHEME_END_INDEX)).data();

  httplib::Result res;

  // Perform the request based on the HTTP method
  if (method == "GET")
  {
    res = client.Get(path);
  }
  else if (method == "POST")
  {
    res = client.Post(path, body.data(), body.size(), "application/json");
  }
  else
  {
    throw std::invalid_argument(std::string("Unsupported HTTP method: ") + method.data());
  }

  if (!res)
  {
    throw std::runtime_error("HTTP error: " + httplib::to_string(res.error()));
  }

  return res->body;
}

} // namespace

// example infura query: rpcMethod = R"({"jsonrpc":"2.0","method":"eth_getTransactionByHash","params":[")" + hash +
// R"("],"id":1})"
std::string HttpClient::invokeRPC(std::string_view url, std::string_view rpcMethod)
{
  return performRequest(url, "POST", rpcMethod);
}

// example mirrorNode query:
// httpClient.invokeREST("https://testnet.mirrornode.hedera.com/api/v1/accounts/" + newAccountId ,"GET", "");
// note: should time out before calling this function because the mirror node is not updated on time if accountID has
// been created exactly before the call. Works without timeout if the data in the mirror node is there from some seconds
// beforehand
std::string HttpClient::invokeREST(std::string_view url, std::string_view httpMethod, std::string_view requestBody)
{
  return performRequest(url, httpMethod, requestBody);
}

//-----

} // namespace Hiero::internal
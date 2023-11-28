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

#include "impl/HttpClient.h"

namespace Hedera::internal
{
HttpClient::HttpClient()
{
  if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0)
  {
    throw std::runtime_error("Failed to initialize libcurl");
  }
}

HttpClient::~HttpClient()
{
  curl_global_cleanup();
}

// example mirrorNode query:
// httpClient.invokeREST("https://testnet.mirrornode.hedera.com/api/v1/accounts/" + newAccountId ,"GET", "");
// note: should time out before calling this function because the mirror node is not updated on time if accountID has
// been created exactly before the call. Works without timeout if the data in the mirror node is there from some seconds
// beforehand
std::string HttpClient::invokeREST(const std::string& url,
                                   const std::string& httpMethod,
                                   const std::string& requestBody)
{
  std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl(curl_easy_init(), curl_easy_cleanup);

  if (!curl)
  {
    throw std::runtime_error("CURL initialization failed");
  }

  curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());

  std::string response;

  curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, &HttpClient::writeCallback);
  curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

  curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, httpMethod.c_str());

  if (httpMethod == "POST")
  {
    curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, requestBody.c_str());
  }

  CURLcode res = curl_easy_perform(curl.get());
  if (res != CURLE_OK)
  {
    throw std::runtime_error(std::string("Error getting curl result: ") + curl_easy_strerror(res));
  }

  return response;
}

// example infura query: rpcMethod = R"({"jsonrpc":"2.0","method":"eth_getTransactionByHash","params":[")" + hash +
// R"("],"id":1})"
std::string HttpClient::invokeRPC(const std::string& url, const std::string& rpcMethod)
{
  std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl(curl_easy_init(), curl_easy_cleanup);
  if (!curl)
  {
    throw std::runtime_error("Failed to initialize libcurl");
  }

  curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl.get(), CURLOPT_POST, 1L);

  std::shared_ptr<curl_slist> headers(curl_slist_append(nullptr, "Content-Type: application/json"),
                                      curl_slist_free_all);
  curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, headers.get());

  std::string json_body = rpcMethod;
  curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, json_body.c_str());

  std::string response;
  curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, writeCallback);
  curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

  CURLcode res = curl_easy_perform(curl.get());
  if (res != CURLE_OK)
  {
    throw std::runtime_error(std::string("Error getting curl result! ") + curl_easy_strerror(res));
  }

  return response;
}

size_t HttpClient::writeCallback(char* contents, size_t size, size_t nmemb, std::string* output)
{
  size_t totalSize = size * nmemb;
  output->append(contents, totalSize);
  return totalSize;
};
} // namespace Hedera::internal
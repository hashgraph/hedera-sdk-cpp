/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_IMPL_HTTP_CLIENT_H_
#define HEDERA_SDK_CPP_IMPL_HTTP_CLIENT_H_

#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <string>

namespace Hedera::internal
{
class HttpClient
{
public:
  /**
   * Constructor for HttpClient.
   *
   * This constructor initializes the HttpClient and performs global libcurl initialization
   * using CURL_GLOBAL_DEFAULT.
   *
   * @throw std::runtime_error If libcurl initialization fails.
   */
  HttpClient();

  HttpClient(const HttpClient&) = delete;            // no use case for httpClient copy
  HttpClient& operator=(const HttpClient&) = delete; // no use case for httpClient copy

  /**
   * Destructor for HttpClient.
   *
   * This destructor cleans up global libcurl resources using curl_global_cleanup.
   * It should be called when an HttpClient instance is no longer needed to release libcurl resources.
   */
  ~HttpClient();
  /**
   * Fetches data from the specified URL using the provided RPC method.
   * @param url The URL to fetch data from.
   * @param rpcMethod The RPC method.
   * @return The fetched data as a string.
   */
  [[nodiscard]] std::string invokeRPC(const std::string& url, const std::string& rpcMethod);
  /**
   * This invokeREST function creates GET and POST requests.
   *  Can be further extended for supporting other HTTP
   *  methods or handle more advanced scenarios as needed.
   * @param url The URL to fetch data from.
   * @param httpMethod The HTTP method.
   * @param requestBody The HTTP request body.
   * @return The fetched data as a string.
   */
  [[nodiscard]] std::string invokeREST(const std::string& url,
                                       const std::string& httpMethod = "GET",
                                       const std::string& requestBody = "");
  /**
   * The callback function used for writing fetched data.
   * @param contents A pointer to the fetched data.
   * @param size The size of each element.
   * @param nmemb The number of elements.
   * @param output A pointer to the output string.
   * @return The total size of the fetched data.
   */
private:
  static size_t writeCallback(char* contents, size_t size, size_t nmemb, std::string* output);
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_HTTP_CLIENT_H_
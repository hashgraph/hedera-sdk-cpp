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

#include <httplib.h>
#include <string>
#include <string_view>

namespace Hedera::internal::HttpClient
{
/**
 * Fetches data from the specified URL using the provided RPC method.
 * @param url       The URL to fetch data from.
 * @param rpcMethod The RPC method.
 * @return The response data as a string.
 */
[[nodiscard]] std::string invokeRPC(std::string_view url, std::string_view rpcMethod);

/**
 * Create a GET or POST request. Can be further extended for supporting other HTTP methods or handle more advanced
 * scenarios as needed.
 * @param url         The URL to which to submit the request.
 * @param httpMethod  The HTTP method.
 * @param requestBody The HTTP request body.
 * @return The response data as a string.
 */
[[nodiscard]] std::string invokeREST(std::string_view url,
                                     std::string_view httpMethod = "GET",
                                     std::string_view requestBody = "");

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_HTTP_CLIENT_H_
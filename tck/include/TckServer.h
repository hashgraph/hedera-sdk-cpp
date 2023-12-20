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
#ifndef HEDERA_TCK_CPP_TCK_SERVER_H_
#define HEDERA_TCK_CPP_TCK_SERVER_H_

#include "JsonTypeMapper.h"

#include <httplib.h>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Hedera::TCK
{
class JsonRpcException;

/**
 * Class encompassing all HTTP and JSON processing of TCK requests.
 */
class TckServer
{
public:
  [[maybe_unused]] constexpr static const int DEFAULT_HTTP_PORT = 80;

  /**
   * Construct a TckServer with the default HTTP port.
   */
  TckServer();

  /**
   * Construct a TckServer with a port on which to listen for JSON requests.
   *
   * @param port The port on which to listen for JSON requests.
   */
  explicit TckServer(int port);

  /**
   * Add a JSON RPC method function.
   *
   * @param name   The name of the function.
   * @param func   The function to run.
   * @param params The parameters to input into the function.
   */
  void add(const std::string& name, const MethodHandle& func, const std::vector<std::string>& params = {});

  /**
   * Add a JSON RPC notification function.
   *
   * @param name   The name of the function.
   * @param func   The function to run.
   * @param params The parameters to input into the function.
   */
  void add(const std::string& name, const NotificationHandle& func, const std::vector<std::string>& params = {});

  /**
   * Start listening for HTTP requests. All JSON RPC functions should be added before this is called.
   */
  void startServer();

private:
  /**
   * Handle an HTTP request sent from a client.
   *
   * @param request  The HTTP request to process.
   * @param response The HTTP response to fill out.
   */
  void handleHttpRequest(const httplib::Request& request, httplib::Response& response);

  /**
   * Handle a JSON request sent from the JSON RPC client.
   *
   * @param request The JSON request to handle.
   * @return The response to the request.
   */
  std::string handleJsonRequest(std::string_view request);

  /**
   * Handle a single JSON request.
   *
   * @param request The JSON request to handle.
   * @return The JSON response.
   */
  [[nodiscard]] nlohmann::json handleSingleRequest(const nlohmann::json& request);

  /**
   * Execute a method request.
   *
   * @param method The name and method function to execute.
   * @param params The parameters to pass into the method function.
   * @return The result of the JSON RPC execution.
   * @throws JsonRpcException If there's an issue executing the method function.
   */
  [[nodiscard]] nlohmann::json executeMethod(
    const std::unordered_map<std::string, MethodHandle>::const_iterator& method,
    const nlohmann::json& params) const;

  /**
   * Execute a notification request.
   *
   * @param notification The name and notification function to execute.
   * @param params       The parameters to pass into the notification function.
   * @throws JsonRpcException If there's an issue executing the notification function.
   */
  void executeNotification(const std::unordered_map<std::string, NotificationHandle>::const_iterator& notification,
                           const nlohmann::json& params) const;

  /**
   * Normalize parameters to a JSON list.
   *
   * @param name   The name of the function.
   * @param params The parameters to normalize.
   * @return The parameters in a JSON list.
   */
  [[nodiscard]] nlohmann::json normalizeParameters(const std::string& name, const nlohmann::json& params) const;

  /**
   * Process an error thrown when trying to map JSON parameters to function inputs.
   *
   * @param name      The name of the function being executed that threw.
   * @param exception The JsonRpcException that threw.
   * @return The updated JsonRpcException.
   */
  [[nodiscard]] JsonRpcException processTypeError(const std::string& name, const JsonRpcException& exception) const;

  /**
   * Set up the handler for the HTTP server.
   */
  void setupHttpHandler();

  /**
   * Map of function names to their corresponding methods.
   */
  std::unordered_map<std::string, MethodHandle> mMethods;

  /**
   * Map of function names to their corresponding notifications.
   */
  std::unordered_map<std::string, NotificationHandle> mNotifications;

  /**
   * Map of function names to the corresponding function's parameters.
   */
  std::unordered_map<std::string, std::vector<std::string>> mParameters;

  /**
   * The HTTP server to use to receive JSON requests.
   */
  httplib::Server mHttpServer;

  /**
   * The port on which to listen for JSON requests.
   */
  int mPort = DEFAULT_HTTP_PORT;
};

} // namespace Hedera::TCK

#endif // HEDERA_TCK_CPP_TCK_SERVER_H_

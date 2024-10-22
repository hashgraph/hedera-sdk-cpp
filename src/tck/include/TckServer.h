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
#ifndef HEDERA_TCK_CPP_TCK_SERVER_H_
#define HEDERA_TCK_CPP_TCK_SERVER_H_

#include "JsonTypeMapper.h"
#include "SdkClient.h"

#include <functional>
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
   */
  void add(const std::string& name, const MethodHandle& func);

  /**
   * Add a JSON RPC notification function.
   *
   * @param name   The name of the function.
   * @param func   The function to run.
   * @param params The parameters to input into the function.
   */
  void add(const std::string& name, const NotificationHandle& func);

  /**
   * Start listening for HTTP requests. All JSON RPC functions should be added before this is called.
   */
  void startServer();

  /**
   * Create a handle for the input method.
   *
   * @tparam ReturnType The type to be returned from the method.
   * @tparam ParamTypes The parameters to pass into to method.
   * @tparam index      The index sequence to use to get parameters.
   * @param method The method of which to get the handle.
   * @return The handle of the method.
   */
  template<typename ParamsType>
  MethodHandle createMethodHandle(const std::function<nlohmann::json(const ParamsType&)>& method)
  {
    // Return the input method with additional parameter checks added. Must create a copy of method here as there are no
    // guarantees if it will remain in scope.
    // MethodHandle handle = [method](const nlohmann::json& params)
    //{
    //  if (sizeof...(ParamTypes) > 0 && !params.empty())
    //  {
    //    (checkParamType<std::decay_t<ParamTypes>>(index, params[index], getType<std::decay_t<ParamTypes>>()), ...);
    //  }
    //
    //  return method(getParam<std::decay_t<ParamTypes>>(params, index)...);
    //};
    return [method](const nlohmann::json& params) { return method(params.get<ParamsType>()); };
  }

  /**
   * Create a handle for the input notification.
   *
   * @tparam ReturnType The type to be returned from the notification.
   * @tparam ParamTypes The parameters to pass into to notification.
   * @tparam index      The index sequence to use to get parameters.
   * @param method The notification of which to get the handle.
   * @return The handle of the notification.
   */
  template<typename... ParamTypes, std::size_t... index>
  NotificationHandle createNotificationHandle(const std::function<void(ParamTypes...)>& notification,
                                              const std::index_sequence<index...>& /*unused*/)
  {
    // Return the input notification with additional parameter checks added. Must create a copy of notification here as
    // there are no guarantees if it will remain in scope.
    return [notification](const nlohmann::json& params)
    {
      if (sizeof...(ParamTypes) > 0 && !params.empty())
      {
        (checkParamType<std::decay_t<ParamTypes>>(index, params[index], getType<std::decay_t<ParamTypes>>()), ...);
      }

      return notification(getParam<std::decay_t<ParamTypes>>(params, index)...);
    };
  }

  /**
   * Create a method handle for the input method.
   *
   * @tparam ReturnType The type to be returned from the method.
   * @tparam ParamTypes The parameters to pass into to method.
   * @param method The method of which to get the handle.
   * @return The handle for the method.
   */
  template<typename ParamsType>
  MethodHandle getHandle(const std::function<nlohmann::json(const ParamsType&)>& method)
  {
    return createMethodHandle(method);
  }
  template<typename ParamsType>
  MethodHandle getHandle(nlohmann::json (*method)(const ParamsType&))
  {
    return getHandle(std::function<nlohmann::json(const ParamsType&)>(method));
  }

  /**
   * Create a handle for the input notification.
   *
   * @tparam ReturnType The type to be returned from the notification.
   * @tparam ParamTypes The parameters to pass into to notification.
   * @param notification The notification of which to get the handle.
   * @return The handle for the notification.
   */
  template<typename ParamType>
  NotificationHandle getHandle(const std::function<void(ParamType)>& notification)
  {
    return createNotificationHandle(notification);
  }
  template<typename ParamType>
  NotificationHandle getHandle(void (*notification)(ParamType))
  {
    return getHandle(std::function<void(ParamType)>(notification));
  }

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
  [[nodiscard]] nlohmann::json executeMethod(const MethodHandle& method, const nlohmann::json& params) const;

  /**
   * Execute a notification request.
   *
   * @param notification The name and notification function to execute.
   * @param params       The parameters to pass into the notification function.
   * @throws JsonRpcException If there's an issue executing the notification function.
   */
  // void executeNotification(const std::unordered_map<std::string, NotificationHandle>::const_iterator& notification,
  //                          const nlohmann::json& params) const;

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

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
#ifndef HEDERA_TCK_CPP_JSON_TYPE_MAPPER_H_
#define HEDERA_TCK_CPP_JSON_TYPE_MAPPER_H_

#include "JsonErrorType.h"
#include "JsonRpcException.h"

#include <cstddef>
#include <functional>
#include <limits>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace Hedera::TCK
{
using MethodHandle = std::function<nlohmann::json(const nlohmann::json&)>;
using NotificationHandle = std::function<void(const nlohmann::json&)>;

/**
 * Helper type traits to check if an argument is optional.
 */
// clang-format off
template<typename T> struct isOptional : std::false_type {};
template<typename T> struct isOptional<std::optional<T>> : std::true_type {};
// clang-format on

/**
 * Map a type to its corresponding JSON type.
 *
 * @tparam T The type to map.
 * @return The JSON type that corresponds with the input type.
 */
template<typename T>
constexpr nlohmann::json::value_t getType()
{
  if constexpr (isOptional<T>::value)
  {
    return getType<typename T::value_type>();
  }
  else if constexpr (std::is_same_v<T, void>)
  {
    return nlohmann::json::value_t::null;
  }
  else if constexpr (std::is_same_v<T, bool>)
  {
    return nlohmann::json::value_t::boolean;
  }
  else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, long double>)
  {
    return nlohmann::json::value_t::number_float;
  }
  else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>)
  {
    return nlohmann::json::value_t::number_integer;
  }
  else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>)
  {
    return nlohmann::json::value_t::number_unsigned;
  }
  else if constexpr (std::is_same_v<T, std::string> || std::is_enum_v<T>)
  {
    return nlohmann::json::value_t::string;
  }
  else if constexpr (std::is_same_v<T, std::vector<typename T::value_type>>)
  {
    return nlohmann::json::value_t::array;
  }
  else
  {
    return nlohmann::json::value_t::object;
  }
}

/**
 * Get the name of a type.
 *
 * @param type The type of which to get the name.
 * @return The name of the type as a string.
 */
inline std::string getTypeName(nlohmann::json::value_t type)
{
  switch (type)
  {
      // clang-format off
    case nlohmann::json::value_t::number_integer:  return "integer";
    case nlohmann::json::value_t::boolean:         return "boolean";
    case nlohmann::json::value_t::number_float:    return "float";
    case nlohmann::json::value_t::number_unsigned: return "unsigned integer";
    case nlohmann::json::value_t::object:          return "object";
    case nlohmann::json::value_t::array:           return "array";
    case nlohmann::json::value_t::string:          return "string";
    default:                                       return "null";
      // clang-format on
  }
}

/**
 * Check the JSON parameter to make sure it matches the expected type.
 *
 * @param index        The index of the JSON object to check.
 * @param param        The JSON parameter to check.
 * @param expectedType The expected type of the parameter.
 * @throws JsonRpcException If a parameter does not match its expected type.
 */
template<typename T>
void checkParamType(size_t index, const nlohmann::json& param, nlohmann::json::value_t expectedType)
{

  if constexpr (isOptional<T>::value)
  {
    if constexpr (std::is_same_v<T, std::optional<typename T::value_type>>)
    {
      // If the param isn't missing or null, check it like any other argument. If it is missing or null, then nothing
      // needs to be done.
      if (!param.is_null() && index < param.size())
      {
        checkParamType<typename T::value_type>(index, param, expectedType);
      }
    }
  }

  // If a number, make sure it's the correct kind of number (integer, float, etc.) and that its within its bounds.
  else if constexpr (std::is_arithmetic_v<T>)
  {
    if (expectedType == nlohmann::json::value_t::number_unsigned &&
        param.type() == nlohmann::json::value_t::number_integer)
    {
      if (param.get<long long int>() < 0)
      {
        throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                               "invalid parameter: must be " + getTypeName(expectedType) + ", but is " +
                                 getTypeName(param.type()),
                               index);
      }
    }
    else if (param.type() == nlohmann::json::value_t::number_unsigned &&
             expectedType == nlohmann::json::value_t::number_integer)
    {
      if (param.get<long long unsigned>() > static_cast<long long unsigned>(std::numeric_limits<T>::max()))
      {
        throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                               "invalid parameter: exceeds value range of " + getTypeName(expectedType),
                               index);
      }
    }
    else if ((param.type() == nlohmann::json::value_t::number_unsigned ||
              param.type() == nlohmann::json::value_t::number_integer) &&
             expectedType == nlohmann::json::value_t::number_float)
    {
      if (static_cast<long long int>(param.get<double>()) != param.get<long long int>())
      {
        throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                               "invalid parameter: exceeds value range of " + getTypeName(expectedType),
                               index);
      }
    }
    else if (param.type() != expectedType)
    {
      throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                             "invalid parameter: must be " + getTypeName(expectedType) + ", but is " +
                               getTypeName(param.type()),
                             index);
    }
  }

  // If not a number, just make sure it matches its expected type.
  else if (param.type() != expectedType)
  {
    throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                           "invalid parameter: must be " + getTypeName(expectedType) + ", but is " +
                             getTypeName(param.type()),
                           index);
  }
}

/**
 * Get the param located in the input params list at the specified index.
 *
 * @tparam T The type of the parameter to get.
 * @param params The JSON request parameters from which to get the parameter.
 * @param index  The index at which to get the parameter.
 * @return The requested parameter.
 * @throws JsonRpcException If the parameter is missing or null and not optional.
 */
template<typename T>
T getParam(const nlohmann::json& params, std::size_t index)
{
  // If a parameter is missing or null, determine if its optional.
  if (params.size() <= index || params[index].is_null())
  {
    // If this is an optional argument, return an empty std::optional.
    if constexpr (std::is_same_v<std::decay_t<T>, std::optional<typename std::decay_t<T>::value_type>>)
    {
      return std::decay_t<T>();
    }

    // If not optional, throw.
    throw JsonRpcException(JsonErrorType::INVALID_PARAMS,
                           "invalid parameter: missing required parameter at index " + std::to_string(index));
  }

  return params[index].get<std::decay_t<T>>();
}

/**
 * Create a handle for the input method.
 *
 * @tparam ReturnType The type to be returned from the method.
 * @tparam ParamTypes The parameters to pass into to method.
 * @tparam index      The index sequence to use to get parameters.
 * @param method The method of which to get the handle.
 * @return The handle of the method.
 */
template<typename ReturnType, typename... ParamTypes, std::size_t... index>
MethodHandle createMethodHandle(const std::function<ReturnType(ParamTypes...)>& method,
                                const std::index_sequence<index...>& /*unused*/)
{
  // Return the input method with additional parameter checks added. Must create a copy of method here as there are no
  // guarantees if it will remain in scope.
  MethodHandle handle = [method](const nlohmann::json& params)
  {
    if (sizeof...(ParamTypes) > 0 && !params.empty())
    {
      (checkParamType<std::decay_t<ParamTypes>>(index, params[index], getType<std::decay_t<ParamTypes>>()), ...);
    }

    return method(getParam<std::decay_t<ParamTypes>>(params, index)...);
  };

  return handle;
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
    if constexpr (sizeof...(ParamTypes) > 0)
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
template<typename ReturnType, typename... ParamTypes>
MethodHandle getHandle(const std::function<ReturnType(ParamTypes...)>& method)
{
  return createMethodHandle(method, std::index_sequence_for<ParamTypes...>());
}
template<typename ReturnType, typename... ParamTypes>
MethodHandle getHandle(ReturnType (*method)(ParamTypes...))
{
  return getHandle(std::function<ReturnType(ParamTypes...)>(method));
}

/**
 * Create a handle for the input notification.
 *
 * @tparam ReturnType The type to be returned from the notification.
 * @tparam ParamTypes The parameters to pass into to notification.
 * @param notification The notification of which to get the handle.
 * @return The handle for the notification.
 */
template<typename... ParamTypes>
NotificationHandle getHandle(const std::function<void(ParamTypes...)>& notification)
{
  return createNotificationHandle(notification, std::index_sequence_for<ParamTypes...>());
}
template<typename... ParamTypes>
NotificationHandle getHandle(void (*notification)(ParamTypes...))
{
  return getHandle(std::function<void(ParamTypes...)>(notification));
}

} // namespace Hedera::TCK

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert optional JSON arguments properly.
 * @tparam T The optional type.
 */
template<typename T>
struct [[maybe_unused]] adl_serializer<std::optional<T>>
{
  /**
   * Convert an optional parameter to a JSON object.
   *
   * @param jsonTo The JSON object to fill with the optional parameter.
   * @param opt    The optional parameter with which to fill the JSON object.
   */
  static void to_json(json& jsonTo, const std::optional<T>& opt) { jsonTo = opt.has_value() ? opt.value() : nullptr; }

  /**
   * Convert a JSON object to an optional parameter.
   *
   * @param jsonFrom The JSON object with which to fill the optional parameter.
   * @param opt      The optional parameter to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, std::optional<T>& opt)
  {
    opt = jsonFrom.is_null() ? std::optional<T>(std::nullopt) : jsonFrom.get<T>();
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_JSON_TYPE_MAPPER_H_

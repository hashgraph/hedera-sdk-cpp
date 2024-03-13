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
#include "JsonRpcException.h"

#include "JsonUtils.h"
#include <utility>

namespace Hedera::TCK
{
//-----
JsonRpcException JsonRpcException::fromJson(const nlohmann::json& json)
{
  if (hasKeyType(json, "code", nlohmann::json::value_t::number_integer) &&
      hasKeyType(json, "message", nlohmann::json::value_t::string))
  {
    if (hasKey(json, "data"))
    {
      return { json["code"], json["message"].get<std::string_view>(), json["data"] };
    }

    return { json["code"], json["message"].get<std::string_view>() };
  }

  return { JsonErrorType::INTERNAL_ERROR,
           R"(invalid error response: "code" (negative number) and "message" (string) are required)" };
}

//-----
JsonRpcException::JsonRpcException(JsonErrorType code, std::string_view message) noexcept
  : mCode(code)
  , mMessage(message)
  , mData(nullptr)
  , mError(std::to_string(static_cast<int>(code)) + ": " + mMessage)
{
}

//-----
JsonRpcException::JsonRpcException(JsonErrorType code, std::string_view message, nlohmann::json data) noexcept
  : mCode(code)
  , mMessage(message)
  , mData(std::move(data))
  , mError(std::to_string(static_cast<int>(code)) + ": " + mMessage + ", data: " + mData.dump())
{
}

} // namespace Hedera::TCK

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
#include "JsonUtils.h"

namespace Hedera::TCK
{
//-----
auto hasKey(const nlohmann::json& request, std::string_view key) -> bool
{
  return request.contains(key);
}

//-----
auto hasKeyType(const nlohmann::json& request, std::string_view key, nlohmann::json::value_t type) -> bool
{
  return hasKey(request, key) && request.at(key).type() == type;
}

//-----
auto hasValidId(const nlohmann::json& request) -> bool
{
  return hasKey(request, "id") && (request["id"].is_number() || request["id"].is_string() || request["id"].is_null());
}

} // namespace Hedera::TCK

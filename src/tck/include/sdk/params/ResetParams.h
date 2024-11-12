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
#ifndef HIERO_TCK_CPP_RESET_PARAMS_H_
#define HIERO_TCK_CPP_RESET_PARAMS_H_

#include <nlohmann/json_fwd.hpp>

namespace Hiero::TCK::SdkClient
{
/**
 * Struct to hold the arguments for a `reset` JSON-RPC method call. It takes no arguments
 */
struct ResetParams
{
};

} // namespace Hiero::TCK::SdkClient

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert ResetParams arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<Hiero::TCK::SdkClient::ResetParams>
{
  /**
   * Convert a JSON object to a ResetParams.
   *
   * @param jsonFrom The JSON object with which to fill the ResetParams.
   * @param params   The ResetParams to fill with the JSON object.
   */
  static void from_json(const json&, Hiero::TCK::SdkClient::ResetParams&) {}
};

} // namespace nlohmann

#endif // HIERO_TCK_CPP_RESET_PARAMS_H_

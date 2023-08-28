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
#ifndef HEDERA_SDK_CPP_IMPL_TLS_BEHAVIOR_H_
#define HEDERA_SDK_CPP_IMPL_TLS_BEHAVIOR_H_

#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

namespace Hedera::internal
{
/**
 * Enum representing different modes of network communication.
 */
enum class TLSBehavior
{
  /**
   * Communicate only in the clear
   */
  DISABLE,
  /**
   * Require TLS connection
   */
  REQUIRE
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_TLS_BEHAVIOR_H_

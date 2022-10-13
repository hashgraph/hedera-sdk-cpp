/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "NodeAddress.h"

namespace Hedera
{
//-----
NodeAddress NodeAddress::fromString(std::string_view address)
{
  NodeAddress nodeAddress;

  // TODO: add string checks
  size_t colonIndex = address.find(':');
  nodeAddress.mAddress = address.substr(0, address.size() - colonIndex + 1);
  nodeAddress.mPort =
    atoi(static_cast<std::string>(
           address.substr(colonIndex + 1,
                          address.size() - nodeAddress.mAddress.size() - 1))
           .c_str());

  return nodeAddress;
}

} // namespace Hedera

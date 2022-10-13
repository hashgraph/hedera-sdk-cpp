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
#ifndef NODE_ADDRESS_H_
#define NODE_ADDRESS_H_

#include <string>

namespace Hedera
{
class NodeAddress
{
public:
  /**
   * Get a node address from a string.
   */
  static NodeAddress fromString(std::string_view address);

private:
  /**
   * The address of the node.
   */
  std::string mAddress;

  /**
   * The port on which the node is listening.
   */
  uint32_t mPort;
};
} // namespace Hedera

#endif // NODE_ADDRESS_H_
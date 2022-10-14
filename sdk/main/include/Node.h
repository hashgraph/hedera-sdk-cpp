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
#ifndef NODE_H_
#define NODE_H_

#include "AccountId.h"
#include "Channel.h"
#include "NodeAddress.h"

#include <string>

namespace Hedera
{
class Node
{
public:
  /**
   * Default constructor.
   */
  Node() = default;

  /**
   * Construct a node from a URL and an account ID.
   *
   * @param url       The URL of the node.
   * @param accountId The account ID of the node.
   */
  explicit Node(const std::string& url, const AccountId& accountId);

  /**
   * Shutdown connections with the node.
   */
  void shutdown() const;

private:
  /**
   * The account ID associated with this node.
   */
  AccountId mAccountId;

  /**
   * The address of this node.
   */
  NodeAddress mAddress;

  /**
   * The channels this node is using to communicate.
   */
  Channel mChannel;
};

} // namespace Hedera

#endif // NODE_H_
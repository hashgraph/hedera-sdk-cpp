/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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

#ifndef HEDERA_SDK_CPP_NODEADDRESSBOOK_H
#define HEDERA_SDK_CPP_NODEADDRESSBOOK_H

#include "AccountId.h"
#include "NodeAddress.h"

#include <unordered_map>

namespace proto
{
class NodeAddressBook;
class NodeAddress;
}

namespace Hedera
{

class NodeAddressBook
{
public:
  static NodeAddressBook fromFile(const std::string& fileName);
  static NodeAddressBook fromBytes(const std::vector<char>& bytes);
  static NodeAddressBook fromProtobuf(const proto::NodeAddressBook& addressBook);

  [[nodiscard]] const std::unordered_map<AccountId, NodeAddress>& getAddressMap() const;
private:

  std::unordered_map<AccountId, NodeAddress> addressMap;
};

} // Hedera

#endif // HEDERA_SDK_CPP_NODEADDRESSBOOK_H

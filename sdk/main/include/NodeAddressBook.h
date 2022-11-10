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

#include <memory>
#include <unordered_map>

namespace proto
{
class NodeAddressBook;
class NodeAddress;
}

namespace Hedera
{

/**
 * An address book containing all known nodes on the network
 */
class NodeAddressBook
{
public:
  /**
   * Creates a new address book from a file. File must contain the serialization of an address book protobuf
   *
   * @param fileName the name of the file where the address book is encoded
   *
   * @return the newly created address book
   */
  static NodeAddressBook fromFile(const std::string& fileName);

  /**
   * Creates a new address book from the byte serialization of an address book protobuf
   *
   * @param bytes the byte serialization of an address book protobuf
   *
   * @return the newly created address book
   */
  static NodeAddressBook fromBytes(const std::vector<char>& bytes);

  /**
   * Creates a new address book from an address book protobuf
   *
   * @param addressBook the address book protobuf
   *
   * @return the newly created address book
   */
  static NodeAddressBook fromProtobuf(const proto::NodeAddressBook& addressBook);

  /**
   * Gets the map of all nodes, where the key is the associated account ID, and the value is the NodeAddress
   *
   * @return a map representing the nodes in the address book
   */
  [[nodiscard]] const std::unordered_map<AccountId, std::shared_ptr<NodeAddress>>& getAddressMap() const;

private:
  /**
   * A map from node account ID to node address
   */
  std::unordered_map<AccountId, std::shared_ptr<NodeAddress>> addressMap;
};

} // Hedera

#endif // HEDERA_SDK_CPP_NODEADDRESSBOOK_H

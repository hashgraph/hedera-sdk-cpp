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
#ifndef HEDERA_SDK_CPP_IMPL_NODE_ADDRESS_BOOK_H_
#define HEDERA_SDK_CPP_IMPL_NODE_ADDRESS_BOOK_H_

#include "AccountId.h"
#include "impl/NodeAddress.h"

#include <memory>
#include <unordered_map>

namespace proto
{
class NodeAddressBook;
class NodeAddress;
}

namespace Hedera::internal
{
/**
 * An address book containing all known nodes on the network.
 */
class NodeAddressBook
{
public:
  /**
   * Create a NodeAddressBook object from a file that contains the serialization of a NodeAddressBook protobuf object.
   *
   * @param fileName The name of the file where the NodeAddressBook protobuf object is encoded.
   * @return The created NodeAddressBook object.
   */
  static NodeAddressBook fromFile(const std::string& fileName);

  /**
   * Create a NodeAddressBook object from the byte serialization of a NodeAddressBook protobuf object.
   *
   * @param bytes The byte serialization of the NodeAddressBook protobuf object to decode.
   * @return The created NodeAddressBook object.
   */
  static NodeAddressBook fromBytes(const std::vector<char>& bytes);

  /**
   * Create a NodeAddressBook object from a NodeAddressBook protobuf object.
   *
   * @param protoAddressBook The NodeAddressBook protobuf object from which to create a NodeAddressBook object.
   * @return The created NodeAddressBook object.
   */
  static NodeAddressBook fromProtobuf(const proto::NodeAddressBook& protoAddressBook);

  /**
   * Gets the map of all nodes, where the key is the associated account ID, and the value is the NodeAddress
   *
   * @return a map representing the nodes in the address book
   */
  [[nodiscard]] const std::unordered_map<AccountId, std::shared_ptr<internal::NodeAddress>>& getAddressMap() const;

private:
  /**
   * A map from node account ID to node address.
   */
  std::unordered_map<AccountId, std::shared_ptr<internal::NodeAddress>> mAddressMap;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_NODE_ADDRESS_BOOK_H_

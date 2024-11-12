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
#include "NodeAddressBook.h"
#include "impl/Utilities.h"

#include <basic_types.pb.h>

namespace Hiero
{
//-----
NodeAddressBook NodeAddressBook::fromProtobuf(const proto::NodeAddressBook& proto)
{
  NodeAddressBook nodeAddressBook;
  for (int i = 0; i < proto.nodeaddress_size(); ++i)
  {
    nodeAddressBook.mNodeAddresses.push_back(NodeAddress::fromProtobuf(proto.nodeaddress(i)));
  }

  return nodeAddressBook;
}

//-----
NodeAddressBook NodeAddressBook::fromBytes(const std::vector<char>& bytes)
{
  proto::NodeAddressBook addressBook;
  addressBook.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(addressBook);
}

//-----
std::unique_ptr<proto::NodeAddressBook> NodeAddressBook::toProtobuf() const
{
  auto proto = std::make_unique<proto::NodeAddressBook>();
  for (const NodeAddress& address : mNodeAddresses)
  {
    *proto->add_nodeaddress() = *address.toProtobuf();
  }

  return proto;
}

//-----
std::vector<std::byte> NodeAddressBook::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
NodeAddressBook& NodeAddressBook::setNodeAddresses(const std::vector<NodeAddress>& addresses)
{
  mNodeAddresses = addresses;
  return *this;
}

} // namespace Hiero

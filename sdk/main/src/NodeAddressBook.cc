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
#include "NodeAddressBook.h"

#include <proto/basic_types.pb.h>
#include <fstream>

namespace Hedera
{
NodeAddressBook NodeAddressBook::fromFile(const std::string& fileName)
{
  std::cout << fileName << std::endl;
  std::ifstream infile(fileName, std::ios_base::binary);

  std::vector<char> buffer((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());

  return NodeAddressBook::fromBytes(buffer);
}

NodeAddressBook NodeAddressBook::fromBytes(const std::vector<char>& bytes)
{
  proto::NodeAddressBook addressBook = proto::NodeAddressBook();
  addressBook.ParseFromArray(&bytes.front(), (int)bytes.size());

  return fromProtobuf(addressBook);
}

NodeAddressBook NodeAddressBook::fromProtobuf(const proto::NodeAddressBook& addressBook)
{
  NodeAddressBook outputAddressBook = NodeAddressBook();

  for (int i = 0; i < addressBook.nodeaddress_size(); ++i)
  {
    const proto::NodeAddress& nodeAddress = addressBook.nodeaddress(i);

    outputAddressBook.addressMap.insert(
      { AccountId::fromProtobuf(nodeAddress.nodeaccountid()), NodeAddress::fromProtobuf(nodeAddress) });
  }

  return outputAddressBook;
}

const std::unordered_map<AccountId, NodeAddress>& NodeAddressBook::getAddressMap() const
{
  return addressMap;
}

} // Hedera
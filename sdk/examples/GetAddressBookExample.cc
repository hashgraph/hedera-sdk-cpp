/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "AddressBookQuery.h"
#include "Client.h"
#include "FileId.h"
#include "NodeAddressBook.h"

#include <iostream>
#include <thread>

using namespace Hedera;

int main(int argc, char** argv)
{
  // Create a client for the Hedera network of which to get the address book.
  Client client = Client::forTestnet();

  // Query for the address book using the client.
  const NodeAddressBook nodeAddressBook = AddressBookQuery().setFileId(FileId::ADDRESS_BOOK).execute(client);

  // Print off the received addresses contained in the address book.
  for (const auto& nodeAddress : nodeAddressBook.getNodeAddresses())
  {
    std::cout << nodeAddress.toString() << std::endl;
  }

  std::cout << std::endl;

  return 0;
}

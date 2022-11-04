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

#include "NodeAddressBook.h"
#include <gtest/gtest.h>

using namespace Hedera;

class NodeAddressBookTest : public ::testing::Test
{
protected:
  void SetUp() override {}
};

TEST_F(NodeAddressBookTest, readAddressBookResource)
{
  NodeAddressBook addressBook = NodeAddressBook::fromFile("mainnet.pb");
  std::cout << std::setw(20) << "Address book size: " << addressBook.getAddressMap().size() << std::endl << std::endl;

  for (const auto& elem : addressBook.getAddressMap()) {
    std::cout << elem.second.toString() << std::endl << std::endl;
  }
}

/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "AddressBookQuery.h"
#include "BaseIntegrationTest.h"
#include "FileId.h"
#include "NodeAddressBook.h"

#include <gtest/gtest.h>

using namespace Hedera;

class AddressBookQueryIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(AddressBookQueryIntegrationTests, ExecuteAddressBookQuery)
{
  // Given / When / Then
  EXPECT_NO_THROW(const NodeAddressBook nodeAddressBook =
                    AddressBookQuery().setFileId(FileId::ADDRESS_BOOK).execute(getTestClient()));
}

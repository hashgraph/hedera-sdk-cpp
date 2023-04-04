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
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"

#include <gtest/gtest.h>

using namespace Hedera;

class AccountBalanceQueryIntegrationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }

private:
  const AccountId mTestAccountId = AccountId(3ULL);
};

// Tests invoking of method execute() from AccountBalanceQuery.
TEST_F(AccountBalanceQueryIntegrationTest, ExecuteRequestToTestnetNode)
{
  // Given
  const auto testAccountId = getTestAccountId();
  Client testnetClient = Client::forTestnet();
  testnetClient.setOperator(getTestAccountId(), ED25519PrivateKey::generatePrivateKey());

  AccountBalanceQuery testAccountBalanceQuery;
  testAccountBalanceQuery.setAccountId(testAccountId);

  // When
  const Hbar hbarBalance = testAccountBalanceQuery.execute(testnetClient).getBalance();

  // Then
  EXPECT_NE(hbarBalance.toTinybars(), 0);
}
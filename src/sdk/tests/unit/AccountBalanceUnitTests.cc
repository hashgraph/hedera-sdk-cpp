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
#include "AccountBalance.h"

#include <crypto_get_account_balance.pb.h>
#include <gtest/gtest.h>

using namespace Hedera;

class AccountBalanceUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const Hbar& getTestBalance() const { return mBalance; }

private:
  const Hbar mBalance = Hbar(100LL);
};

//-----
TEST_F(AccountBalanceUnitTests, DeserializeAccountBalanceFromProtobuf)
{
  // Given
  proto::CryptoGetAccountBalanceResponse testProtoAccountBalance;
  testProtoAccountBalance.set_balance(static_cast<unsigned long long>(getTestBalance().toTinybars()));

  // When
  const AccountBalance accountBalance = AccountBalance::fromProtobuf(testProtoAccountBalance);

  // Then
  EXPECT_EQ(accountBalance.mBalance, getTestBalance());
}
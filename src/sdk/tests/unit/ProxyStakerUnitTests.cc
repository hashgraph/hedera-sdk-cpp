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
#include "ProxyStaker.h"

#include <crypto_get_stakers.pb.h>
#include <gtest/gtest.h>

using namespace Hiero;

class ProxyStakerUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const Hbar& getTestAmount() const { return mTestAmount; }

private:
  const AccountId mTestAccountId = AccountId(1ULL, 2ULL, 3ULL);
  const Hbar mTestAmount = Hbar(4LL);
};

//-----
TEST_F(ProxyStakerUnitTests, ConstructWithValues)
{
  // Given / When
  const ProxyStaker proxyStaker(getTestAccountId(), getTestAmount().toTinybars());

  // Then
  EXPECT_EQ(proxyStaker.mAccountId, getTestAccountId());
  EXPECT_EQ(proxyStaker.mAmount, getTestAmount());
}

//-----
TEST_F(ProxyStakerUnitTests, FromProtobuf)
{
  // Given
  proto::ProxyStaker protoProxyStaker;
  protoProxyStaker.set_allocated_accountid(getTestAccountId().toProtobuf().release());
  protoProxyStaker.set_amount(getTestAmount().toTinybars());

  // When
  const ProxyStaker proxyStaker = ProxyStaker::fromProtobuf(protoProxyStaker);

  // Then
  EXPECT_EQ(proxyStaker.mAccountId, getTestAccountId());
  EXPECT_EQ(proxyStaker.mAmount, getTestAmount());
}

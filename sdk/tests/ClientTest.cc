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
#include "Client.h"
#include "AccountId.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"

#include <gtest/gtest.h>

using namespace Hedera;

class ClientTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const std::unique_ptr<ED25519PrivateKey>& getTestPrivateKey() const { return mPrivateKey; }

private:
  const AccountId mAccountId = AccountId(10ULL);
  const std::unique_ptr<ED25519PrivateKey> mPrivateKey = ED25519PrivateKey::generatePrivateKey();
};

TEST_F(ClientTest, ConstructClient)
{
  Client client;
  EXPECT_FALSE(client.getOperatorAccountId());
  EXPECT_EQ(client.getOperatorPublicKey(), nullptr);
  EXPECT_FALSE(client.getDefaultMaxTransactionFee());
  EXPECT_EQ(client.getRequestTimeout(), std::chrono::minutes(2));
}

TEST_F(ClientTest, MoveClient)
{
  Client client;
  client.setOperator(getTestAccountId(), getTestPrivateKey()->clone());

  Client client2 = std::move(client);
  EXPECT_EQ(*client2.getOperatorAccountId(), getTestAccountId());
  EXPECT_EQ(client2.getOperatorPublicKey()->toString(), getTestPrivateKey()->getPublicKey()->toString());
}

TEST_F(ClientTest, SetOperator)
{
  Client client;
  client.setOperator(getTestAccountId(), getTestPrivateKey()->clone());

  EXPECT_EQ(*client.getOperatorAccountId(), getTestAccountId());
  EXPECT_EQ(client.getOperatorPublicKey()->toString(), getTestPrivateKey()->getPublicKey()->toString());

  client.setOperator(getTestAccountId(), ED25519PrivateKey::generatePrivateKey());

  // No way to grab the string value of the rvalue, just make it's not empty
  EXPECT_FALSE(client.getOperatorPublicKey()->toString().empty());
}

TEST_F(ClientTest, SetDefaultMaxTransactionFee)
{
  Client client;
  const auto fee = Hbar(1ULL);
  client.setDefaultMaxTransactionFee(fee);
  EXPECT_EQ(*client.getDefaultMaxTransactionFee(), fee);

  // Negative value should throw
  EXPECT_THROW(client.setDefaultMaxTransactionFee(fee.negated()), std::invalid_argument);
}

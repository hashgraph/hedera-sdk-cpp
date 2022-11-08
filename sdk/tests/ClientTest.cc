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
};

TEST_F(ClientTest, ConstructClient)
{
  Client client;
  EXPECT_EQ(client.getOperator(), nullptr);
  EXPECT_EQ(client.getDefaultMaxTransactionFee(), nullptr);
  EXPECT_EQ(client.getRequestTimeout(), std::chrono::minutes(2));
}

TEST_F(ClientTest, SetOperator)
{
  Client client;
  const auto accountId = AccountId("0.0.10");
  std::unique_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();
  const std::string publicKeyStr = privateKey->getPublicKey()->toString();
  client.setOperator(accountId, privateKey);

  EXPECT_NE(client.getOperator(), nullptr);
  EXPECT_EQ(client.getOperatorAccountId(), accountId);
  EXPECT_EQ(client.getOperatorPublicKey()->toString(), publicKeyStr);
}

TEST_F(ClientTest, SetDefaultMaxTransactionFee)
{
  Client client;
  const auto fee = Hbar(1ULL);
  client.setDefaultMaxTransactionFee(fee);
  EXPECT_EQ(*client.getDefaultMaxTransactionFee(), fee);

  // Negative value should throw
  EXPECT_ANY_THROW(client.setDefaultMaxTransactionFee(fee.negated()));
}

TEST_F(ClientTest, CloseClient)
{
  Client client = Client::forTestnet();
  client.close();
}
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
#include "AccountCreateTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/UninitializedException.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace Hedera;

class ClientTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const std::unique_ptr<ED25519PrivateKey>& getTestPrivateKey() const { return mPrivateKey; }
  [[nodiscard]] inline const std::string getPathToJSON() const { return mFilePath.string(); }

private:
  const AccountId mAccountId = AccountId(10ULL);
  const std::unique_ptr<ED25519PrivateKey> mPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const std::filesystem::path mFilePath = std::filesystem::current_path() / "local_node.json";
};

//-----
TEST_F(ClientTest, ConstructClient)
{
  Client client;
  EXPECT_FALSE(client.getOperatorAccountId());
  EXPECT_EQ(client.getOperatorPublicKey(), nullptr);
  EXPECT_FALSE(client.getMaxTransactionFee());
  EXPECT_EQ(client.getRequestTimeout(), std::chrono::minutes(2));
}

//-----
TEST_F(ClientTest, MoveClient)
{
  Client client;
  client.setOperator(getTestAccountId(), getTestPrivateKey().get());

  Client client2 = std::move(client);
  EXPECT_EQ(*client2.getOperatorAccountId(), getTestAccountId());
  EXPECT_EQ(client2.getOperatorPublicKey()->toStringDer(), getTestPrivateKey()->getPublicKey()->toStringDer());
}

//-----
TEST_F(ClientTest, SetOperator)
{
  Client client;
  client.setOperator(getTestAccountId(), getTestPrivateKey().get());

  EXPECT_EQ(*client.getOperatorAccountId(), getTestAccountId());
  EXPECT_EQ(client.getOperatorPublicKey()->toStringDer(), getTestPrivateKey()->getPublicKey()->toStringDer());

  client.setOperator(getTestAccountId(), ED25519PrivateKey::generatePrivateKey().get());

  // No way to grab the string value of the rvalue, just make it's not empty
  EXPECT_FALSE(client.getOperatorPublicKey()->toStringDer().empty());
}

//-----
TEST_F(ClientTest, SignWithOperator)
{
  const std::vector<std::byte> bytesToSign = { std::byte(0x1), std::byte(0x2), std::byte(0x3) };

  Client client;
  EXPECT_THROW(auto bytes = client.sign(bytesToSign), UninitializedException);

  client.setOperator(getTestAccountId(), getTestPrivateKey().get());
  EXPECT_TRUE(getTestPrivateKey()->getPublicKey()->verifySignature(client.sign(bytesToSign), bytesToSign));
}

//-----
TEST_F(ClientTest, GetNodes)
{
  Client client;
  EXPECT_THROW(auto nodes = client.getNodesWithAccountIds({ getTestAccountId() }), UninitializedException);
}

//-----
TEST_F(ClientTest, SetDefaultMaxTransactionFee)
{
  Client client;
  const auto fee = Hbar(1ULL);
  client.setMaxTransactionFee(fee);
  EXPECT_EQ(*client.getMaxTransactionFee(), fee);

  // Negative value should throw
  EXPECT_THROW(client.setMaxTransactionFee(fee.negated()), std::invalid_argument);
}

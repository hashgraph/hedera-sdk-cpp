/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "Defaults.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"

#include <gtest/gtest.h>

using namespace Hedera;

class ClientTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const std::shared_ptr<ED25519PrivateKey>& getTestPrivateKey() const { return mPrivateKey; }
  [[nodiscard]] inline const std::chrono::system_clock::duration& getTestNetworkUpdatePeriod() const
  {
    return mTestNetworkUpdatePeriod;
  }

  [[nodiscard]] inline const std::chrono::milliseconds& getNegativeBackoffTime() const { return mNegativeBackoffTime; }
  [[nodiscard]] inline const std::chrono::milliseconds& getZeroBackoffTime() const { return mZeroBackoffTime; }
  [[nodiscard]] inline const std::chrono::milliseconds& getBelowMinBackoffTime() const { return mBelowMinBackoffTime; }
  [[nodiscard]] inline const std::chrono::milliseconds& getAboveMaxBackoffTime() const { return mAboveMaxBackoffTime; }

private:
  const AccountId mAccountId = AccountId(10ULL);
  const std::shared_ptr<ED25519PrivateKey> mPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const std::chrono::system_clock::duration mTestNetworkUpdatePeriod = std::chrono::seconds(2);

  const std::chrono::milliseconds mNegativeBackoffTime = std::chrono::milliseconds(-1);
  const std::chrono::milliseconds mZeroBackoffTime = std::chrono::milliseconds(0);
  const std::chrono::milliseconds mBelowMinBackoffTime = DEFAULT_MIN_BACKOFF - std::chrono::milliseconds(1);
  const std::chrono::milliseconds mAboveMaxBackoffTime = DEFAULT_MAX_BACKOFF + std::chrono::milliseconds(1);
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
  client.setOperator(getTestAccountId(), getTestPrivateKey());

  Client client2 = std::move(client);
  EXPECT_EQ(*client2.getOperatorAccountId(), getTestAccountId());
  EXPECT_EQ(client2.getOperatorPublicKey()->toStringDer(), getTestPrivateKey()->getPublicKey()->toStringDer());
}

//-----
TEST_F(ClientTest, SetOperator)
{
  Client client;
  client.setOperator(getTestAccountId(), getTestPrivateKey());

  EXPECT_EQ(*client.getOperatorAccountId(), getTestAccountId());
  EXPECT_EQ(client.getOperatorPublicKey()->toStringDer(), getTestPrivateKey()->getPublicKey()->toStringDer());

  client.setOperator(getTestAccountId(), ED25519PrivateKey::generatePrivateKey());

  // No way to grab the string value of the rvalue, just make it's not empty
  EXPECT_FALSE(client.getOperatorPublicKey()->toStringDer().empty());
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

//-----
TEST_F(ClientTest, SetNetworkUpdatePeriod)
{
  // Given
  Client client;

  // When
  client.setNetworkUpdatePeriod(getTestNetworkUpdatePeriod());

  // Then
  EXPECT_EQ(client.getNetworkUpdatePeriod(), getTestNetworkUpdatePeriod());
}

//-----
TEST_F(ClientTest, SetInvalidMinBackoff)
{
  // Given
  std::unordered_map<std::string, AccountId> networkMap;
  Client client = Client::forNetwork(networkMap);

  // When / Then
  EXPECT_THROW(client.setMinBackoff(getNegativeBackoffTime()), std::invalid_argument); // INVALID_ARGUMENT
  EXPECT_THROW(client.setMinBackoff(getAboveMaxBackoffTime()), std::invalid_argument); // INVALID_ARGUMENT
}

//-----
TEST_F(ClientTest, SetValidMinBackoff)
{
  // Given
  std::unordered_map<std::string, AccountId> networkMap;
  Client client = Client::forNetwork(networkMap);

  // When / Then
  EXPECT_NO_THROW(client.setMinBackoff(getZeroBackoffTime()));
  EXPECT_NO_THROW(client.setMinBackoff(DEFAULT_MIN_BACKOFF));
  EXPECT_NO_THROW(client.setMinBackoff(DEFAULT_MAX_BACKOFF));
}

//-----
TEST_F(ClientTest, SetInvalidMaxBackoff)
{
  // Given
  std::unordered_map<std::string, AccountId> networkMap;
  Client client = Client::forNetwork(networkMap);

  // When / Then
  EXPECT_THROW(client.setMaxBackoff(getNegativeBackoffTime()), std::invalid_argument); // INVALID_ARGUMENT
  EXPECT_THROW(client.setMaxBackoff(getZeroBackoffTime()), std::invalid_argument);     // INVALID_ARGUMENT
  EXPECT_THROW(client.setMaxBackoff(getBelowMinBackoffTime()), std::invalid_argument); // INVALID_ARGUMENT
  EXPECT_THROW(client.setMaxBackoff(getAboveMaxBackoffTime()), std::invalid_argument); // INVALID_ARGUMENT
}

//-----
TEST_F(ClientTest, SetValidMaxBackoff)
{
  // Given
  std::unordered_map<std::string, AccountId> networkMap;
  Client client = Client::forNetwork(networkMap);

  // When / Then
  EXPECT_NO_THROW(client.setMaxBackoff(DEFAULT_MIN_BACKOFF));
  EXPECT_NO_THROW(client.setMaxBackoff(DEFAULT_MAX_BACKOFF));
}
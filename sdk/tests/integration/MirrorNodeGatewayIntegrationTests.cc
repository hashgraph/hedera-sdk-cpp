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

#include "BaseIntegrationTest.h"
#include "impl/MirrorNodeGateway.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

using json = nlohmann::json;
using namespace Hedera;

class MirrorNodeGatewayIntegrationTests : public BaseIntegrationTest
{
protected:
  [[nodiscard]] inline const std::string& getAccountIdStr() const { return mAccountIdStr; }

private:
  const std::string mAccountIdStr = "0.0.3";
};

//-----
TEST_F(MirrorNodeGatewayIntegrationTests, AccountBalanceQuery)
{
  // Given
  const std::string& accountIdStr = getAccountIdStr();

  // When
  json response;
  ASSERT_NO_THROW(response = internal::MirrorNodeGateway::AccountBalanceQuery(
                    accountIdStr, internal::MirrorNodeGateway::forLocalNode););

  // Then
  ASSERT_FALSE(response.empty());        // checks if any data
  EXPECT_FALSE(response.dump().empty()); // checks for balance data
}

//-----
TEST_F(MirrorNodeGatewayIntegrationTests, AccountInfoQuery)
{
  // Given
  const std::string& accountIdStr = getAccountIdStr();

  // When
  json response;
  ASSERT_NO_THROW(
    response = internal::MirrorNodeGateway::AccountInfoQuery(accountIdStr, internal::MirrorNodeGateway::forLocalNode););

  // Then
  ASSERT_FALSE(response.empty());           // checks if any data
  EXPECT_TRUE(response["_status"].empty()); // if status is in json then not found
}

//-----
TEST_F(MirrorNodeGatewayIntegrationTests, ContractInfoQuery)
{
  // Given
  const std::string& contractIdStr = getAccountIdStr();

  // When
  json response;
  ASSERT_NO_THROW(response = internal::MirrorNodeGateway::ContractInfoQuery(
                    contractIdStr, internal::MirrorNodeGateway::forLocalNode););

  // Then
  ASSERT_FALSE(response.empty());            // checks if any data
  EXPECT_FALSE(response["_status"].empty()); // no such contract exists then should have _status not found
}

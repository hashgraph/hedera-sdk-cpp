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

#include "BaseIntegrationTest.h"
#include "impl/HttpClient.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

using json = nlohmann::json;
using namespace Hiero;

class HttpClientIntegrationTests : public BaseIntegrationTest
{
protected:
  [[nodiscard]] inline const std::string& getURL() const { return mUrl; }
  [[nodiscard]] inline const std::string& getJsonMirrorNetworkTag() const { return mJsonMirrorNetworkTag; }
  [[nodiscard]] inline const std::string& getAccountIdStr() const { return mAccountIdStr; }

private:
  const std::string mUrl = "http://127.0.0.1:5551/api/v1/accounts/";
  const std::string mJsonMirrorNetworkTag = "mirrorNetwork";
  const std::string mAccountIdStr = "0.0.3";
};

//-----
TEST_F(HttpClientIntegrationTests, GETAccountFromLocalMirrorNode)
{
  // Given
  const std::string& mirrorNetworkTag = getJsonMirrorNetworkTag();
  const std::string& accountIdStr = getAccountIdStr();
  std::string mUrl(getURL());
  mUrl += accountIdStr;

  // When
  std::string response;
  ASSERT_NO_THROW(response = internal::HttpClient::invokeREST(mUrl, "GET"));

  // Then
  json responseData = json::parse(response);

  ASSERT_FALSE(responseData.empty());
  EXPECT_FALSE(responseData["account"].empty());
  EXPECT_FALSE(responseData["created_timestamp"].empty());

  EXPECT_EQ(responseData["account"], accountIdStr);
}

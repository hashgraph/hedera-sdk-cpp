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
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class JSONTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::string getPathToJSON() const { return mFilePath.string(); }

private:
  const std::filesystem::path mFilePath = std::filesystem::current_path() / "local_node.json";
};

// Tests JSON parsing of a configuration file for the Local Node.
TEST_F(JSONTest, ParseJSONConfig)
{
  // Given
  std::string testPathToJSON = getPathToJSON();
  std::ifstream testInputFile(testPathToJSON, std::ios::in);

  try
  {
    // When
    json jsonData = json::parse(testInputFile);

    // Then
    EXPECT_FALSE(jsonData.empty());
  }
  catch (json::parse_error& error)
  {
    std::cout << error.what() << '\n';

    EXPECT_TRUE(false);
  }

  testInputFile.close();
}
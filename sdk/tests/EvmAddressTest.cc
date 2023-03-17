/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "EvmAddress.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace Hedera;

class EvmAddressTest : public testing::Test
{
protected:
  [[nodiscard]] inline const std::string& getTestString() const { return mTestString; }
  [[nodiscard]] inline const std::vector<unsigned char>& getTestBytes() const { return mTestBytes; }

private:
  const std::string mTestString = "303132333435363738396162636465666768696A";
  const std::vector<unsigned char> mTestBytes = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                                  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
};

TEST_F(EvmAddressTest, StringConstructor)
{
  EXPECT_NO_THROW(auto evmAddress = EvmAddress::fromString(getTestString()));
  EXPECT_NO_THROW(auto evmAddress = EvmAddress::fromString("0x" + getTestString()));

  // String too short
  std::string badString = getTestString();
  badString.pop_back();
  EXPECT_THROW(auto evmAddress = EvmAddress::fromString(badString), std::invalid_argument);
  EXPECT_THROW(auto evmAddress = EvmAddress::fromString("0x" + badString), std::invalid_argument);

  // String contains non-hex characters
  badString.push_back('x');
  EXPECT_THROW(auto evmAddress = EvmAddress::fromString(badString), std::invalid_argument);
  EXPECT_THROW(auto evmAddress = EvmAddress::fromString("0x" + badString), std::invalid_argument);

  // String contains prefix not at beginning of string
  badString.pop_back();
  badString.pop_back();
  badString.insert(badString.size() / 2, "0x");
  EXPECT_THROW(auto evmAddress = EvmAddress::fromString(badString), std::invalid_argument);
  EXPECT_THROW(auto evmAddress = EvmAddress::fromString("0x" + badString), std::invalid_argument);
}

TEST_F(EvmAddressTest, ByteConstructor)
{
  EXPECT_NO_THROW(auto evmAddress = EvmAddress::fromBytes(getTestBytes()));

  std::vector<unsigned char> badBytes = getTestBytes();

  // Byte array too small
  badBytes.pop_back();
  EXPECT_THROW(auto evmAddress = EvmAddress::fromBytes(badBytes), std::invalid_argument);

  // Byte array too big
  badBytes.push_back(255);
  badBytes.push_back(172);
  EXPECT_THROW(auto evmAddress = EvmAddress::fromBytes(badBytes), std::invalid_argument);
}

TEST_F(EvmAddressTest, StringByteEquality)
{
  std::string testString = getTestString();
  std::vector<unsigned char> testBytes = getTestBytes();

  EXPECT_EQ(EvmAddress::fromString(testString).toBytes(), testBytes);
  EXPECT_EQ(EvmAddress::fromBytes(testBytes).toString(), testString);

  testString.front() = '4';
  testBytes.front() = '@';

  EXPECT_EQ(EvmAddress::fromString(testString).toBytes(), testBytes);
  EXPECT_EQ(EvmAddress::fromBytes(testBytes).toString(), testString);
}
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
  [[nodiscard]] inline const std::vector<std::byte>& getTestBytes() const { return mTestBytes; }

private:
  const std::string mTestString = "303132333435363738396162636465666768696A";
  const std::vector<std::byte> mTestBytes = { std::byte('0'), std::byte('1'), std::byte('2'), std::byte('3'),
                                              std::byte('4'), std::byte('5'), std::byte('6'), std::byte('7'),
                                              std::byte('8'), std::byte('9'), std::byte('a'), std::byte('b'),
                                              std::byte('c'), std::byte('d'), std::byte('e'), std::byte('f'),
                                              std::byte('g'), std::byte('h'), std::byte('i'), std::byte('j') };
};

TEST_F(EvmAddressTest, StringConstructor)
{
  EXPECT_NO_THROW(EvmAddress::fromString(getTestString()));
  EXPECT_NO_THROW(EvmAddress::fromString("0x" + getTestString()));

  // String too short
  std::string badString = getTestString();
  badString.pop_back();
  EXPECT_THROW(EvmAddress::fromString(badString), std::invalid_argument);
  EXPECT_THROW(EvmAddress::fromString("0x" + badString), std::invalid_argument);

  // String contains non-hex characters
  badString.push_back('x');
  EXPECT_THROW(EvmAddress::fromString(badString), std::invalid_argument);
  EXPECT_THROW(EvmAddress::fromString("0x" + badString), std::invalid_argument);

  // String contains prefix not at beginning of string
  badString.pop_back();
  badString.pop_back();
  badString.insert(badString.size() / 2, "0x");
  EXPECT_THROW(EvmAddress::fromString(badString), std::invalid_argument);
  EXPECT_THROW(EvmAddress::fromString("0x" + badString), std::invalid_argument);
}

TEST_F(EvmAddressTest, ByteConstructor)
{
  EXPECT_NO_THROW(EvmAddress::fromBytes(getTestBytes()));

  std::vector<std::byte> badBytes = getTestBytes();

  // Byte array too small
  badBytes.pop_back();
  EXPECT_THROW(EvmAddress::fromBytes(badBytes), std::invalid_argument);

  // Byte array too big
  badBytes.push_back(std::byte(255));
  badBytes.push_back(std::byte(172));
  EXPECT_THROW(EvmAddress::fromBytes(badBytes), std::invalid_argument);
}

TEST_F(EvmAddressTest, StringByteEquality)
{
  std::string testString = getTestString();
  std::vector<std::byte> testBytes = getTestBytes();

  EXPECT_EQ(EvmAddress::fromString(testString).toBytes(), testBytes);
  EXPECT_EQ(EvmAddress::fromBytes(testBytes).toString(), testString);

  testString.front() = '4';
  testBytes.front() = std::byte('@');

  EXPECT_EQ(EvmAddress::fromString(testString).toBytes(), testBytes);
  EXPECT_EQ(EvmAddress::fromBytes(testBytes).toString(), testString);
}
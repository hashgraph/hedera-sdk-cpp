/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "NftId.h"

#include <gtest/gtest.h>
#include <limits>
#include <token_get_nft_info.pb.h>

using namespace Hedera;

class NftIdUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTokenId; }
  [[nodiscard]] inline const uint64_t& getTestSerialNum() const { return mSerialNum; }

private:
  const TokenId mTokenId = TokenId(10ULL, 200ULL, 3000ULL);
  const uint64_t mSerialNum = 40000ULL;
};

//-----
TEST_F(NftIdUnitTests, ConstructWithTokenIdSerialNum)
{
  // Given / When
  const NftId nftId(getTestTokenId(), getTestSerialNum());

  // Then
  EXPECT_EQ(nftId.mTokenId, getTestTokenId());
  EXPECT_EQ(nftId.mSerialNum, getTestSerialNum());
}

//-----
TEST_F(NftIdUnitTests, CompareNftIds)
{
  EXPECT_TRUE(NftId() == NftId());
  EXPECT_TRUE(NftId(getTestTokenId(), getTestSerialNum()) == NftId(getTestTokenId(), getTestSerialNum()));
}

//-----
TEST_F(NftIdUnitTests, ConstructFromString)
{
  // Given
  const std::string testTokenIdStr = getTestTokenId().toString();
  const std::string testSerialNumSr = std::to_string(getTestSerialNum());

  NftId nftId;
  EXPECT_NO_THROW(nftId = NftId::fromString(testTokenIdStr + '/' + testSerialNumSr));
  EXPECT_EQ(nftId.mTokenId, getTestTokenId());
  EXPECT_EQ(nftId.mSerialNum, getTestSerialNum());

  EXPECT_THROW(nftId = NftId::fromString(testTokenIdStr + testSerialNumSr), std::invalid_argument);
  EXPECT_THROW(nftId = NftId::fromString('/' + testTokenIdStr + testSerialNumSr), std::invalid_argument);
  EXPECT_THROW(nftId = NftId::fromString(testTokenIdStr + testSerialNumSr + '/'), std::invalid_argument);
  EXPECT_THROW(nftId = NftId::fromString("//" + testTokenIdStr + testSerialNumSr), std::invalid_argument);
  EXPECT_THROW(nftId = NftId::fromString('/' + testTokenIdStr + '/' + testSerialNumSr), std::invalid_argument);
  EXPECT_THROW(nftId = NftId::fromString(testTokenIdStr + '/' + testSerialNumSr + '/'), std::invalid_argument);
  EXPECT_THROW(nftId = NftId::fromString(testTokenIdStr + "//" + testSerialNumSr), std::invalid_argument);

  EXPECT_THROW(nftId = NftId::fromString(testTokenIdStr + "/abc"), std::invalid_argument);
  EXPECT_THROW(nftId = NftId::fromString(testTokenIdStr + "/o.o.e"), std::invalid_argument);
  EXPECT_THROW(nftId = NftId::fromString(testTokenIdStr + "/0001!"), std::invalid_argument);
}

//-----
TEST_F(NftIdUnitTests, ProtobufNftId)
{
  NftId nftId(getTestTokenId(), getTestSerialNum());

  // Serialize token ID and serial number
  std::unique_ptr<proto::NftID> protoNftID = nftId.toProtobuf();
  EXPECT_EQ(TokenId::fromProtobuf(protoNftID->token_id()), getTestTokenId());
  EXPECT_EQ(protoNftID->serial_number(), getTestSerialNum());

  // Adjust protobuf fields
  protoNftID->set_serial_number(static_cast<int64_t>(getTestSerialNum() - 1ULL));

  // Deserialize token ID and serial number
  nftId = NftId::fromProtobuf(*protoNftID);
  EXPECT_EQ(nftId.mTokenId, getTestTokenId());
  EXPECT_EQ(nftId.mSerialNum, getTestSerialNum() - 1ULL);
}

//-----
TEST_F(NftIdUnitTests, ToString)
{
  NftId nftId;
  EXPECT_EQ(nftId.toString(), "0.0.0/0");

  nftId.mTokenId = getTestTokenId();
  nftId.mSerialNum = getTestSerialNum();
  EXPECT_EQ(nftId.toString(), getTestTokenId().toString() + '/' + std::to_string(getTestSerialNum()));
}

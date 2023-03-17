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
#include "TokenId.h"

#include <gtest/gtest.h>
#include <limits>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class TokenIdTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const uint64_t& getTestShardNum() const { return mShardNum; }
  [[nodiscard]] inline const uint64_t& getTestRealmNum() const { return mRealmNum; }
  [[nodiscard]] inline const uint64_t& getTestTokenNum() const { return mTokenNum; }
  [[nodiscard]] inline const uint64_t& getTestNumTooBig() const { return mNumTooBig; }

private:
  const uint64_t mShardNum = 8ULL;
  const uint64_t mRealmNum = 90ULL;
  const uint64_t mTokenNum = 1000ULL;
  const uint64_t mNumTooBig = static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1ULL;
};

//-----
TEST_F(TokenIdTest, DefaultConstructTokenId)
{
  const TokenId tokenId;
  EXPECT_EQ(tokenId.getShardNum(), 0ULL);
  EXPECT_EQ(tokenId.getRealmNum(), 0ULL);
  EXPECT_EQ(tokenId.getTokenNum(), 0ULL);
}

//-----
TEST_F(TokenIdTest, ConstructWithTokenNum)
{
  const TokenId tokenId(getTestTokenNum());
  EXPECT_EQ(tokenId.getShardNum(), 0ULL);
  EXPECT_EQ(tokenId.getRealmNum(), 0ULL);
  EXPECT_EQ(tokenId.getTokenNum(), getTestTokenNum());

  EXPECT_THROW(TokenId{ getTestNumTooBig() }, std::invalid_argument);
}

//-----
TEST_F(TokenIdTest, ConstructWithShardRealmTokenNum)
{
  const TokenId tokenId(getTestShardNum(), getTestRealmNum(), getTestTokenNum());
  EXPECT_EQ(tokenId.getShardNum(), getTestShardNum());
  EXPECT_EQ(tokenId.getRealmNum(), getTestRealmNum());
  EXPECT_EQ(tokenId.getTokenNum(), getTestTokenNum());

  EXPECT_THROW((TokenId{ getTestNumTooBig(), getTestRealmNum(), getTestTokenNum() }), std::invalid_argument);
  EXPECT_THROW((TokenId{ getTestShardNum(), getTestNumTooBig(), getTestTokenNum() }), std::invalid_argument);
  EXPECT_THROW((TokenId{ getTestShardNum(), getTestRealmNum(), getTestNumTooBig() }), std::invalid_argument);
}

//-----
TEST_F(TokenIdTest, CompareTokenIds)
{
  EXPECT_TRUE(TokenId() == TokenId());
  EXPECT_TRUE(TokenId(getTestTokenNum()) == TokenId(getTestTokenNum()));
  EXPECT_TRUE(TokenId(getTestShardNum(), getTestRealmNum(), getTestTokenNum()) ==
              TokenId(getTestShardNum(), getTestRealmNum(), getTestTokenNum()));
}

//-----
TEST_F(TokenIdTest, ConstructFromString)
{
  const std::string testShardNumStr = std::to_string(getTestShardNum());
  const std::string testRealmNumStr = std::to_string(getTestRealmNum());
  const std::string testTokenNumStr = std::to_string(getTestTokenNum());
  const std::string testNumTooBigStr = std::to_string(getTestNumTooBig());

  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = TokenId::fromString(testShardNumStr + '.' + testRealmNumStr + '.' + testTokenNumStr));
  EXPECT_EQ(tokenId.getShardNum(), getTestShardNum());
  EXPECT_EQ(tokenId.getRealmNum(), getTestRealmNum());
  EXPECT_EQ(tokenId.getTokenNum(), getTestTokenNum());

  EXPECT_THROW(tokenId = TokenId::fromString(testShardNumStr + testRealmNumStr + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString('.' + testShardNumStr + testRealmNumStr + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(testShardNumStr + '.' + testRealmNumStr + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(testShardNumStr + testRealmNumStr + '.' + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(testShardNumStr + testRealmNumStr + testTokenNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(".." + testShardNumStr + testRealmNumStr + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString('.' + testShardNumStr + '.' + testRealmNumStr + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString('.' + testShardNumStr + testRealmNumStr + '.' + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString('.' + testShardNumStr + testRealmNumStr + testTokenNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(testShardNumStr + ".." + testRealmNumStr + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(testShardNumStr + '.' + testRealmNumStr + testTokenNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(testShardNumStr + testRealmNumStr + ".." + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(testShardNumStr + testRealmNumStr + '.' + testTokenNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(tokenId =
                 TokenId::fromString('.' + testShardNumStr + '.' + testRealmNumStr + '.' + testTokenNumStr + '.'),
               std::invalid_argument);

  EXPECT_THROW(tokenId = TokenId::fromString("abc"), std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString("o.o.e"), std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString("0.0.1!"), std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(testNumTooBigStr + '.' + testRealmNumStr + '.' + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(testShardNumStr + '.' + testNumTooBigStr + '.' + testTokenNumStr),
               std::invalid_argument);
  EXPECT_THROW(tokenId = TokenId::fromString(testShardNumStr + '.' + testRealmNumStr + '.' + testNumTooBigStr),
               std::invalid_argument);
}

//-----
TEST_F(TokenIdTest, ProtobufTokenId)
{
  TokenId tokenId;
  tokenId.setShardNum(getTestShardNum());
  tokenId.setRealmNum(getTestRealmNum());
  tokenId.setTokenNum(getTestTokenNum());

  // Serialize shard, realm, token number
  std::unique_ptr<proto::TokenID> protoTokenId = tokenId.toProtobuf();
  EXPECT_EQ(protoTokenId->shardnum(), getTestShardNum());
  EXPECT_EQ(protoTokenId->realmnum(), getTestRealmNum());
  EXPECT_EQ(protoTokenId->tokennum(), getTestTokenNum());

  // Adjust protobuf fields
  const uint64_t adjustment = 3ULL;
  const uint64_t newShard = getTestShardNum() + adjustment;
  const uint64_t newRealm = getTestRealmNum() - adjustment;
  const uint64_t newToken = getTestTokenNum() * adjustment;

  protoTokenId->set_shardnum(static_cast<int64_t>(newShard));
  protoTokenId->set_realmnum(static_cast<int64_t>(newRealm));
  protoTokenId->set_tokennum(static_cast<int64_t>(newToken));

  // Deserialize shard, realm, token number
  tokenId = TokenId::fromProtobuf(*protoTokenId);
  EXPECT_EQ(tokenId.getShardNum(), newShard);
  EXPECT_EQ(tokenId.getRealmNum(), newRealm);
  EXPECT_EQ(tokenId.getTokenNum(), newToken);
}

//-----
TEST_F(TokenIdTest, ToString)
{
  TokenId tokenId;
  EXPECT_EQ(tokenId.toString(), "0.0.0");

  tokenId.setShardNum(getTestShardNum());
  tokenId.setRealmNum(getTestRealmNum());
  tokenId.setTokenNum(getTestTokenNum());
  EXPECT_EQ(tokenId.toString(),
            std::to_string(getTestShardNum()) + '.' + std::to_string(getTestRealmNum()) + '.' +
              std::to_string(getTestTokenNum()));
}

//-----
TEST_F(TokenIdTest, SetGetShardNum)
{
  TokenId tokenId;
  tokenId.setShardNum(getTestShardNum());

  EXPECT_EQ(tokenId.getShardNum(), getTestShardNum());
  EXPECT_THROW(tokenId.setShardNum(getTestNumTooBig()), std::invalid_argument);
}

//-----
TEST_F(TokenIdTest, SetGetRealmNum)
{
  TokenId tokenId;
  tokenId.setRealmNum(getTestRealmNum());

  EXPECT_EQ(tokenId.getRealmNum(), getTestRealmNum());
  EXPECT_THROW(tokenId.setRealmNum(getTestNumTooBig()), std::invalid_argument);
}

//-----
TEST_F(TokenIdTest, SetGetAccountNum)
{
  TokenId tokenId;
  tokenId.setTokenNum(getTestTokenNum());

  EXPECT_EQ(tokenId.getTokenNum(), getTestTokenNum());
  EXPECT_THROW(tokenId.setTokenNum(getTestNumTooBig()), std::invalid_argument);
}

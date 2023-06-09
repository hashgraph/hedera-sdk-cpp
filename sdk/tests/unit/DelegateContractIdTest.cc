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
#include "DelegateContractId.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <limits>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class DelegateContractIdTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const uint64_t& getTestShardNum() const { return mShardNum; }
  [[nodiscard]] inline const uint64_t& getTestRealmNum() const { return mRealmNum; }
  [[nodiscard]] inline const uint64_t& getTestContractNum() const { return mContractNum; }
  [[nodiscard]] inline const uint64_t& getTestNumTooBig() const { return mNumTooBig; }

private:
  const uint64_t mShardNum = 1ULL;
  const uint64_t mRealmNum = 2ULL;
  const uint64_t mContractNum = 3ULL;
  const uint64_t mNumTooBig = static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1ULL;
};

//-----
TEST_F(DelegateContractIdTest, DefaultConstructDelegateContractId)
{
  // Given / When
  const DelegateContractId delegateContractId;

  // Then
  EXPECT_EQ(delegateContractId.getShardNum(), 0ULL);
  EXPECT_EQ(delegateContractId.getRealmNum(), 0ULL);
  EXPECT_FALSE(delegateContractId.getContractNum().has_value());
  EXPECT_FALSE(delegateContractId.getEvmAddress().has_value());
}

//-----
TEST_F(DelegateContractIdTest, ConstructWithContractNum)
{
  // Given / When
  const DelegateContractId delegateContractId(getTestContractNum());

  // Then
  EXPECT_EQ(delegateContractId.getShardNum(), 0ULL);
  EXPECT_EQ(delegateContractId.getRealmNum(), 0ULL);
  ASSERT_TRUE(delegateContractId.getContractNum().has_value());
  EXPECT_EQ(delegateContractId.getContractNum(), getTestContractNum());
  EXPECT_FALSE(delegateContractId.getEvmAddress().has_value());
}

//-----
TEST_F(DelegateContractIdTest, ConstructWithContractNumTooBig)
{
  // Given / When / Then
  EXPECT_THROW(const DelegateContractId delegateContractId(getTestNumTooBig()), std::invalid_argument);
}

//-----
TEST_F(DelegateContractIdTest, ConstructWithShardRealmContractNum)
{
  // Given / When
  const DelegateContractId delegateContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum());

  // Then
  EXPECT_EQ(delegateContractId.getShardNum(), getTestShardNum());
  EXPECT_EQ(delegateContractId.getRealmNum(), getTestRealmNum());
  ASSERT_TRUE(delegateContractId.getContractNum().has_value());
  EXPECT_EQ(delegateContractId.getContractNum(), getTestContractNum());
  EXPECT_FALSE(delegateContractId.getEvmAddress().has_value());
}

//-----
TEST_F(DelegateContractIdTest, ConstructWithShardRealmContractNumTooBig)
{
  // Given / When / Then
  EXPECT_THROW(const DelegateContractId delegateContractId(getTestNumTooBig(), getTestRealmNum(), getTestContractNum()),
               std::invalid_argument);
  EXPECT_THROW(const DelegateContractId delegateContractId(getTestShardNum(), getTestNumTooBig(), getTestContractNum()),
               std::invalid_argument);
  EXPECT_THROW(const DelegateContractId delegateContractId(getTestShardNum(), getTestRealmNum(), getTestNumTooBig()),
               std::invalid_argument);
}

//-----
TEST_F(DelegateContractIdTest, CompareDelegateContractIds)
{
  // Given / When / Then
  EXPECT_TRUE(DelegateContractId() == DelegateContractId());
  EXPECT_TRUE(DelegateContractId(getTestContractNum()) == DelegateContractId(getTestContractNum()));
  EXPECT_TRUE(DelegateContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum()) ==
              DelegateContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum()));
  EXPECT_FALSE(DelegateContractId(getTestContractNum()) == DelegateContractId(getTestContractNum() - 1ULL));
  EXPECT_FALSE(DelegateContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum()) ==
               DelegateContractId(getTestShardNum() - 1ULL, getTestRealmNum(), getTestContractNum()));
  EXPECT_FALSE(DelegateContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum()) ==
               DelegateContractId(getTestShardNum(), getTestRealmNum() - 1ULL, getTestContractNum()));
}

//-----
TEST_F(DelegateContractIdTest, FromCorrectlyFormedString)
{
  // Given / When
  DelegateContractId delegateContractId;
  EXPECT_NO_THROW(delegateContractId = DelegateContractId::fromString(std::to_string(getTestShardNum()) + '.' +
                                                                      std::to_string(getTestRealmNum()) + '.' +
                                                                      std::to_string(getTestContractNum())));

  // Then
  EXPECT_EQ(delegateContractId.getShardNum(), getTestShardNum());
  EXPECT_EQ(delegateContractId.getRealmNum(), getTestRealmNum());
  ASSERT_TRUE(delegateContractId.getContractNum().has_value());
  EXPECT_EQ(delegateContractId.getContractNum(), getTestContractNum());
}

//-----
TEST_F(DelegateContractIdTest, FromMalformedString)
{
  // Given
  const std::string testShardNumStr = std::to_string(getTestShardNum());
  const std::string testRealmNumStr = std::to_string(getTestRealmNum());
  const std::string testContractNumStr = std::to_string(getTestContractNum());
  const std::string testNumTooBigStr = std::to_string(getTestNumTooBig());
  DelegateContractId delegateContractId;

  // When / Then
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testShardNumStr + testRealmNumStr + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString('.' + testShardNumStr + testRealmNumStr + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testShardNumStr + '.' + testRealmNumStr + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testShardNumStr + testRealmNumStr + '.' + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testShardNumStr + testRealmNumStr + testContractNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(".." + testShardNumStr + testRealmNumStr + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString('.' + testShardNumStr + '.' + testRealmNumStr + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString('.' + testShardNumStr + testRealmNumStr + '.' + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString('.' + testShardNumStr + testRealmNumStr + testContractNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testShardNumStr + ".." + testRealmNumStr + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testShardNumStr + '.' + testRealmNumStr + testContractNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testShardNumStr + testRealmNumStr + ".." + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testShardNumStr + testRealmNumStr + '.' + testContractNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId = DelegateContractId::fromString('.' + testShardNumStr + '.' + testRealmNumStr + '.' +
                                                                   testContractNumStr + '.'),
               std::invalid_argument);

  EXPECT_THROW(delegateContractId = DelegateContractId::fromString("abc"), std::invalid_argument);
  EXPECT_THROW(delegateContractId = DelegateContractId::fromString("o.o.e"), std::invalid_argument);
  EXPECT_THROW(delegateContractId = DelegateContractId::fromString("0.0.1!"), std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testNumTooBigStr + '.' + testRealmNumStr + '.' + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testShardNumStr + '.' + testNumTooBigStr + '.' + testContractNumStr),
               std::invalid_argument);
  EXPECT_THROW(delegateContractId =
                 DelegateContractId::fromString(testShardNumStr + '.' + testRealmNumStr + '.' + testNumTooBigStr),
               std::invalid_argument);
}

//-----
TEST_F(DelegateContractIdTest, FromProtobuf)
{
  // Given
  proto::ContractID protoContractId;
  protoContractId.set_shardnum(static_cast<int64_t>(getTestShardNum()));
  protoContractId.set_realmnum(static_cast<int64_t>(getTestRealmNum()));
  protoContractId.set_contractnum(static_cast<int64_t>(getTestContractNum()));

  // When
  const DelegateContractId delegateContractId = DelegateContractId::fromProtobuf(protoContractId);

  // Then
  EXPECT_EQ(delegateContractId.getShardNum(), getTestShardNum());
  EXPECT_EQ(delegateContractId.getRealmNum(), getTestRealmNum());
  ASSERT_TRUE(delegateContractId.getContractNum().has_value());
  EXPECT_EQ(delegateContractId.getContractNum(), getTestContractNum());
  EXPECT_FALSE(delegateContractId.getEvmAddress().has_value());
}

//-----
TEST_F(DelegateContractIdTest, Clone)
{
  // Given
  const DelegateContractId delegateContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum());

  // When
  const std::unique_ptr<Key> clonedDelegateContractId = delegateContractId.clone();

  // Then
  EXPECT_EQ(clonedDelegateContractId->toBytes(), delegateContractId.toBytes());
}

//-----
TEST_F(DelegateContractIdTest, ToProtobufKey)
{
  // Given
  const DelegateContractId delegateContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum());

  // When
  const std::unique_ptr<proto::Key> protoDelegateContractId = delegateContractId.toProtobufKey();

  // Then
  ASSERT_TRUE(protoDelegateContractId->has_delegatable_contract_id());

  EXPECT_EQ(protoDelegateContractId->delegatable_contract_id().shardnum(), static_cast<int64_t>(getTestShardNum()));
  EXPECT_EQ(protoDelegateContractId->delegatable_contract_id().realmnum(), static_cast<int64_t>(getTestRealmNum()));
  ASSERT_EQ(protoDelegateContractId->delegatable_contract_id().contract_case(),
            proto::ContractID::ContractCase::kContractNum);
  EXPECT_EQ(protoDelegateContractId->delegatable_contract_id().contractnum(),
            static_cast<int64_t>(getTestContractNum()));
}

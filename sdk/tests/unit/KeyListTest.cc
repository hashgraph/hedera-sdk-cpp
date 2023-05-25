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
#include "KeyList.h"
#include "ED25519PrivateKey.h"
#include "PublicKey.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <memory>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class KeyListTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey1() const { return mTestPublicKey1; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey2() const { return mTestPublicKey2; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey3() const { return mTestPublicKey3; }

private:
  const std::shared_ptr<PublicKey> mTestPublicKey1 =
    ED25519PrivateKey::fromString(
      "302e020100300506032b657004220420db484b828e64b2d8f12ce3c0a0e93a0b8cce7af1bb8f39c97732394482538e10")
      ->getPublicKey();
  const std::shared_ptr<PublicKey> mTestPublicKey2 =
    ED25519PrivateKey::fromString(
      "302e020100300506032b657004220420db484b828e64b2d8f12ce3c0a0e93a0b8cce7af1bb8f39c97732394482538e11")
      ->getPublicKey();
  const std::shared_ptr<PublicKey> mTestPublicKey3 =
    ED25519PrivateKey::fromString(
      "302e020100300506032b657004220420db484b828e64b2d8f12ce3c0a0e93a0b8cce7af1bb8f39c97732394482538e12")
      ->getPublicKey();
};

//-----
TEST_F(KeyListTest, FromProtobuf)
{
  // Given
  proto::KeyList protoKeyList;
  protoKeyList.add_keys()->set_ed25519(internal::Utilities::byteVectorToString(getTestPublicKey1()->toBytesDer()));
  protoKeyList.add_keys()->set_ed25519(internal::Utilities::byteVectorToString(getTestPublicKey2()->toBytesDer()));
  protoKeyList.add_keys()->set_ed25519(internal::Utilities::byteVectorToString(getTestPublicKey3()->toBytesDer()));

  // When
  KeyList keyList;
  EXPECT_NO_THROW(keyList = KeyList::fromProtobuf(protoKeyList));

  // Then
  EXPECT_TRUE(keyList.contains(getTestPublicKey1().get()));
  EXPECT_TRUE(keyList.contains(getTestPublicKey2().get()));
  EXPECT_TRUE(keyList.contains(getTestPublicKey3().get()));
}

//-----
TEST_F(KeyListTest, Of)
{
  // Given / When
  const KeyList keyList =
    KeyList::of({ getTestPublicKey1().get(), getTestPublicKey2().get(), getTestPublicKey3().get() });

  // Then
  EXPECT_TRUE(keyList.contains(getTestPublicKey1().get()));
  EXPECT_TRUE(keyList.contains(getTestPublicKey2().get()));
  EXPECT_TRUE(keyList.contains(getTestPublicKey3().get()));
}

//-----
TEST_F(KeyListTest, ToProtobufKey)
{
  // Given
  const KeyList keyList =
    KeyList::of({ getTestPublicKey1().get(), getTestPublicKey2().get(), getTestPublicKey3().get() });

  // When
  std::unique_ptr<proto::Key> protoKey;
  EXPECT_NO_THROW(protoKey = keyList.toProtobufKey());

  // Then
  ASSERT_TRUE(protoKey->has_keylist());
  ASSERT_EQ(protoKey->keylist().keys_size(), 3);
  EXPECT_EQ(protoKey->keylist().keys(0).ed25519(),
            internal::Utilities::byteVectorToString(getTestPublicKey1()->toBytesRaw()));
  EXPECT_EQ(protoKey->keylist().keys(1).ed25519(),
            internal::Utilities::byteVectorToString(getTestPublicKey2()->toBytesRaw()));
  EXPECT_EQ(protoKey->keylist().keys(2).ed25519(),
            internal::Utilities::byteVectorToString(getTestPublicKey3()->toBytesRaw()));
}

//-----
TEST_F(KeyListTest, ToProtobuf)
{
  // Given
  const KeyList keyList =
    KeyList::of({ getTestPublicKey1().get(), getTestPublicKey2().get(), getTestPublicKey3().get() });

  // When
  std::unique_ptr<proto::KeyList> protoKeyList;
  EXPECT_NO_THROW(protoKeyList = keyList.toProtobuf());

  // Then
  ASSERT_EQ(protoKeyList->keys_size(), 3);
  EXPECT_EQ(protoKeyList->keys(0).ed25519(),
            internal::Utilities::byteVectorToString(getTestPublicKey1()->toBytesRaw()));
  EXPECT_EQ(protoKeyList->keys(1).ed25519(),
            internal::Utilities::byteVectorToString(getTestPublicKey2()->toBytesRaw()));
  EXPECT_EQ(protoKeyList->keys(2).ed25519(),
            internal::Utilities::byteVectorToString(getTestPublicKey3()->toBytesRaw()));
}

//-----
TEST_F(KeyListTest, Size)
{
  // Given / When
  const KeyList keyList =
    KeyList::of({ getTestPublicKey1().get(), getTestPublicKey2().get(), getTestPublicKey3().get() });
  const KeyList emptyKeyList;

  // Then
  EXPECT_EQ(keyList.size(), 3);
  EXPECT_EQ(emptyKeyList.size(), 0);
}

//-----
TEST_F(KeyListTest, Empty)
{
  // Given / When
  const KeyList keyList =
    KeyList::of({ getTestPublicKey1().get(), getTestPublicKey2().get(), getTestPublicKey3().get() });
  const KeyList emptyKeyList;

  // Then
  EXPECT_FALSE(keyList.empty());
  EXPECT_TRUE(emptyKeyList.empty());
}

//-----
TEST_F(KeyListTest, Contains)
{
  // Given / When
  const KeyList keyList =
    KeyList::of({ getTestPublicKey1().get(), getTestPublicKey2().get(), getTestPublicKey3().get() });
  const KeyList emptyKeyList;

  // Then
  EXPECT_TRUE(keyList.contains(getTestPublicKey1().get()));
  EXPECT_TRUE(keyList.contains(getTestPublicKey2().get()));
  EXPECT_TRUE(keyList.contains(getTestPublicKey3().get()));

  EXPECT_FALSE(emptyKeyList.contains(getTestPublicKey1().get()));
  EXPECT_FALSE(emptyKeyList.contains(getTestPublicKey2().get()));
  EXPECT_FALSE(emptyKeyList.contains(getTestPublicKey3().get()));
}

//-----
TEST_F(KeyListTest, PushBack)
{
  // Given
  KeyList keyList = KeyList::of({ getTestPublicKey1().get(), getTestPublicKey2().get() });

  // When
  keyList.push_back(getTestPublicKey3().get());

  // Then
  EXPECT_EQ(keyList.size(), 3);
  EXPECT_TRUE(keyList.contains(getTestPublicKey3().get()));
}

//-----
TEST_F(KeyListTest, Remove)
{
  // Given
  KeyList keyList = KeyList::of({ getTestPublicKey1().get(), getTestPublicKey2().get(), getTestPublicKey3().get() });

  // When
  keyList.remove(getTestPublicKey1().get());

  // Then
  EXPECT_EQ(keyList.size(), 2);
  EXPECT_FALSE(keyList.contains(getTestPublicKey1().get()));
  EXPECT_TRUE(keyList.contains(getTestPublicKey2().get()));
  EXPECT_TRUE(keyList.contains(getTestPublicKey3().get()));
}

//-----
TEST_F(KeyListTest, Clear)
{
  // Given
  KeyList keyList = KeyList::of({ getTestPublicKey1().get(), getTestPublicKey2().get(), getTestPublicKey3().get() });

  // When
  keyList.clear();

  // Then
  EXPECT_TRUE(keyList.empty());
}

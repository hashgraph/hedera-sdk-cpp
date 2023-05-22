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
#include "Key.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class KeyTest : public ::testing::Test
{
};

//-----
/*TEST_F(KeyTest, PublicKeyFromProtobuf)
{
  // Given
  const std::unique_ptr<proto::Key> protobufKey = Key::fromBytes(getTestPublicKeyBytes())->toProtobuf();

  // When
  const std::shared_ptr<Key> publicKey = Key::fromProtobuf(*protobufKey);

  // Then
  ASSERT_NE(publicKey, nullptr);
  EXPECT_EQ(publicKey->toBytes(), getTestPublicKeyBytes());
}*/

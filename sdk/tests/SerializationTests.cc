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
#include "ECDSAPrivateKey.h"
#include "ECDSAPublicKey.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class SerializationTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::unique_ptr<ECDSAPrivateKey>& getTestECDSAPrivateKey() const { return mPrivateKey; }
  [[nodiscard]] inline const std::shared_ptr<ECDSAPublicKey>& getTestECDSAPublicKey() const { return mPublicKeyFromString; }

private:
  const std::unique_ptr<ECDSAPrivateKey> mPrivateKey = ECDSAPrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> mPublicKeyFromPrivate = mPrivateKey->getPublicKey();
  const std::shared_ptr<ECDSAPublicKey> mPublicKeyFromString = ECDSAPublicKey::fromString(mPublicKeyFromPrivate->toString());
};

TEST_F(SerializationTests, ECDSAPrivateKeyToProtobufTest)
{
  // Given
  const std::shared_ptr<ECDSAPublicKey> testECDSAPublicKey = getTestECDSAPublicKey();

  // // When
  const std::unique_ptr<proto::Key> protobufECDSAPublicKey = testECDSAPublicKey.get()->toProtobuf();
  
  // Then
  EXPECT_NE(protobufECDSAPublicKey.get(), nullptr);
  EXPECT_TRUE(protobufECDSAPublicKey.get()->has_ecdsa_secp256k1());
}
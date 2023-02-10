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
#include "ED25519PrivateKey.h"
#include "ED25519PublicKey.h"
#include "exceptions/BadKeyException.h"
#include "exceptions/UninitializedException.h"
#include "impl/DerivationPathUtils.h"
#include "impl/HexConverter.h"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace Hedera;

class ED25519PrivateKeyTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::unique_ptr<ED25519PrivateKey>& getTestPrivateKeyGenerated() const
  {
    return mPrivateKeyGenerated;
  }

  [[nodiscard]] inline const std::unique_ptr<ED25519PrivateKey>& getTestPrivateKeyLoaded() const
  {
    return mPrivateKeyLoaded;
  }

private:
  const std::unique_ptr<ED25519PrivateKey> mPrivateKeyGenerated = ED25519PrivateKey::generatePrivateKey();
  const std::unique_ptr<ED25519PrivateKey> mPrivateKeyLoaded =
    ED25519PrivateKey::fromString(mPrivateKeyGenerated->toString());
};

TEST_F(ED25519PrivateKeyTest, CopyAndMoveConstructors)
{
  ED25519PrivateKey copiedPrivateKey(*getTestPrivateKeyGenerated());
  EXPECT_EQ(copiedPrivateKey.toString(), getTestPrivateKeyGenerated()->toString());

  copiedPrivateKey = *getTestPrivateKeyLoaded();
  EXPECT_EQ(copiedPrivateKey.toString(), getTestPrivateKeyLoaded()->toString());

  ED25519PrivateKey movedPrivateKey(std::move(copiedPrivateKey));
  EXPECT_EQ(movedPrivateKey.toString(), getTestPrivateKeyLoaded()->toString());

  copiedPrivateKey = std::move(movedPrivateKey);
  EXPECT_EQ(copiedPrivateKey.toString(), getTestPrivateKeyLoaded()->toString());
}

TEST_F(ED25519PrivateKeyTest, GetPublicKey)
{
  // get the public keys from the private keys
  const std::shared_ptr<PublicKey> publicFromGenerated = getTestPrivateKeyGenerated()->getPublicKey();
  const std::shared_ptr<PublicKey> publicFromLoaded = getTestPrivateKeyLoaded()->getPublicKey();

  EXPECT_NE(publicFromGenerated, nullptr);
  EXPECT_NE(publicFromLoaded, nullptr);
  EXPECT_EQ(publicFromGenerated->toString(), publicFromLoaded->toString());
}

TEST_F(ED25519PrivateKeyTest, Sign)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> signatureFromGenerated = getTestPrivateKeyGenerated()->sign(bytesToSign);
  const std::vector<unsigned char> signatureFromLoaded = getTestPrivateKeyLoaded()->sign(bytesToSign);

  EXPECT_EQ(signatureFromGenerated.size(), 64);
  EXPECT_EQ(signatureFromLoaded.size(), 64);
  EXPECT_EQ(signatureFromGenerated, signatureFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, SignEmptyBytes)
{
  const std::vector<unsigned char> bytesToSign;
  const std::vector<unsigned char> signatureFromGenerated = getTestPrivateKeyGenerated()->sign(bytesToSign);
  const std::vector<unsigned char> signatureFromLoaded = getTestPrivateKeyLoaded()->sign(bytesToSign);

  EXPECT_EQ(signatureFromGenerated.size(), 64);
  EXPECT_EQ(signatureFromLoaded.size(), 64);
  EXPECT_EQ(signatureFromGenerated, signatureFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, ToString)
{
  const std::string stringFromGenerated = getTestPrivateKeyGenerated()->toString();
  const std::string stringFromLoaded = getTestPrivateKeyLoaded()->toString();

  EXPECT_EQ(stringFromGenerated.size(), 64);
  EXPECT_EQ(stringFromLoaded.size(), 64);
  EXPECT_EQ(stringFromGenerated, stringFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, FromString)
{
  // These are 2 versions of the same private key. The first conforms to the full RFC 8410 standard, the second is just
  // the private key.
  const std::string privateKeyStringExtended =
    "302E020100300506032B65700422042068FBA516472B387C9F33C3E667616D806E5B9CEFF23A766E5D9A3818C77871F1";
  const std::string privateKeyStringShort = "68FBA516472B387C9F33C3E667616D806E5B9CEFF23A766E5D9A3818C77871F1";

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromExtended =
    ED25519PrivateKey::fromString(privateKeyStringExtended);
  const std::unique_ptr<ED25519PrivateKey> privateKeyFromShort = ED25519PrivateKey::fromString(privateKeyStringShort);

  EXPECT_NE(privateKeyFromExtended, nullptr);
  EXPECT_NE(privateKeyFromShort, nullptr);
  EXPECT_EQ(privateKeyFromExtended->toString(), privateKeyFromShort->toString());

  // Throw on garbage data
  EXPECT_THROW(ED25519PrivateKey::fromString("asdfdsafds"), BadKeyException);
}

//-----
TEST_F(ED25519PrivateKeyTest, Derive)
{
  std::unique_ptr<ED25519PrivateKey> privateKey =
    ED25519PrivateKey::fromSeed(internal::HexConverter::hexToBase64("000102030405060708090a0b0c0d0e0f"));

  // Throw when not initialized with a chain code
  EXPECT_THROW(privateKey = getTestPrivateKeyLoaded()->derive(0), UninitializedException);
  EXPECT_THROW(privateKey = getTestPrivateKeyGenerated()->derive(0), UninitializedException);

  // Throw if input index is hardened
  EXPECT_THROW(privateKey = privateKey->derive(1 | 0x80000000), std::invalid_argument);
}

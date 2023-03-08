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
#include "impl/Utilities.h"

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
    ED25519PrivateKey::fromString(mPrivateKeyGenerated->toStringDer());
};

//-----
TEST_F(ED25519PrivateKeyTest, FromString)
{
  const std::string privateKeyString = "68FBA516472B387C9F33C3E667616D806E5B9CEFF23A766E5D9A3818C77871F1";
  const std::string privateKeyStringDer = ED25519PrivateKey::DER_ENCODED_PREFIX_HEX + privateKeyString;

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromString = ED25519PrivateKey::fromString(privateKeyString);

  ASSERT_NE(privateKeyFromString, nullptr);
  EXPECT_EQ(privateKeyFromString->toStringDer(), privateKeyStringDer);
  EXPECT_EQ(privateKeyFromString->toStringRaw(), privateKeyString);
  EXPECT_EQ(privateKeyFromString->toBytesDer(), internal::HexConverter::hexToBytes(privateKeyStringDer));
  EXPECT_EQ(privateKeyFromString->toBytesRaw(), internal::HexConverter::hexToBytes(privateKeyString));

  EXPECT_THROW(const std::unique_ptr<ED25519PrivateKey> privateKeyFromStringDer =
                 ED25519PrivateKey::fromStringDer(privateKeyString),
               BadKeyException);

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromStringDer =
    ED25519PrivateKey::fromStringDer(privateKeyStringDer);

  ASSERT_NE(privateKeyFromStringDer, nullptr);
  EXPECT_EQ(privateKeyFromStringDer->toStringDer(), privateKeyFromString->toStringDer());
  EXPECT_EQ(privateKeyFromStringDer->toStringRaw(), privateKeyFromString->toStringRaw());
  EXPECT_EQ(privateKeyFromStringDer->toBytesDer(), privateKeyFromString->toBytesDer());
  EXPECT_EQ(privateKeyFromStringDer->toBytesRaw(), privateKeyFromString->toBytesRaw());

  EXPECT_THROW(const std::unique_ptr<ED25519PrivateKey> privateKeyFromStringRaw =
                 ED25519PrivateKey::fromStringRaw(privateKeyStringDer),
               BadKeyException);

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromStringRaw = ED25519PrivateKey::fromStringRaw(privateKeyString);

  ASSERT_NE(privateKeyFromStringRaw, nullptr);
  EXPECT_EQ(privateKeyFromStringRaw->toStringDer(), privateKeyFromStringDer->toStringDer());
  EXPECT_EQ(privateKeyFromStringRaw->toStringRaw(), privateKeyFromStringDer->toStringRaw());
  EXPECT_EQ(privateKeyFromStringRaw->toBytesDer(), privateKeyFromStringDer->toBytesDer());
  EXPECT_EQ(privateKeyFromStringRaw->toBytesRaw(), privateKeyFromStringDer->toBytesRaw());

  // Throw if input garbage
  EXPECT_THROW(const std::unique_ptr<ED25519PrivateKey> key = ED25519PrivateKey::fromString("fdsakfdsalf"),
               BadKeyException);
  EXPECT_THROW(const std::unique_ptr<ED25519PrivateKey> key =
                 ED25519PrivateKey::fromString(ED25519PrivateKey::DER_ENCODED_PREFIX_HEX + "fjdskaf;"),
               BadKeyException);
  EXPECT_THROW(const std::unique_ptr<ED25519PrivateKey> key =
                 ED25519PrivateKey::fromString(ED25519PrivateKey::DER_ENCODED_PREFIX_HEX +
                                               "F83DEF42411E046461D5AEEAE9S11C56F661 557F349F3412DBD95C9FE8B026X"),
               BadKeyException);
}

//-----
TEST_F(ED25519PrivateKeyTest, FromBytes)
{
  const std::vector<unsigned char> privateKeyBytes = { 0x68, 0xFB, 0xA5, 0x16, 0x47, 0x2B, 0x38, 0x7C, 0x9F, 0x33, 0xC3,
                                                       0xE6, 0x67, 0x61, 0x6D, 0x80, 0x6E, 0x5B, 0x9C, 0xEF, 0xF2, 0x3A,
                                                       0x76, 0x6E, 0x5D, 0x9A, 0x38, 0x18, 0xC7, 0x78, 0x71, 0xF1 };
  const std::vector<unsigned char> privateKeyBytesDer =
    internal::Utilities::concatenateVectors(ED25519PrivateKey::DER_ENCODED_PREFIX_BYTES, privateKeyBytes);

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromBytes = ED25519PrivateKey::fromBytes(privateKeyBytes);

  ASSERT_NE(privateKeyFromBytes, nullptr);
  EXPECT_EQ(privateKeyFromBytes->toStringDer(), internal::HexConverter::bytesToHex(privateKeyBytesDer));
  EXPECT_EQ(privateKeyFromBytes->toStringRaw(), internal::HexConverter::bytesToHex(privateKeyBytes));
  EXPECT_EQ(privateKeyFromBytes->toBytesDer(), privateKeyBytesDer);
  EXPECT_EQ(privateKeyFromBytes->toBytesRaw(), privateKeyBytes);

  EXPECT_THROW(const std::unique_ptr<ED25519PrivateKey> privateKeyFromBytesDer =
                 ED25519PrivateKey::fromBytesDer(privateKeyBytes),
               BadKeyException);

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromBytesDer = ED25519PrivateKey::fromBytesDer(privateKeyBytesDer);

  ASSERT_NE(privateKeyFromBytesDer, nullptr);
  EXPECT_EQ(privateKeyFromBytesDer->toStringDer(), privateKeyFromBytes->toStringDer());
  EXPECT_EQ(privateKeyFromBytesDer->toStringRaw(), privateKeyFromBytes->toStringRaw());
  EXPECT_EQ(privateKeyFromBytesDer->toBytesDer(), privateKeyFromBytes->toBytesDer());
  EXPECT_EQ(privateKeyFromBytesDer->toBytesRaw(), privateKeyFromBytes->toBytesRaw());

  EXPECT_THROW(const std::unique_ptr<ED25519PrivateKey> privateKeyFromBytesRaw =
                 ED25519PrivateKey::fromBytesRaw(privateKeyBytesDer),
               BadKeyException);

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromBytesRaw = ED25519PrivateKey::fromBytesRaw(privateKeyBytes);

  ASSERT_NE(privateKeyFromBytesRaw, nullptr);
  EXPECT_EQ(privateKeyFromBytesRaw->toStringDer(), privateKeyFromBytesDer->toStringDer());
  EXPECT_EQ(privateKeyFromBytesRaw->toStringRaw(), privateKeyFromBytesDer->toStringRaw());
  EXPECT_EQ(privateKeyFromBytesRaw->toBytesDer(), privateKeyFromBytesDer->toBytesDer());
  EXPECT_EQ(privateKeyFromBytesRaw->toBytesRaw(), privateKeyFromBytesDer->toBytesRaw());
}

TEST_F(ED25519PrivateKeyTest, GetPublicKey)
{
  // get the public keys from the private keys
  const std::shared_ptr<PublicKey> publicFromGenerated = getTestPrivateKeyGenerated()->getPublicKey();
  const std::shared_ptr<PublicKey> publicFromLoaded = getTestPrivateKeyLoaded()->getPublicKey();

  EXPECT_NE(publicFromGenerated, nullptr);
  EXPECT_NE(publicFromLoaded, nullptr);
  EXPECT_EQ(publicFromGenerated->toStringDer(), publicFromLoaded->toStringDer());
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
  const std::string stringFromGenerated = getTestPrivateKeyGenerated()->toStringRaw();
  const std::string stringFromLoaded = getTestPrivateKeyLoaded()->toStringRaw();

  EXPECT_EQ(stringFromGenerated.size(), 64);
  EXPECT_EQ(stringFromLoaded.size(), 64);
  EXPECT_EQ(stringFromGenerated, stringFromLoaded);
}

//-----
TEST_F(ED25519PrivateKeyTest, Derive)
{
  std::unique_ptr<PrivateKey> privateKey =
    ED25519PrivateKey::fromSeed(internal::HexConverter::hexToBytes("000102030405060708090a0b0c0d0e0f"));

  // Throw when not initialized with a chain code
  EXPECT_THROW(privateKey = getTestPrivateKeyLoaded()->derive(0), UninitializedException);
  EXPECT_THROW(privateKey = getTestPrivateKeyGenerated()->derive(0), UninitializedException);

  // Throw if input index is hardened
  EXPECT_THROW(privateKey = privateKey->derive(1 | 0x80000000), std::invalid_argument);
}

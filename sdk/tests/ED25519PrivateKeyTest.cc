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
#include "ECDSAsecp256k1PrivateKey.h"
#include "ED25519PublicKey.h"
#include "exceptions/BadKeyException.h"
#include "exceptions/UninitializedException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace Hedera;
using namespace Hedera::internal::Utilities;

class ED25519PrivateKeyTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::string& getTestPrivateKeyHexString() const { return mPrivateKeyHexString; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestPrivateKeyBytes() const { return mPrivateKeyBytes; }

private:
  const std::string mPrivateKeyHexString = "68FBA516472B387C9F33C3E667616D806E5B9CEFF23A766E5D9A3818C77871F1";
  const std::vector<std::byte> mPrivateKeyBytes = {
    std::byte(0x68), std::byte(0xFB), std::byte(0xA5), std::byte(0x16), std::byte(0x47), std::byte(0x2B),
    std::byte(0x38), std::byte(0x7C), std::byte(0x9F), std::byte(0x33), std::byte(0xC3), std::byte(0xE6),
    std::byte(0x67), std::byte(0x61), std::byte(0x6D), std::byte(0x80), std::byte(0x6E), std::byte(0x5B),
    std::byte(0x9C), std::byte(0xEF), std::byte(0xF2), std::byte(0x3A), std::byte(0x76), std::byte(0x6E),
    std::byte(0x5D), std::byte(0x9A), std::byte(0x38), std::byte(0x18), std::byte(0xC7), std::byte(0x78),
    std::byte(0x71), std::byte(0xF1)
  };
};

//-----
TEST_F(ED25519PrivateKeyTest, GeneratePrivateKey)
{
  // When
  const std::unique_ptr<ED25519PrivateKey> privateKeyGenerated = ED25519PrivateKey::generatePrivateKey();

  // Then
  ASSERT_NE(privateKeyGenerated, nullptr);
  EXPECT_EQ(privateKeyGenerated->toStringRaw().size(), ED25519PrivateKey::KEY_SIZE * 2);
  EXPECT_EQ(privateKeyGenerated->toStringDer().size(),
            ED25519PrivateKey::KEY_SIZE * 2 + ED25519PrivateKey::DER_ENCODED_PREFIX_HEX.size());
  EXPECT_EQ(privateKeyGenerated->toBytesRaw().size(), ED25519PrivateKey::KEY_SIZE);
  EXPECT_EQ(privateKeyGenerated->toBytesDer().size(),
            ED25519PrivateKey::KEY_SIZE + ED25519PrivateKey::DER_ENCODED_PREFIX_BYTES.size());
}

//-----
TEST_F(ED25519PrivateKeyTest, FromString)
{
  const std::string derEncodedPrivateKeyHexString =
    ED25519PrivateKey::DER_ENCODED_PREFIX_HEX + getTestPrivateKeyHexString();

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromString =
    ED25519PrivateKey::fromString(getTestPrivateKeyHexString());

  ASSERT_NE(privateKeyFromString, nullptr);
  EXPECT_EQ(privateKeyFromString->toStringDer(), derEncodedPrivateKeyHexString);
  EXPECT_EQ(privateKeyFromString->toStringRaw(), getTestPrivateKeyHexString());
  EXPECT_EQ(privateKeyFromString->toBytesDer(),
            concatenateVectors({ ED25519PrivateKey::DER_ENCODED_PREFIX_BYTES, getTestPrivateKeyBytes() }));
  EXPECT_EQ(privateKeyFromString->toBytesRaw(), getTestPrivateKeyBytes());

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromStringDer =
    ED25519PrivateKey::fromString(derEncodedPrivateKeyHexString);

  ASSERT_NE(privateKeyFromStringDer, nullptr);
  EXPECT_EQ(privateKeyFromStringDer->toStringDer(), privateKeyFromString->toStringDer());
  EXPECT_EQ(privateKeyFromStringDer->toStringRaw(), privateKeyFromString->toStringRaw());
  EXPECT_EQ(privateKeyFromStringDer->toBytesDer(), privateKeyFromString->toBytesDer());
  EXPECT_EQ(privateKeyFromStringDer->toBytesRaw(), privateKeyFromString->toBytesRaw());

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
  EXPECT_THROW(const std::unique_ptr<ED25519PrivateKey> key = ED25519PrivateKey::fromString(
                 ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX + getTestPrivateKeyHexString()),
               BadKeyException);
  EXPECT_THROW(const std::unique_ptr<ED25519PrivateKey> key = ED25519PrivateKey::fromString(
                 std::string(ED25519PrivateKey::DER_ENCODED_PREFIX_HEX.size(), 'A') + getTestPrivateKeyHexString()),
               BadKeyException);
}

//-----
TEST_F(ED25519PrivateKeyTest, FromBytes)
{
  const std::vector<std::byte> derEncodedPrivateKeyBytes =
    concatenateVectors({ ED25519PrivateKey::DER_ENCODED_PREFIX_BYTES, getTestPrivateKeyBytes() });

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromBytes = ED25519PrivateKey::fromBytes(getTestPrivateKeyBytes());

  ASSERT_NE(privateKeyFromBytes, nullptr);
  EXPECT_EQ(privateKeyFromBytes->toStringDer(),
            ED25519PrivateKey::DER_ENCODED_PREFIX_HEX + getTestPrivateKeyHexString());
  EXPECT_EQ(privateKeyFromBytes->toStringRaw(), getTestPrivateKeyHexString());
  EXPECT_EQ(privateKeyFromBytes->toBytesDer(), derEncodedPrivateKeyBytes);
  EXPECT_EQ(privateKeyFromBytes->toBytesRaw(), getTestPrivateKeyBytes());

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromBytesDer =
    ED25519PrivateKey::fromBytes(derEncodedPrivateKeyBytes);

  ASSERT_NE(privateKeyFromBytesDer, nullptr);
  EXPECT_EQ(privateKeyFromBytesDer->toStringDer(), privateKeyFromBytes->toStringDer());
  EXPECT_EQ(privateKeyFromBytesDer->toStringRaw(), privateKeyFromBytes->toStringRaw());
  EXPECT_EQ(privateKeyFromBytesDer->toBytesDer(), privateKeyFromBytes->toBytesDer());
  EXPECT_EQ(privateKeyFromBytesDer->toBytesRaw(), privateKeyFromBytes->toBytesRaw());
}

//-----
TEST_F(ED25519PrivateKeyTest, Clone)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> privateKey = ED25519PrivateKey::fromString(getTestPrivateKeyHexString());

  // When
  const std::unique_ptr<PrivateKey> clonedPrivateKey = privateKey->clone();

  // Then
  ASSERT_NE(clonedPrivateKey, nullptr);
  EXPECT_EQ(privateKey->toStringRaw(), getTestPrivateKeyHexString());
}

//-----
TEST_F(ED25519PrivateKeyTest, Derive)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> privateKey = ED25519PrivateKey::fromString(getTestPrivateKeyHexString());

  // When / Then
  // The privateKey was not initialized with a chain code, so derivation shouldn't be supported.
  EXPECT_THROW(auto key = privateKey->derive(0), UninitializedException);

  // Derivation functionality is further tested in SLIP10 test vectors
}

//-----
TEST_F(ED25519PrivateKeyTest, Sign)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> privateKey = ED25519PrivateKey::fromString(getTestPrivateKeyHexString());
  const std::vector<std::byte> bytesToSign = { std::byte(0x1), std::byte(0x2), std::byte(0x3) };

  // When / Then
  EXPECT_NO_THROW(const std::vector<std::byte> signature = privateKey->sign(bytesToSign));

  // Signature functionality is further tested in RFC8032 test vectors
}

//-----
TEST_F(ED25519PrivateKeyTest, SignEmptyBytes)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> privateKey = ED25519PrivateKey::fromString(getTestPrivateKeyHexString());

  // When / Then
  EXPECT_NO_THROW(const std::vector<std::byte> signature = privateKey->sign({}));

  // Signature functionality is further tested in RFC8032 test vectors
}

//-----
TEST_F(ED25519PrivateKeyTest, ToString)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> privateKey = ED25519PrivateKey::fromString(getTestPrivateKeyHexString());

  // When
  const std::string stringDer = privateKey->toStringDer();
  const std::string stringRaw = privateKey->toStringRaw();

  // Then
  EXPECT_EQ(stringDer, ED25519PrivateKey::DER_ENCODED_PREFIX_HEX + getTestPrivateKeyHexString());
  EXPECT_EQ(stringRaw, getTestPrivateKeyHexString());
}

//-----
TEST_F(ED25519PrivateKeyTest, ToBytes)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> privateKey = ED25519PrivateKey::fromString(getTestPrivateKeyHexString());

  // When
  const std::vector<std::byte> bytesDer = privateKey->toBytesDer();
  const std::vector<std::byte> bytesRaw = privateKey->toBytesRaw();

  // Then
  EXPECT_EQ(bytesDer, concatenateVectors({ ED25519PrivateKey::DER_ENCODED_PREFIX_BYTES, getTestPrivateKeyBytes() }));
  EXPECT_EQ(bytesRaw, getTestPrivateKeyBytes());
}

//-----
TEST_F(ED25519PrivateKeyTest, GetChainCode)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> privateKey = ED25519PrivateKey::fromString(getTestPrivateKeyHexString());

  // When
  const std::vector<std::byte> chainCode = privateKey->getChainCode();

  // Then
  EXPECT_TRUE(chainCode.empty());

  // Chain code functionality is further tested in SLIP10 test vectors
}

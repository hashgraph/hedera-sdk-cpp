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
#include "ECDSAsecp256k1PrivateKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PrivateKey.h"
#include "exceptions/BadKeyException.h"
#include "exceptions/UninitializedException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

using namespace Hedera;
using namespace Hedera::internal::Utilities;

class ECDSAsecp256k1PrivateKeyTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::string& getTestPrivateKeyHexString() const { return mPrivateKeyHexString; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestPrivateKeyBytes() const { return mPrivateKeyBytes; }

private:
  const std::string mPrivateKeyHexString = "E8F32E723DECF4051AEFAC8E2C93C9C5B214313817CDB01A1494B917C8436B35";
  const std::vector<std::byte> mPrivateKeyBytes = {
    std::byte(0xE8), std::byte(0xF3), std::byte(0x2E), std::byte(0x72), std::byte(0x3D), std::byte(0xEC),
    std::byte(0xF4), std::byte(0x05), std::byte(0x1A), std::byte(0xEF), std::byte(0xAC), std::byte(0x8E),
    std::byte(0x2C), std::byte(0x93), std::byte(0xC9), std::byte(0xC5), std::byte(0xB2), std::byte(0x14),
    std::byte(0x31), std::byte(0x38), std::byte(0x17), std::byte(0xCD), std::byte(0xB0), std::byte(0x1A),
    std::byte(0x14), std::byte(0x94), std::byte(0xB9), std::byte(0x17), std::byte(0xC8), std::byte(0x43),
    std::byte(0x6B), std::byte(0x35)
  };
};

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, GeneratePrivateKey)
{
  // Given / When
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyGenerated = ECDSAsecp256k1PrivateKey::generatePrivateKey();

  // Then
  ASSERT_NE(privateKeyGenerated, nullptr);
  EXPECT_EQ(privateKeyGenerated->toStringRaw().size(), ECDSAsecp256k1PrivateKey::KEY_SIZE * 2);
  EXPECT_EQ(privateKeyGenerated->toStringDer().size(),
            ECDSAsecp256k1PrivateKey::KEY_SIZE * 2 + ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX.size());
  EXPECT_EQ(privateKeyGenerated->toBytesRaw().size(), ECDSAsecp256k1PrivateKey::KEY_SIZE);
  EXPECT_EQ(privateKeyGenerated->toBytesDer().size(),
            ECDSAsecp256k1PrivateKey::KEY_SIZE + ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_BYTES.size());
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, FromString)
{
  const std::string derEncodedPrivateKeyHexString =
    ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX + getTestPrivateKeyHexString();

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromString =
    ECDSAsecp256k1PrivateKey::fromString(getTestPrivateKeyHexString());

  ASSERT_NE(privateKeyFromString, nullptr);
  EXPECT_EQ(privateKeyFromString->toStringDer(), derEncodedPrivateKeyHexString);
  EXPECT_EQ(privateKeyFromString->toStringRaw(), getTestPrivateKeyHexString());
  EXPECT_EQ(privateKeyFromString->toBytesDer(),
            concatenateVectors({ ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_BYTES, getTestPrivateKeyBytes() }));
  EXPECT_EQ(privateKeyFromString->toBytesRaw(), getTestPrivateKeyBytes());

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromStringDer =
    ECDSAsecp256k1PrivateKey::fromString(derEncodedPrivateKeyHexString);

  ASSERT_NE(privateKeyFromStringDer, nullptr);
  EXPECT_EQ(privateKeyFromStringDer->toStringDer(), privateKeyFromString->toStringDer());
  EXPECT_EQ(privateKeyFromStringDer->toStringRaw(), privateKeyFromString->toStringRaw());
  EXPECT_EQ(privateKeyFromStringDer->toBytesDer(), privateKeyFromString->toBytesDer());
  EXPECT_EQ(privateKeyFromStringDer->toBytesRaw(), privateKeyFromString->toBytesRaw());

  // Throw if input garbage
  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> key =
                 ECDSAsecp256k1PrivateKey::fromString("fdsakfdsalf"),
               BadKeyException);
  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> key =
                 ECDSAsecp256k1PrivateKey::fromString(ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX + "fjdskaf;"),
               BadKeyException);
  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> key = ECDSAsecp256k1PrivateKey::fromString(
                 ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX +
                 "F83DEF42411E046461D5AEEAE9S11C56F661 557F349F3412DBD95C9FE8B026X"),
               BadKeyException);
  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> key = ECDSAsecp256k1PrivateKey::fromString(
                 ED25519PrivateKey::DER_ENCODED_PREFIX_HEX + getTestPrivateKeyHexString()),
               BadKeyException);
  EXPECT_THROW(
    const std::unique_ptr<ECDSAsecp256k1PrivateKey> key = ECDSAsecp256k1PrivateKey::fromString(
      std::string(ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX.size(), 'A') + getTestPrivateKeyHexString()),
    BadKeyException);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, FromBytes)
{
  const std::vector<std::byte> derEncodedPrivateKeyBytes =
    concatenateVectors({ ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_BYTES, getTestPrivateKeyBytes() });

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromBytes =
    ECDSAsecp256k1PrivateKey::fromBytes(getTestPrivateKeyBytes());

  ASSERT_NE(privateKeyFromBytes, nullptr);
  EXPECT_EQ(privateKeyFromBytes->toStringDer(),
            ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX + getTestPrivateKeyHexString());
  EXPECT_EQ(privateKeyFromBytes->toStringRaw(), getTestPrivateKeyHexString());
  EXPECT_EQ(privateKeyFromBytes->toBytesDer(), derEncodedPrivateKeyBytes);
  EXPECT_EQ(privateKeyFromBytes->toBytesRaw(), getTestPrivateKeyBytes());

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromBytesDer =
    ECDSAsecp256k1PrivateKey::fromBytes(derEncodedPrivateKeyBytes);

  ASSERT_NE(privateKeyFromBytesDer, nullptr);
  EXPECT_EQ(privateKeyFromBytesDer->toStringDer(), privateKeyFromBytes->toStringDer());
  EXPECT_EQ(privateKeyFromBytesDer->toStringRaw(), privateKeyFromBytes->toStringRaw());
  EXPECT_EQ(privateKeyFromBytesDer->toBytesDer(), privateKeyFromBytes->toBytesDer());
  EXPECT_EQ(privateKeyFromBytesDer->toBytesRaw(), privateKeyFromBytes->toBytesRaw());
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, Clone)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey =
    ECDSAsecp256k1PrivateKey::fromString(getTestPrivateKeyHexString());

  // When
  const std::unique_ptr<PrivateKey> clonedPrivateKey = privateKey->clone();

  // Then
  ASSERT_NE(clonedPrivateKey, nullptr);
  EXPECT_EQ(privateKey->toStringRaw(), getTestPrivateKeyHexString());
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, Derive)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey =
    ECDSAsecp256k1PrivateKey::fromString(getTestPrivateKeyHexString());

  // When / Then
  // The privateKey was not initialized with a chain code, so derivation shouldn't be supported.
  EXPECT_THROW(auto key = privateKey->derive(0), UninitializedException);

  // Derivation functionality is further tested in SLIP10 test vectors
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, Sign)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey =
    ECDSAsecp256k1PrivateKey::fromString(getTestPrivateKeyHexString());
  const std::vector<std::byte> bytesToSign = { std::byte(0x1), std::byte(0x2), std::byte(0x3) };

  // When
  const std::vector<std::byte> signature = privateKey->sign(bytesToSign);

  // Then
  // ECDSA signatures incorporate random elements, so equality can't be tested. Just make sure its size makes sense.
  EXPECT_LE(signature.size(), ECDSAsecp256k1PrivateKey::MAX_SIGNATURE_SIZE);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, SignEmptyBytes)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey =
    ECDSAsecp256k1PrivateKey::fromString(getTestPrivateKeyHexString());

  // When
  const std::vector<std::byte> signature = privateKey->sign({});

  // Then
  EXPECT_LE(signature.size(), ECDSAsecp256k1PrivateKey::MAX_SIGNATURE_SIZE);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, ToString)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey =
    ECDSAsecp256k1PrivateKey::fromString(getTestPrivateKeyHexString());

  // When
  const std::string stringDer = privateKey->toStringDer();
  const std::string stringRaw = privateKey->toStringRaw();

  // Then
  EXPECT_EQ(stringDer, ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX + getTestPrivateKeyHexString());
  EXPECT_EQ(stringRaw, getTestPrivateKeyHexString());
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, ToBytes)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey =
    ECDSAsecp256k1PrivateKey::fromString(getTestPrivateKeyHexString());

  // When
  const std::vector<std::byte> bytesDer = privateKey->toBytesDer();
  const std::vector<std::byte> bytesRaw = privateKey->toBytesRaw();

  // Then
  EXPECT_EQ(bytesDer,
            concatenateVectors({ ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_BYTES, getTestPrivateKeyBytes() }));
  EXPECT_EQ(bytesRaw, getTestPrivateKeyBytes());
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, GetChainCode)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey =
    ECDSAsecp256k1PrivateKey::fromString(getTestPrivateKeyHexString());

  // When
  const std::vector<std::byte> chainCode = privateKey->getChainCode();

  // Then
  EXPECT_TRUE(chainCode.empty());

  // Chain code functionality is further tested in SLIP10 test vectors
}

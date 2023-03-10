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
#include "ED25519PublicKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PrivateKey.h"
#include "exceptions/BadKeyException.h"
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <memory>
#include <proto/basic_types.pb.h>
#include <vector>

using namespace Hedera;
using namespace Hedera::internal::Utilities;

class ED25519PublicKeyTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::string& getTestPublicKeyHex() const { return mPublicKeyHexString; }
  [[nodiscard]] inline const std::vector<unsigned char>& getTestPublicKeyBytes() const { return mPublicKeyBytes; }

private:
  const std::string mPublicKeyHexString = "F83DEF42411E046461D5AEEAE9311C56F6612557F349F3412DBD95C9FE8B0265";
  const std::vector<unsigned char> mPublicKeyBytes = { 0xF8, 0x3D, 0xEF, 0x42, 0x41, 0x1E, 0x04, 0x64, 0x61, 0xD5, 0xAE,
                                                       0xEA, 0xE9, 0x31, 0x1C, 0x56, 0xF6, 0x61, 0x25, 0x57, 0xF3, 0x49,
                                                       0xF3, 0x41, 0x2D, 0xBD, 0x95, 0xC9, 0xFE, 0x8B, 0x02, 0x65 };
};

//-----
TEST_F(ED25519PublicKeyTest, FromString)
{
  const std::string derEncodedPublicKeyHexString = ED25519PublicKey::DER_ENCODED_PREFIX_HEX + getTestPublicKeyHex();

  const std::shared_ptr<ED25519PublicKey> publicKeyFromString = ED25519PublicKey::fromString(getTestPublicKeyHex());

  ASSERT_NE(publicKeyFromString, nullptr);
  EXPECT_EQ(publicKeyFromString->toStringDer(), derEncodedPublicKeyHexString);
  EXPECT_EQ(publicKeyFromString->toStringRaw(), getTestPublicKeyHex());
  EXPECT_EQ(publicKeyFromString->toBytesDer(),
            concatenateVectors(ED25519PublicKey::DER_ENCODED_PREFIX_BYTES, getTestPublicKeyBytes()));
  EXPECT_EQ(publicKeyFromString->toBytesRaw(), getTestPublicKeyBytes());

  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> publicKeyFromStringDer =
                 ED25519PublicKey::fromStringDer(getTestPublicKeyHex()),
               BadKeyException);

  const std::shared_ptr<ED25519PublicKey> publicKeyFromStringDer =
    ED25519PublicKey::fromStringDer(derEncodedPublicKeyHexString);

  ASSERT_NE(publicKeyFromStringDer, nullptr);
  EXPECT_EQ(publicKeyFromStringDer->toStringDer(), publicKeyFromString->toStringDer());
  EXPECT_EQ(publicKeyFromStringDer->toStringRaw(), publicKeyFromString->toStringRaw());
  EXPECT_EQ(publicKeyFromStringDer->toBytesDer(), publicKeyFromString->toBytesDer());
  EXPECT_EQ(publicKeyFromStringDer->toBytesRaw(), publicKeyFromString->toBytesRaw());

  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> publicKeyFromStringRaw =
                 ED25519PublicKey::fromStringRaw(derEncodedPublicKeyHexString),
               BadKeyException);

  const std::shared_ptr<ED25519PublicKey> publicKeyFromStringRaw =
    ED25519PublicKey::fromStringRaw(getTestPublicKeyHex());

  ASSERT_NE(publicKeyFromStringRaw, nullptr);
  EXPECT_EQ(publicKeyFromStringRaw->toStringDer(), publicKeyFromStringDer->toStringDer());
  EXPECT_EQ(publicKeyFromStringRaw->toStringRaw(), publicKeyFromStringDer->toStringRaw());
  EXPECT_EQ(publicKeyFromStringRaw->toBytesDer(), publicKeyFromStringDer->toBytesDer());
  EXPECT_EQ(publicKeyFromStringRaw->toBytesRaw(), publicKeyFromStringDer->toBytesRaw());

  // Throw if input garbage
  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> key = ED25519PublicKey::fromString("fdsakfdsalf"),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> key =
                 ED25519PublicKey::fromString(ED25519PublicKey::DER_ENCODED_PREFIX_HEX + "fjdskaf;"),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> key =
                 ED25519PublicKey::fromString(ED25519PublicKey::DER_ENCODED_PREFIX_HEX +
                                              "F83DEF42411E046461D5AEEAE9S11C56F661 557F349F3412DBD95C9FE8B026X"),
               BadKeyException);
}

//-----
TEST_F(ED25519PublicKeyTest, FromBytes)
{
  const std::vector<unsigned char> derEncodedPublicKeyBytes =
    concatenateVectors(ED25519PublicKey::DER_ENCODED_PREFIX_BYTES, getTestPublicKeyBytes());

  const std::shared_ptr<ED25519PublicKey> publicKeyFromBytes = ED25519PublicKey::fromBytes(getTestPublicKeyBytes());

  ASSERT_NE(publicKeyFromBytes, nullptr);
  EXPECT_EQ(publicKeyFromBytes->toStringDer(), ED25519PublicKey::DER_ENCODED_PREFIX_HEX + getTestPublicKeyHex());
  EXPECT_EQ(publicKeyFromBytes->toStringRaw(), getTestPublicKeyHex());
  EXPECT_EQ(publicKeyFromBytes->toBytesDer(), derEncodedPublicKeyBytes);
  EXPECT_EQ(publicKeyFromBytes->toBytesRaw(), getTestPublicKeyBytes());

  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> publicKeyFromBytesDer =
                 ED25519PublicKey::fromBytesDer(getTestPublicKeyBytes()),
               BadKeyException);

  const std::shared_ptr<ED25519PublicKey> publicKeyFromBytesDer =
    ED25519PublicKey::fromBytesDer(derEncodedPublicKeyBytes);

  ASSERT_NE(publicKeyFromBytesDer, nullptr);
  EXPECT_EQ(publicKeyFromBytesDer->toStringDer(), publicKeyFromBytes->toStringDer());
  EXPECT_EQ(publicKeyFromBytesDer->toStringRaw(), publicKeyFromBytes->toStringRaw());
  EXPECT_EQ(publicKeyFromBytesDer->toBytesDer(), publicKeyFromBytes->toBytesDer());
  EXPECT_EQ(publicKeyFromBytesDer->toBytesRaw(), publicKeyFromBytes->toBytesRaw());

  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> publicKeyFromBytesRaw =
                 ED25519PublicKey::fromBytesRaw(derEncodedPublicKeyBytes),
               BadKeyException);

  const std::shared_ptr<ED25519PublicKey> publicKeyFromBytesRaw =
    ED25519PublicKey::fromBytesRaw(getTestPublicKeyBytes());

  ASSERT_NE(publicKeyFromBytesRaw, nullptr);
  EXPECT_EQ(publicKeyFromBytesRaw->toStringDer(), publicKeyFromBytesDer->toStringDer());
  EXPECT_EQ(publicKeyFromBytesRaw->toStringRaw(), publicKeyFromBytesDer->toStringRaw());
  EXPECT_EQ(publicKeyFromBytesRaw->toBytesDer(), publicKeyFromBytesDer->toBytesDer());
  EXPECT_EQ(publicKeyFromBytesRaw->toBytesRaw(), publicKeyFromBytesDer->toBytesRaw());

  // Throw if input garbage
  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> key =
                 ED25519PublicKey::fromBytes({ 0x65, 0x4D, 0x58, 0x13, 0x47, 0x21, 0x04, 0x76 }),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> key = ED25519PublicKey::fromBytes(concatenateVectors(
                 ED25519PublicKey::DER_ENCODED_PREFIX_BYTES, { 0x76, 0x47, 0x85, 0x47, 0x15, 0xd4 })),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> key =
                 ED25519PublicKey::fromBytes(ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES),
               BadKeyException);
}

//-----
TEST_F(ED25519PublicKeyTest, Clone)
{
  // Given
  const std::shared_ptr<ED25519PublicKey> publicKey = ED25519PublicKey::fromBytes(getTestPublicKeyBytes());

  // When
  const std::unique_ptr<PublicKey> clonedPublicKey = publicKey->clone();

  // Then
  EXPECT_EQ(publicKey->toStringDer(), clonedPublicKey->toStringDer());
  EXPECT_EQ(publicKey->toStringRaw(), clonedPublicKey->toStringRaw());
  EXPECT_EQ(publicKey->toBytesDer(), clonedPublicKey->toBytesDer());
  EXPECT_EQ(publicKey->toBytesRaw(), clonedPublicKey->toBytesRaw());
}

//-----
TEST_F(ED25519PublicKeyTest, VerifySignatureAgainstModifiedBytes)
{
  // Given
  const std::unique_ptr<ED25519PrivateKey> privateKey = ED25519PrivateKey::fromBytes(getTestPublicKeyBytes());
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();
  std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };

  // When
  const std::vector<unsigned char> signature = privateKey->sign(bytesToSign);
  bytesToSign.push_back(0x4);

  // Then
  EXPECT_FALSE(publicKey->verifySignature(signature, bytesToSign));

  // Signature verification is further tested in RFV8032 test vectors
}

//-----
TEST_F(ED25519PublicKeyTest, VerifyArbitrarySignature)
{
  // Given
  const std::shared_ptr<ED25519PublicKey> publicKey = ED25519PublicKey::fromBytes(getTestPublicKeyBytes());
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> arbitrarySignature = { 0x1, 0x2, 0x3, 0x4 };

  // When / Then
  EXPECT_FALSE(publicKey->verifySignature(arbitrarySignature, bytesToSign));

  // Signature verification is further tested in RFV8032 test vectors
}

//-----
TEST_F(ED25519PublicKeyTest, VerifyEmptySignature)
{
  // Given / When / Then
  EXPECT_FALSE(ED25519PublicKey::fromBytes(getTestPublicKeyBytes())->verifySignature({}, { 0x1, 0x2, 0x3 }));

  // Signature verification is further tested in RFV8032 test vectors
}

//-----
TEST_F(ED25519PublicKeyTest, ToString)
{
  // Given
  const std::shared_ptr<ED25519PublicKey> publicKey = ED25519PublicKey::fromBytes(getTestPublicKeyBytes());

  // When
  const std::string stringDer = publicKey->toStringDer();
  const std::string stringRaw = publicKey->toStringRaw();

  // Then
  EXPECT_EQ(stringDer, ED25519PublicKey::DER_ENCODED_PREFIX_HEX + getTestPublicKeyHex());
  EXPECT_EQ(stringRaw, getTestPublicKeyHex());
}

//-----
TEST_F(ED25519PublicKeyTest, ToBytes)
{
  // Given
  const std::shared_ptr<ED25519PublicKey> publicKey = ED25519PublicKey::fromString(getTestPublicKeyHex());

  // When
  const std::vector<unsigned char> bytesDer = publicKey->toBytesDer();
  const std::vector<unsigned char> bytesRaw = publicKey->toBytesRaw();

  // Then
  EXPECT_EQ(bytesDer, concatenateVectors(ED25519PublicKey::DER_ENCODED_PREFIX_BYTES, getTestPublicKeyBytes()));
  EXPECT_EQ(bytesRaw, getTestPublicKeyBytes());
}

//-----
TEST_F(ED25519PublicKeyTest, PublicKeyToProtobuf)
{
  // Given
  const std::shared_ptr<ED25519PublicKey> publicKey = ED25519PublicKey::fromBytes(getTestPublicKeyBytes());

  // When
  const std::unique_ptr<proto::Key> protobufKey = publicKey->toProtobuf();

  // Then
  ASSERT_NE(protobufKey, nullptr);
  EXPECT_TRUE(protobufKey->has_ed25519());

  const std::vector<unsigned char> protobufEd25519PublicKeyBytes = { protobufKey->ed25519().cbegin(),
                                                                     protobufKey->ed25519().cend() };
  EXPECT_EQ(protobufEd25519PublicKeyBytes, getTestPublicKeyBytes());
}

//-----
TEST_F(ED25519PublicKeyTest, PublicKeyFromProtobuf)
{
  // Given
  const std::unique_ptr<proto::Key> protobufKey = ED25519PublicKey::fromBytes(getTestPublicKeyBytes())->toProtobuf();

  // When
  const std::shared_ptr<PublicKey> publicKey = PublicKey::fromProtobuf(*protobufKey);

  // Then
  ASSERT_NE(publicKey, nullptr);
  EXPECT_EQ(publicKey->toStringDer(), ED25519PublicKey::DER_ENCODED_PREFIX_HEX + getTestPublicKeyHex());
  EXPECT_EQ(publicKey->toStringRaw(), getTestPublicKeyHex());
  EXPECT_EQ(publicKey->toBytesDer(),
            concatenateVectors(ED25519PublicKey::DER_ENCODED_PREFIX_BYTES, getTestPublicKeyBytes()));
  EXPECT_EQ(publicKey->toBytesRaw(), getTestPublicKeyBytes());
}

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
#include "ECDSAsecp256k1PublicKey.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "exceptions/BadKeyException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <memory>
#include <proto/basic_types.pb.h>
#include <vector>

using namespace Hedera;
using namespace Hedera::internal::Utilities;

class ECDSAsecp256k1PublicKeyTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::string& getTestUncompressedPublicKeyHex() const { return mUncompressedPublicKeyHex; }
  [[nodiscard]] inline const std::string& getTestCompressedPublicKeyHex() const { return mCompressedPublicKeyHex; }
  [[nodiscard]] inline const std::vector<unsigned char>& getTestUncompressedPublicKeyBytes() const
  {
    return mUncompressedPublicKeyBytes;
  }
  [[nodiscard]] inline const std::vector<unsigned char>& getTestCompressedPublicKeyBytes() const
  {
    return mCompressedPublicKeyBytes;
  }

private:
  const std::string mUncompressedPublicKeyHex = "045B36E22D710E79646F1A86D633EB38343BFE9DF39185EC730B1E7DFA79EE92CFD8C9"
                                                "80B4FB4DC5493A0EE40A85543FFC49E3CDC65E0B8B8C8A8AB64A00D9B5BE";
  const std::string mCompressedPublicKeyHex = "025B36E22D710E79646F1A86D633EB38343BFE9DF39185EC730B1E7DFA79EE92CF";
  const std::vector<unsigned char> mUncompressedPublicKeyBytes = {
    0x04, 0x5B, 0x36, 0xE2, 0x2D, 0x71, 0x0E, 0x79, 0x64, 0x6F, 0x1A, 0x86, 0xD6, 0x33, 0xEB, 0x38, 0x34,
    0x3B, 0xFE, 0x9D, 0xF3, 0x91, 0x85, 0xEC, 0x73, 0x0B, 0x1E, 0x7D, 0xFA, 0x79, 0xEE, 0x92, 0xCF, 0xD8,
    0xC9, 0x80, 0xB4, 0xFB, 0x4D, 0xC5, 0x49, 0x3A, 0x0E, 0xE4, 0x0A, 0x85, 0x54, 0x3F, 0xFC, 0x49, 0xE3,
    0xCD, 0xC6, 0x5E, 0x0B, 0x8B, 0x8C, 0x8A, 0x8A, 0xB6, 0x4A, 0x00, 0xD9, 0xB5, 0xBE
  };
  const std::vector<unsigned char> mCompressedPublicKeyBytes = { 0x02, 0x5B, 0x36, 0xE2, 0x2D, 0x71, 0x0E, 0x79, 0x64,
                                                                 0x6F, 0x1A, 0x86, 0xD6, 0x33, 0xEB, 0x38, 0x34, 0x3B,
                                                                 0xFE, 0x9D, 0xF3, 0x91, 0x85, 0xEC, 0x73, 0x0B, 0x1E,
                                                                 0x7D, 0xFA, 0x79, 0xEE, 0x92, 0xCF };
};

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, FromString)
{
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressed =
    ECDSAsecp256k1PublicKey::fromString(getTestUncompressedPublicKeyHex());
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressed =
    ECDSAsecp256k1PublicKey::fromString(getTestCompressedPublicKeyHex());

  ASSERT_NE(publicKeyFromUncompressed, nullptr);
  ASSERT_NE(publicKeyFromCompressed, nullptr);
  EXPECT_EQ(publicKeyFromUncompressed->toStringRaw(), publicKeyFromCompressed->toStringRaw());
  EXPECT_EQ(publicKeyFromUncompressed->toStringDer(), publicKeyFromCompressed->toStringDer());
  EXPECT_EQ(publicKeyFromUncompressed->toBytesRaw(), publicKeyFromCompressed->toBytesRaw());
  EXPECT_EQ(publicKeyFromUncompressed->toBytesDer(), publicKeyFromCompressed->toBytesDer());

  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedDer =
                 ECDSAsecp256k1PublicKey::fromStringDer(getTestUncompressedPublicKeyHex()),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedDer =
                 ECDSAsecp256k1PublicKey::fromStringDer(getTestCompressedPublicKeyHex()),
               BadKeyException);

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedDer = ECDSAsecp256k1PublicKey::fromStringDer(
    ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_HEX + getTestUncompressedPublicKeyHex());
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedDer = ECDSAsecp256k1PublicKey::fromStringDer(
    ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX + getTestCompressedPublicKeyHex());

  ASSERT_NE(publicKeyFromUncompressedDer, nullptr);
  ASSERT_NE(publicKeyFromCompressedDer, nullptr);
  EXPECT_EQ(publicKeyFromUncompressedDer->toStringRaw(), publicKeyFromCompressedDer->toStringRaw());
  EXPECT_EQ(publicKeyFromUncompressedDer->toStringDer(), publicKeyFromCompressedDer->toStringDer());
  EXPECT_EQ(publicKeyFromUncompressedDer->toBytesRaw(), publicKeyFromCompressedDer->toBytesRaw());
  EXPECT_EQ(publicKeyFromUncompressedDer->toBytesDer(), publicKeyFromCompressedDer->toBytesDer());

  EXPECT_EQ(publicKeyFromUncompressedDer->toStringRaw(), publicKeyFromCompressed->toStringRaw());
  EXPECT_EQ(publicKeyFromUncompressedDer->toStringDer(), publicKeyFromCompressed->toStringDer());
  EXPECT_EQ(publicKeyFromUncompressedDer->toBytesRaw(), publicKeyFromCompressed->toBytesRaw());
  EXPECT_EQ(publicKeyFromUncompressedDer->toBytesDer(), publicKeyFromCompressed->toBytesDer());

  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedRaw =
                 ECDSAsecp256k1PublicKey::fromStringRaw(ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_HEX +
                                                        getTestUncompressedPublicKeyHex()),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedRaw =
                 ECDSAsecp256k1PublicKey::fromStringRaw(ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX +
                                                        getTestCompressedPublicKeyHex()),
               BadKeyException);

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedRaw =
    ECDSAsecp256k1PublicKey::fromStringRaw(getTestUncompressedPublicKeyHex());
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedRaw =
    ECDSAsecp256k1PublicKey::fromStringRaw(getTestCompressedPublicKeyHex());

  ASSERT_NE(publicKeyFromUncompressedRaw, nullptr);
  ASSERT_NE(publicKeyFromCompressedRaw, nullptr);
  EXPECT_EQ(publicKeyFromUncompressedRaw->toStringRaw(), publicKeyFromCompressedRaw->toStringRaw());
  EXPECT_EQ(publicKeyFromUncompressedRaw->toStringDer(), publicKeyFromCompressedRaw->toStringDer());
  EXPECT_EQ(publicKeyFromUncompressedRaw->toBytesRaw(), publicKeyFromCompressedRaw->toBytesRaw());
  EXPECT_EQ(publicKeyFromUncompressedRaw->toBytesDer(), publicKeyFromCompressedRaw->toBytesDer());

  EXPECT_EQ(publicKeyFromCompressedRaw->toStringRaw(), publicKeyFromUncompressedDer->toStringRaw());
  EXPECT_EQ(publicKeyFromCompressedRaw->toStringDer(), publicKeyFromUncompressedDer->toStringDer());
  EXPECT_EQ(publicKeyFromCompressedRaw->toBytesRaw(), publicKeyFromUncompressedDer->toBytesRaw());
  EXPECT_EQ(publicKeyFromCompressedRaw->toBytesDer(), publicKeyFromUncompressedDer->toBytesDer());

  // Throw if input garbage
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> key = ECDSAsecp256k1PublicKey::fromString("fdsakfdsalf"),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> key = ECDSAsecp256k1PublicKey::fromString(
                 ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_HEX + "fjdskaf;"),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> key = ECDSAsecp256k1PublicKey::fromString(
                 ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX +
                 "XXXB36E22D710E79646F1A86D633PB38343BFE9DF39185EC7G0B1E7DFA79EE9ABC"),
               BadKeyException);
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, FromBytes)
{
  std::vector<unsigned char> publicKeyCompressed = { 0x02, 0x5B, 0x36, 0xE2, 0x2D, 0x71, 0x0E, 0x79, 0x64, 0x6F, 0x1A,
                                                     0x86, 0xD6, 0x33, 0xEB, 0x38, 0x34, 0x3B, 0xFE, 0x9D, 0xF3, 0x91,
                                                     0x85, 0xEC, 0x73, 0x0B, 0x1E, 0x7D, 0xFA, 0x79, 0xEE, 0x92, 0xCF };

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressed =
    ECDSAsecp256k1PublicKey::fromBytes(getTestUncompressedPublicKeyBytes());
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressed =
    ECDSAsecp256k1PublicKey::fromBytes(publicKeyCompressed);

  ASSERT_NE(publicKeyFromUncompressed, nullptr);
  ASSERT_NE(publicKeyFromCompressed, nullptr);
  EXPECT_EQ(publicKeyFromUncompressed->toStringRaw(), publicKeyFromCompressed->toStringRaw());
  EXPECT_EQ(publicKeyFromUncompressed->toStringDer(), publicKeyFromCompressed->toStringDer());
  EXPECT_EQ(publicKeyFromUncompressed->toBytesRaw(), publicKeyFromCompressed->toBytesRaw());
  EXPECT_EQ(publicKeyFromUncompressed->toBytesDer(), publicKeyFromCompressed->toBytesDer());

  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedDer =
                 ECDSAsecp256k1PublicKey::fromBytesDer(getTestUncompressedPublicKeyBytes()),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedDer =
                 ECDSAsecp256k1PublicKey::fromBytesDer(publicKeyCompressed),
               BadKeyException);

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedDer =
    ECDSAsecp256k1PublicKey::fromBytesDer(concatenateVectors(
      ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES, getTestUncompressedPublicKeyBytes()));
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedDer =
    ECDSAsecp256k1PublicKey::fromBytesDer(concatenateVectors(
      ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES, getTestCompressedPublicKeyBytes()));

  ASSERT_NE(publicKeyFromUncompressedDer, nullptr);
  ASSERT_NE(publicKeyFromCompressedDer, nullptr);
  EXPECT_EQ(publicKeyFromUncompressedDer->toStringRaw(), publicKeyFromCompressedDer->toStringRaw());
  EXPECT_EQ(publicKeyFromUncompressedDer->toStringDer(), publicKeyFromCompressedDer->toStringDer());
  EXPECT_EQ(publicKeyFromUncompressedDer->toBytesRaw(), publicKeyFromCompressedDer->toBytesRaw());
  EXPECT_EQ(publicKeyFromUncompressedDer->toBytesDer(), publicKeyFromCompressedDer->toBytesDer());

  EXPECT_EQ(publicKeyFromUncompressedDer->toStringRaw(), publicKeyFromCompressed->toStringRaw());
  EXPECT_EQ(publicKeyFromUncompressedDer->toStringDer(), publicKeyFromCompressed->toStringDer());
  EXPECT_EQ(publicKeyFromUncompressedDer->toBytesRaw(), publicKeyFromCompressed->toBytesRaw());
  EXPECT_EQ(publicKeyFromUncompressedDer->toBytesDer(), publicKeyFromCompressed->toBytesDer());

  EXPECT_THROW(
    const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedRaw =
      ECDSAsecp256k1PublicKey::fromBytesRaw(concatenateVectors(
        ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES, getTestUncompressedPublicKeyBytes())),
    BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedRaw =
                 ECDSAsecp256k1PublicKey::fromBytesRaw(concatenateVectors(
                   ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES, getTestCompressedPublicKeyBytes())),
               BadKeyException);

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedRaw =
    ECDSAsecp256k1PublicKey::fromBytesRaw(getTestUncompressedPublicKeyBytes());
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedRaw =
    ECDSAsecp256k1PublicKey::fromBytesRaw(getTestCompressedPublicKeyBytes());

  ASSERT_NE(publicKeyFromUncompressedRaw, nullptr);
  ASSERT_NE(publicKeyFromCompressedRaw, nullptr);
  EXPECT_EQ(publicKeyFromUncompressedRaw->toStringRaw(), publicKeyFromCompressedRaw->toStringRaw());
  EXPECT_EQ(publicKeyFromUncompressedRaw->toStringDer(), publicKeyFromCompressedRaw->toStringDer());
  EXPECT_EQ(publicKeyFromUncompressedRaw->toBytesRaw(), publicKeyFromCompressedRaw->toBytesRaw());
  EXPECT_EQ(publicKeyFromUncompressedRaw->toBytesDer(), publicKeyFromCompressedRaw->toBytesDer());

  EXPECT_EQ(publicKeyFromCompressedRaw->toStringRaw(), publicKeyFromUncompressedDer->toStringRaw());
  EXPECT_EQ(publicKeyFromCompressedRaw->toStringDer(), publicKeyFromUncompressedDer->toStringDer());
  EXPECT_EQ(publicKeyFromCompressedRaw->toBytesRaw(), publicKeyFromUncompressedDer->toBytesRaw());
  EXPECT_EQ(publicKeyFromCompressedRaw->toBytesDer(), publicKeyFromUncompressedDer->toBytesDer());

  // Throw if input garbage
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> key =
                 ECDSAsecp256k1PublicKey::fromBytes({ 0x65, 0x4D, 0x58, 0x13, 0x47, 0x21, 0x04, 0x76 }),
               BadKeyException);
  EXPECT_THROW(
    const std::shared_ptr<ECDSAsecp256k1PublicKey> key = ECDSAsecp256k1PublicKey::fromBytes(concatenateVectors(
      ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES, { 0x76, 0x47, 0x85, 0x47, 0x15, 0xd4 })),
    BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> key =
                 ECDSAsecp256k1PublicKey::fromBytes(ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES),
               BadKeyException);
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, CompressBytes)
{
  // Given / When
  const std::vector<unsigned char> compressedBytes =
    ECDSAsecp256k1PublicKey::compressBytes(getTestUncompressedPublicKeyBytes());

  // Then
  EXPECT_EQ(compressedBytes, getTestCompressedPublicKeyBytes());

  // Throw if not correct size
  EXPECT_THROW(const std::vector<unsigned char> bytes = ECDSAsecp256k1PublicKey::compressBytes({ 0x4, 0x3, 0x2, 0x1 }),
               std::invalid_argument);
  // Throw if not starting with 0x4
  EXPECT_THROW(const std::vector<unsigned char> bytes = ECDSAsecp256k1PublicKey::compressBytes(
                 std::vector<unsigned char>(ECDSAsecp256k1PublicKey::UNCOMPRESSED_KEY_SIZE, 0x3)),
               std::invalid_argument);
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, UncompressBytes)
{
  // Given / When
  const std::vector<unsigned char> uncompressedBytes =
    ECDSAsecp256k1PublicKey::uncompressBytes(getTestCompressedPublicKeyBytes());

  // Then
  EXPECT_EQ(uncompressedBytes, getTestUncompressedPublicKeyBytes());

  // Throw if not correct size
  EXPECT_THROW(const std::vector<unsigned char> bytes = ECDSAsecp256k1PublicKey::uncompressBytes({ 0x3, 0x2, 0x1 }),
               std::invalid_argument);
  // Throw if not starting with 0x2 or 0x3
  EXPECT_THROW(const std::vector<unsigned char> bytes = ECDSAsecp256k1PublicKey::uncompressBytes(
                 std::vector<unsigned char>(ECDSAsecp256k1PublicKey::COMPRESSED_KEY_SIZE, 0x1)),
               std::invalid_argument);
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, Clone)
{
  // Given
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKey =
    ECDSAsecp256k1PublicKey::fromBytes(getTestUncompressedPublicKeyBytes());

  // When
  const std::unique_ptr<PublicKey> clonedPublicKey = publicKey->clone();

  // Then
  EXPECT_EQ(publicKey->toStringDer(), clonedPublicKey->toStringDer());
  EXPECT_EQ(publicKey->toStringRaw(), clonedPublicKey->toStringRaw());
  EXPECT_EQ(publicKey->toBytesDer(), clonedPublicKey->toBytesDer());
  EXPECT_EQ(publicKey->toBytesRaw(), clonedPublicKey->toBytesRaw());
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyValidSignature)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };

  // When
  const std::vector<unsigned char> signature = privateKey->sign(bytesToSign);

  // Then
  EXPECT_TRUE(publicKey->verifySignature(signature, bytesToSign));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyValidSignatureOfEmptyMessage)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();

  // When
  const std::vector<unsigned char> signature = privateKey->sign({});

  // Then
  EXPECT_TRUE(publicKey->verifySignature(signature, {}));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifySignatureAgainstModifiedBytes)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();
  std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };

  // When
  const std::vector<unsigned char> signature = privateKey->sign(bytesToSign);
  bytesToSign.push_back(0x4);

  // Then
  EXPECT_FALSE(publicKey->verifySignature(signature, bytesToSign));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyArbitrarySignature)
{
  // Given
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKey =
    ECDSAsecp256k1PublicKey::fromBytes(getTestUncompressedPublicKeyBytes());
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> arbitrarySignature = { 0x1, 0x2, 0x3, 0x4 };

  // When / Then
  EXPECT_FALSE(publicKey->verifySignature(arbitrarySignature, bytesToSign));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyEmptySignature)
{
  // Given
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKey =
    ECDSAsecp256k1PublicKey::fromBytes(getTestUncompressedPublicKeyBytes());
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };

  // When / Then
  EXPECT_FALSE(publicKey->verifySignature({}, bytesToSign));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyEmptyMessage)
{
  // Given
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();

  // When
  const std::vector<unsigned char> signature = privateKey->sign({ 0x1, 0x2, 0x3 });

  // Then
  EXPECT_FALSE(publicKey->verifySignature(signature, {}));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, ToString)
{
  // Given
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKey =
    ECDSAsecp256k1PublicKey::fromBytes(getTestUncompressedPublicKeyBytes());

  // When
  const std::string stringDer = publicKey->toStringDer();
  const std::string stringRaw = publicKey->toStringRaw();

  // Then
  EXPECT_EQ(stringDer, ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX + getTestCompressedPublicKeyHex());
  EXPECT_EQ(stringRaw, getTestCompressedPublicKeyHex());
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, ToBytes)
{
  // Given
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKey =
    ECDSAsecp256k1PublicKey::fromBytes(getTestUncompressedPublicKeyBytes());

  // When
  const std::vector<unsigned char> bytesDer = publicKey->toBytesDer();
  const std::vector<unsigned char> bytesRaw = publicKey->toBytesRaw();

  // Then
  EXPECT_EQ(bytesDer,
            concatenateVectors(ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES,
                               getTestCompressedPublicKeyBytes()));
  EXPECT_EQ(bytesRaw, getTestCompressedPublicKeyBytes());
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, PublicKeyToProtobuf)
{
  // Given
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKey =
    ECDSAsecp256k1PublicKey::fromBytes(getTestUncompressedPublicKeyBytes());

  // When
  const std::unique_ptr<proto::Key> protobufKey = publicKey->toProtobuf();

  // Then
  ASSERT_NE(protobufKey, nullptr);
  EXPECT_TRUE(protobufKey->has_ecdsa_secp256k1());

  const std::vector<unsigned char> protobufEcdsaSecp256K1PublicKeyBytes = { protobufKey->ecdsa_secp256k1().cbegin(),
                                                                            protobufKey->ecdsa_secp256k1().cend() };
  EXPECT_EQ(protobufEcdsaSecp256K1PublicKeyBytes, getTestCompressedPublicKeyBytes());
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, PublicKeyFromProtobuf)
{
  // Given
  const std::unique_ptr<proto::Key> protobufKey =
    ECDSAsecp256k1PublicKey::fromBytes(getTestUncompressedPublicKeyBytes())->toProtobuf();

  // When
  const std::shared_ptr<PublicKey> publicKey = PublicKey::fromProtobuf(*protobufKey);

  // Then
  ASSERT_NE(publicKey, nullptr);
  EXPECT_EQ(publicKey->toStringDer(),
            ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX + getTestCompressedPublicKeyHex());
  EXPECT_EQ(publicKey->toStringRaw(), getTestCompressedPublicKeyHex());
  EXPECT_EQ(publicKey->toBytesDer(),
            concatenateVectors(ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES,
                               getTestCompressedPublicKeyBytes()));
  EXPECT_EQ(publicKey->toBytesRaw(), getTestCompressedPublicKeyBytes());
}

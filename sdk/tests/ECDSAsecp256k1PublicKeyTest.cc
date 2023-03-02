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
#include "impl/HexConverter.h"

#include <gtest/gtest.h>
#include <memory>
#include <proto/basic_types.pb.h>
#include <vector>

using namespace Hedera;

class ECDSAsecp256k1PublicKeyTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::unique_ptr<ECDSAsecp256k1PrivateKey>& getTestPrivateKey() const
  {
    return mPrivateKey;
  }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKeyFromPrivate() const
  {
    return mPublicKeyFromPrivate;
  }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKeyFromString() const
  {
    return mPublicKeyFromString;
  }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKeyFromProtobuf() const
  {
    return mPublicKeyFromProtobuf;
  }

private:
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> mPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> mPublicKeyFromPrivate = mPrivateKey->getPublicKey();
  const std::shared_ptr<PublicKey> mPublicKeyFromString =
    ECDSAsecp256k1PublicKey::fromString(mPublicKeyFromPrivate->toStringDer());
  const std::shared_ptr<PublicKey> mPublicKeyFromProtobuf =
    PublicKey::fromProtobuf(*mPublicKeyFromString->toProtobuf());
};

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, CopyAndMoveConstructors)
{
  ECDSAsecp256k1PublicKey copiedPublicKey(*dynamic_cast<ECDSAsecp256k1PublicKey*>(getTestPublicKeyFromPrivate().get()));
  EXPECT_EQ(copiedPublicKey.toStringDer(), getTestPublicKeyFromPrivate()->toStringDer());

  copiedPublicKey = *dynamic_cast<ECDSAsecp256k1PublicKey*>(getTestPublicKeyFromString().get());
  EXPECT_EQ(copiedPublicKey.toStringDer(), getTestPublicKeyFromString()->toStringDer());

  ECDSAsecp256k1PublicKey movedPublicKey(std::move(copiedPublicKey));
  EXPECT_EQ(movedPublicKey.toStringDer(), getTestPublicKeyFromString()->toStringDer());

  copiedPublicKey = std::move(movedPublicKey);
  EXPECT_EQ(copiedPublicKey.toStringDer(), getTestPublicKeyFromString()->toStringDer());
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, FromString)
{
  const std::string publicKeyUncompressed = "045B36E22D710E79646F1A86D633EB38343BFE9DF39185EC730B1E7DFA79EE92CFD8C980"
                                            "B4FB4DC5493A0EE40A85543FFC49E3CDC65E0B8B8C8A8AB64A00D9B5BE";
  const std::string publicKeyCompressed = "025B36E22D710E79646F1A86D633EB38343BFE9DF39185EC730B1E7DFA79EE92CF";

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressed =
    ECDSAsecp256k1PublicKey::fromString(publicKeyUncompressed);
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressed =
    ECDSAsecp256k1PublicKey::fromString(publicKeyCompressed);

  ASSERT_NE(publicKeyFromUncompressed, nullptr);
  ASSERT_NE(publicKeyFromCompressed, nullptr);
  EXPECT_EQ(publicKeyFromUncompressed->toStringRaw(), publicKeyFromCompressed->toStringRaw());
  EXPECT_EQ(publicKeyFromUncompressed->toStringDer(), publicKeyFromCompressed->toStringDer());
  EXPECT_EQ(publicKeyFromUncompressed->toBytesRaw(), publicKeyFromCompressed->toBytesRaw());
  EXPECT_EQ(publicKeyFromUncompressed->toBytesDer(), publicKeyFromCompressed->toBytesDer());

  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedDer =
                 ECDSAsecp256k1PublicKey::fromStringDer(publicKeyUncompressed),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedDer =
                 ECDSAsecp256k1PublicKey::fromStringDer(publicKeyCompressed),
               BadKeyException);

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedDer = ECDSAsecp256k1PublicKey::fromStringDer(
    ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_HEX + publicKeyUncompressed);
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedDer = ECDSAsecp256k1PublicKey::fromStringDer(
    ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX + publicKeyCompressed);

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
                                                        publicKeyUncompressed),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedRaw =
                 ECDSAsecp256k1PublicKey::fromStringRaw(ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX +
                                                        publicKeyCompressed),
               BadKeyException);

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedRaw =
    ECDSAsecp256k1PublicKey::fromStringRaw(publicKeyUncompressed);
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedRaw =
    ECDSAsecp256k1PublicKey::fromStringRaw(publicKeyCompressed);

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
  std::vector<unsigned char> publicKeyUncompressed = { 0x04, 0x5B, 0x36, 0xE2, 0x2D, 0x71, 0x0E, 0x79, 0x64, 0x6F, 0x1A,
                                                       0x86, 0xD6, 0x33, 0xEB, 0x38, 0x34, 0x3B, 0xFE, 0x9D, 0xF3, 0x91,
                                                       0x85, 0xEC, 0x73, 0x0B, 0x1E, 0x7D, 0xFA, 0x79, 0xEE, 0x92, 0xCF,
                                                       0xD8, 0xC9, 0x80, 0xB4, 0xFB, 0x4D, 0xC5, 0x49, 0x3A, 0x0E, 0xE4,
                                                       0x0A, 0x85, 0x54, 0x3F, 0xFC, 0x49, 0xE3, 0xCD, 0xC6, 0x5E, 0x0B,
                                                       0x8B, 0x8C, 0x8A, 0x8A, 0xB6, 0x4A, 0x00, 0xD9, 0xB5, 0xBE };
  std::vector<unsigned char> publicKeyCompressed = { 0x02, 0x5B, 0x36, 0xE2, 0x2D, 0x71, 0x0E, 0x79, 0x64, 0x6F, 0x1A,
                                                     0x86, 0xD6, 0x33, 0xEB, 0x38, 0x34, 0x3B, 0xFE, 0x9D, 0xF3, 0x91,
                                                     0x85, 0xEC, 0x73, 0x0B, 0x1E, 0x7D, 0xFA, 0x79, 0xEE, 0x92, 0xCF };

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressed =
    ECDSAsecp256k1PublicKey::fromBytes(publicKeyUncompressed);
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressed =
    ECDSAsecp256k1PublicKey::fromBytes(publicKeyCompressed);

  ASSERT_NE(publicKeyFromUncompressed, nullptr);
  ASSERT_NE(publicKeyFromCompressed, nullptr);
  EXPECT_EQ(publicKeyFromUncompressed->toStringRaw(), publicKeyFromCompressed->toStringRaw());
  EXPECT_EQ(publicKeyFromUncompressed->toStringDer(), publicKeyFromCompressed->toStringDer());
  EXPECT_EQ(publicKeyFromUncompressed->toBytesRaw(), publicKeyFromCompressed->toBytesRaw());
  EXPECT_EQ(publicKeyFromUncompressed->toBytesDer(), publicKeyFromCompressed->toBytesDer());

  publicKeyUncompressed.insert(publicKeyUncompressed.begin(),
                               ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.cbegin(),
                               ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.cend());
  publicKeyCompressed.insert(publicKeyCompressed.begin(),
                             ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES.cbegin(),
                             ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES.cend());

  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedDer =
                 ECDSAsecp256k1PublicKey::fromBytesRaw(publicKeyUncompressed),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedDer =
                 ECDSAsecp256k1PublicKey::fromBytesRaw(publicKeyCompressed),
               BadKeyException);

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedDer =
    ECDSAsecp256k1PublicKey::fromBytesDer(publicKeyUncompressed);
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedDer =
    ECDSAsecp256k1PublicKey::fromBytesDer(publicKeyCompressed);

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

  publicKeyUncompressed.erase(
    publicKeyUncompressed.begin(),
    publicKeyUncompressed.begin() +
      static_cast<long>(ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size()));
  publicKeyCompressed.erase(publicKeyCompressed.begin(),
                            publicKeyCompressed.begin() +
                              static_cast<long>(ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES.size()));

  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedRaw =
                 ECDSAsecp256k1PublicKey::fromBytesDer(publicKeyUncompressed),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedRaw =
                 ECDSAsecp256k1PublicKey::fromBytesDer(publicKeyCompressed),
               BadKeyException);

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressedRaw =
    ECDSAsecp256k1PublicKey::fromBytesRaw(publicKeyUncompressed);
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressedRaw =
    ECDSAsecp256k1PublicKey::fromBytesRaw(publicKeyCompressed);

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
                 ECDSAsecp256k1PublicKey::fromBytes(internal::HexConverter::hexToBytes("654d581347210476")),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> key =
                 ECDSAsecp256k1PublicKey::fromBytes(internal::HexConverter::hexToBytes(
                   ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_HEX + "7647854715d4")),
               BadKeyException);
  EXPECT_THROW(const std::shared_ptr<ECDSAsecp256k1PublicKey> key = ECDSAsecp256k1PublicKey::fromBytes(
                 internal::HexConverter::hexToBytes(ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX)),
               BadKeyException);
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, CompressBytes)
{
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
  const std::shared_ptr<PublicKey> publicKey = ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  ASSERT_NE(publicKey, nullptr);

  const std::shared_ptr<PublicKey> clonedPublicKey = publicKey->clone();
  ASSERT_NE(clonedPublicKey, nullptr);

  EXPECT_EQ(publicKey->toStringDer(), clonedPublicKey->toStringDer());
  EXPECT_EQ(publicKey->toStringRaw(), clonedPublicKey->toStringRaw());
  EXPECT_EQ(publicKey->toBytesDer(), clonedPublicKey->toBytesDer());
  EXPECT_EQ(publicKey->toBytesRaw(), clonedPublicKey->toBytesRaw());
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyValidSignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign(bytesToSign);

  EXPECT_TRUE(getTestPublicKeyFromPrivate()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromString()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromProtobuf()->verifySignature(signature, bytesToSign));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyValidSignatureOfEmptyMessage)
{
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign({});

  EXPECT_TRUE(getTestPublicKeyFromPrivate()->verifySignature(signature, {}));
  EXPECT_TRUE(getTestPublicKeyFromString()->verifySignature(signature, {}));
  EXPECT_TRUE(getTestPublicKeyFromProtobuf()->verifySignature(signature, {}));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifySignatureAgainstModifiedBytes)
{
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign({ 0x1, 0x2, 0x3 });
  const std::vector<unsigned char> modifiedBytes = { 0x1, 0x2, 0x3, 0x4 };

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature(signature, modifiedBytes));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature(signature, modifiedBytes));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature(signature, modifiedBytes));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyArbitrarySignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> arbitrarySignature = { 0x1, 0x2, 0x3, 0x4 };

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature(arbitrarySignature, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature(arbitrarySignature, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature(arbitrarySignature, bytesToSign));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyEmptySignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature({}, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature({}, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature({}, bytesToSign));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyEmptyMessage)
{
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign({ 0x1, 0x2, 0x3 });
  const std::vector<unsigned char> emptyMessage;

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature(signature, emptyMessage));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature(signature, emptyMessage));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature(signature, emptyMessage));
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, ToString)
{
  const std::string derEncodingFromPrivate = getTestPublicKeyFromPrivate()->toStringDer();
  const std::string derEncodingFromLoaded = getTestPublicKeyFromString()->toStringDer();
  const std::string derEncodingFromProtobuf = getTestPublicKeyFromProtobuf()->toStringDer();

  EXPECT_EQ(derEncodingFromPrivate.size(),
            ECDSAsecp256k1PublicKey::COMPRESSED_KEY_SIZE * 2 +
              ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX.size());
  EXPECT_EQ(derEncodingFromLoaded.size(),
            ECDSAsecp256k1PublicKey::COMPRESSED_KEY_SIZE * 2 +
              ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX.size());
  EXPECT_EQ(derEncodingFromProtobuf.size(),
            ECDSAsecp256k1PublicKey::COMPRESSED_KEY_SIZE * 2 +
              ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX.size());

  EXPECT_EQ(derEncodingFromPrivate, derEncodingFromLoaded);
  EXPECT_EQ(derEncodingFromPrivate, derEncodingFromProtobuf);

  const std::string rawEncodingFromPrivate = getTestPublicKeyFromPrivate()->toStringRaw();
  const std::string rawEncodingFromLoaded = getTestPublicKeyFromString()->toStringRaw();
  const std::string rawEncodingFromProtobuf = getTestPublicKeyFromProtobuf()->toStringRaw();

  EXPECT_EQ(rawEncodingFromPrivate.size(), ECDSAsecp256k1PublicKey::COMPRESSED_KEY_SIZE * 2);
  EXPECT_EQ(rawEncodingFromLoaded.size(), ECDSAsecp256k1PublicKey::COMPRESSED_KEY_SIZE * 2);
  EXPECT_EQ(rawEncodingFromProtobuf.size(), ECDSAsecp256k1PublicKey::COMPRESSED_KEY_SIZE * 2);

  EXPECT_EQ(rawEncodingFromPrivate, rawEncodingFromLoaded);
  EXPECT_EQ(rawEncodingFromPrivate, rawEncodingFromProtobuf);
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, PublicKeyToProtobuf)
{
  // Given
  const std::shared_ptr<PublicKey> testPublicKey = getTestPublicKeyFromString();

  // When
  const std::unique_ptr<proto::Key> protobufEcdsaSecp256K1PublicKey = testPublicKey->toProtobuf();
  const std::vector<unsigned char> protobufEcdsaSecp256K1PublicKeyBytes = {
    protobufEcdsaSecp256K1PublicKey->ecdsa_secp256k1().cbegin(),
    protobufEcdsaSecp256K1PublicKey->ecdsa_secp256k1().cend()
  };

  // Then
  ASSERT_NE(protobufEcdsaSecp256K1PublicKey, nullptr);
  EXPECT_TRUE(protobufEcdsaSecp256K1PublicKey->has_ecdsa_secp256k1());
  EXPECT_EQ(testPublicKey->toBytesRaw(), protobufEcdsaSecp256K1PublicKeyBytes);
}

//-----
TEST_F(ECDSAsecp256k1PublicKeyTest, PublicKeyFromProtobuf)
{
  // Given
  const std::shared_ptr<PublicKey> testPublicKey = getTestPublicKeyFromString();
  const std::string testPublicKeyAsString = testPublicKey->toStringDer();
  const std::unique_ptr<proto::Key> testProtobufPublicKey = testPublicKey->toProtobuf();

  // When
  std::shared_ptr<PublicKey> publicKey = PublicKey::fromProtobuf(*testProtobufPublicKey);

  // Then
  ASSERT_NE(publicKey, nullptr);
  EXPECT_EQ(publicKey->toStringDer(), testPublicKeyAsString);
}

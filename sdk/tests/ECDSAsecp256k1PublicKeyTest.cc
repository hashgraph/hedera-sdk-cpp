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
    ECDSAsecp256k1PublicKey::fromString(mPublicKeyFromPrivate->toString());
  const std::shared_ptr<PublicKey> mPublicKeyFromProtobuf =
    PublicKey::fromProtobuf(*mPublicKeyFromString->toProtobuf());
};

TEST_F(ECDSAsecp256k1PublicKeyTest, CopyAndMoveConstructors)
{
  ECDSAsecp256k1PublicKey copiedPublicKey(*dynamic_cast<ECDSAsecp256k1PublicKey*>(getTestPublicKeyFromPrivate().get()));
  EXPECT_EQ(copiedPublicKey.toString(), getTestPublicKeyFromPrivate()->toString());

  copiedPublicKey = *dynamic_cast<ECDSAsecp256k1PublicKey*>(getTestPublicKeyFromString().get());
  EXPECT_EQ(copiedPublicKey.toString(), getTestPublicKeyFromString()->toString());

  ECDSAsecp256k1PublicKey movedPublicKey(std::move(copiedPublicKey));
  EXPECT_EQ(movedPublicKey.toString(), getTestPublicKeyFromString()->toString());

  copiedPublicKey = std::move(movedPublicKey);
  EXPECT_EQ(copiedPublicKey.toString(), getTestPublicKeyFromString()->toString());
}

TEST_F(ECDSAsecp256k1PublicKeyTest, ToString)
{
  const std::string derEncodingFromPrivate = getTestPublicKeyFromPrivate()->toString();
  const std::string derEncodingFromLoaded = getTestPublicKeyFromString()->toString();
  const std::string derEncodingFromProtobuf = getTestPublicKeyFromProtobuf()->toString();

  EXPECT_EQ(derEncodingFromPrivate.size(), 66);
  EXPECT_EQ(derEncodingFromLoaded.size(), 66);
  EXPECT_EQ(derEncodingFromProtobuf.size(), 66);

  EXPECT_EQ(derEncodingFromPrivate, derEncodingFromLoaded);
  EXPECT_EQ(derEncodingFromPrivate, derEncodingFromProtobuf);
}

TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyValidSignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign(bytesToSign);

  EXPECT_TRUE(getTestPublicKeyFromPrivate()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromString()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromProtobuf()->verifySignature(signature, bytesToSign));
}

TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyValidSignatureOfEmptyMessage)
{
  const std::vector<unsigned char> bytesToSign;
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign(bytesToSign);

  EXPECT_TRUE(getTestPublicKeyFromPrivate()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromString()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromProtobuf()->verifySignature(signature, bytesToSign));
}

TEST_F(ECDSAsecp256k1PublicKeyTest, VerifySignatureAgainstModifiedBytes)
{
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign({ 0x1, 0x2, 0x3 });
  const std::vector<unsigned char> modifiedBytes = { 0x1, 0x2, 0x3, 0x4 };

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature(signature, modifiedBytes));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature(signature, modifiedBytes));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature(signature, modifiedBytes));
}

TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyArbitrarySignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> arbitrarySignature = { 0x1, 0x2, 0x3, 0x4 };

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature(arbitrarySignature, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature(arbitrarySignature, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature(arbitrarySignature, bytesToSign));
}

TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyEmptySignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> emptySignature;

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature(emptySignature, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature(emptySignature, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature(emptySignature, bytesToSign));
}

TEST_F(ECDSAsecp256k1PublicKeyTest, VerifyEmptyMessage)
{
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign({ 0x1, 0x2, 0x3 });
  const std::vector<unsigned char> emptyMessage;

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature(signature, emptyMessage));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature(signature, emptyMessage));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature(signature, emptyMessage));
}

TEST_F(ECDSAsecp256k1PublicKeyTest, FromString)
{
  const std::string publicKeyUncompressed = "045B36E22D710E79646F1A86D633EB38343BFE9DF39185EC730B1E7DFA79EE92CFD8C980"
                                            "B4FB4DC5493A0EE40A85543FFC49E3CDC65E0B8B8C8A8AB64A00D9B5BE";
  const std::string publicKeyCompressed = "025B36E22D710E79646F1A86D633EB38343BFE9DF39185EC730B1E7DFA79EE92CF";

  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromUncompressed =
    ECDSAsecp256k1PublicKey::fromString(publicKeyUncompressed);
  const std::shared_ptr<ECDSAsecp256k1PublicKey> publicKeyFromCompressed =
    ECDSAsecp256k1PublicKey::fromString(publicKeyCompressed);

  EXPECT_NE(publicKeyFromUncompressed, nullptr);
  EXPECT_NE(publicKeyFromCompressed, nullptr);
  EXPECT_EQ(publicKeyFromUncompressed->toString(), publicKeyFromCompressed->toString());
}

// Tests serialization of Hedera::ECDSAsecp256k1PublicKey -> proto::Key.
TEST_F(ECDSAsecp256k1PublicKeyTest, EcdsaSecp256K1PublicKeyToProtobuf)
{
  // Given
  const std::shared_ptr<PublicKey> testPublicKey = getTestPublicKeyFromString();

  // When
  const std::unique_ptr<proto::Key> protobufEcdsaSecp256K1PublicKey = testPublicKey->toProtobuf();

  // Then
  EXPECT_NE(protobufEcdsaSecp256K1PublicKey, nullptr);
  EXPECT_TRUE(protobufEcdsaSecp256K1PublicKey->has_ecdsa_secp256k1());
}

// Tests deserialization of proto::Key -> Hedera::PublicKey.
TEST_F(ECDSAsecp256k1PublicKeyTest, PublicKeyFromProtobuf)
{
  // Given
  const std::shared_ptr<PublicKey> testPublicKey = getTestPublicKeyFromString();
  const std::string testPublicKeyAsString = testPublicKey->toString();
  const std::unique_ptr<proto::Key> testProtobufPublicKey = testPublicKey->toProtobuf();

  // When
  std::shared_ptr<PublicKey> publicKey = PublicKey::fromProtobuf(*testProtobufPublicKey);

  // Then
  EXPECT_NE(publicKey, nullptr);
  EXPECT_EQ(publicKey->toString(), testPublicKeyAsString);
}

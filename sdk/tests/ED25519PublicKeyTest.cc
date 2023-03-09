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
#include "ED25519PrivateKey.h"
#include "exceptions/BadKeyException.h"
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <memory>
#include <proto/basic_types.pb.h>
#include <vector>

using namespace Hedera;

class ED25519PublicKeyTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::unique_ptr<ED25519PrivateKey>& getTestPrivateKey() const { return mPrivateKey; }
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
  const std::unique_ptr<ED25519PrivateKey> mPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> mPublicKeyFromPrivate = mPrivateKey->getPublicKey();
  const std::shared_ptr<PublicKey> mPublicKeyFromString =
    ED25519PublicKey::fromString(mPublicKeyFromPrivate->toStringDer());
  const std::shared_ptr<PublicKey> mPublicKeyFromProtobuf =
    PublicKey::fromProtobuf(*mPublicKeyFromString->toProtobuf());
};

//-----
TEST_F(ED25519PublicKeyTest, FromString)
{
  const std::string publicKeyString = "F83DEF42411E046461D5AEEAE9311C56F6612557F349F3412DBD95C9FE8B0265";
  const std::string publicKeyStringDer = ED25519PublicKey::DER_ENCODED_PREFIX_HEX + publicKeyString;

  const std::shared_ptr<ED25519PublicKey> publicKeyFromString = ED25519PublicKey::fromString(publicKeyString);

  ASSERT_NE(publicKeyFromString, nullptr);
  EXPECT_EQ(publicKeyFromString->toStringDer(), publicKeyStringDer);
  EXPECT_EQ(publicKeyFromString->toStringRaw(), publicKeyString);
  EXPECT_EQ(publicKeyFromString->toBytesDer(), internal::HexConverter::hexToBytes(publicKeyStringDer));
  EXPECT_EQ(publicKeyFromString->toBytesRaw(), internal::HexConverter::hexToBytes(publicKeyString));

  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> publicKeyFromStringDer =
                 ED25519PublicKey::fromStringDer(publicKeyString),
               BadKeyException);

  const std::shared_ptr<ED25519PublicKey> publicKeyFromStringDer = ED25519PublicKey::fromStringDer(publicKeyStringDer);

  ASSERT_NE(publicKeyFromStringDer, nullptr);
  EXPECT_EQ(publicKeyFromStringDer->toStringDer(), publicKeyFromString->toStringDer());
  EXPECT_EQ(publicKeyFromStringDer->toStringRaw(), publicKeyFromString->toStringRaw());
  EXPECT_EQ(publicKeyFromStringDer->toBytesDer(), publicKeyFromString->toBytesDer());
  EXPECT_EQ(publicKeyFromStringDer->toBytesRaw(), publicKeyFromString->toBytesRaw());

  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> publicKeyFromStringRaw =
                 ED25519PublicKey::fromStringRaw(publicKeyStringDer),
               BadKeyException);

  const std::shared_ptr<ED25519PublicKey> publicKeyFromStringRaw = ED25519PublicKey::fromStringRaw(publicKeyString);

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
  const std::vector<unsigned char> publicKeyBytes = { 0xF8, 0x3D, 0xEF, 0x42, 0x41, 0x1E, 0x04, 0x64, 0x61, 0xD5, 0xAE,
                                                      0xEA, 0xE9, 0x31, 0x1C, 0x56, 0xF6, 0x61, 0x25, 0x57, 0xF3, 0x49,
                                                      0xF3, 0x41, 0x2D, 0xBD, 0x95, 0xC9, 0xFE, 0x8B, 0x02, 0x65 };
  const std::vector<unsigned char> publicKeyBytesDer =
    internal::Utilities::concatenateVectors(ED25519PublicKey::DER_ENCODED_PREFIX_BYTES, publicKeyBytes);

  const std::shared_ptr<ED25519PublicKey> publicKeyFromBytes = ED25519PublicKey::fromBytes(publicKeyBytes);

  ASSERT_NE(publicKeyFromBytes, nullptr);
  EXPECT_EQ(publicKeyFromBytes->toStringDer(), internal::HexConverter::bytesToHex(publicKeyBytesDer));
  EXPECT_EQ(publicKeyFromBytes->toStringRaw(), internal::HexConverter::bytesToHex(publicKeyBytes));
  EXPECT_EQ(publicKeyFromBytes->toBytesDer(), publicKeyBytesDer);
  EXPECT_EQ(publicKeyFromBytes->toBytesRaw(), publicKeyBytes);

  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> publicKeyFromBytesDer =
                 ED25519PublicKey::fromBytesDer(publicKeyBytes),
               BadKeyException);

  const std::shared_ptr<ED25519PublicKey> publicKeyFromBytesDer = ED25519PublicKey::fromBytesDer(publicKeyBytesDer);

  ASSERT_NE(publicKeyFromBytesDer, nullptr);
  EXPECT_EQ(publicKeyFromBytesDer->toStringDer(), publicKeyFromBytes->toStringDer());
  EXPECT_EQ(publicKeyFromBytesDer->toStringRaw(), publicKeyFromBytes->toStringRaw());
  EXPECT_EQ(publicKeyFromBytesDer->toBytesDer(), publicKeyFromBytes->toBytesDer());
  EXPECT_EQ(publicKeyFromBytesDer->toBytesRaw(), publicKeyFromBytes->toBytesRaw());

  EXPECT_THROW(const std::shared_ptr<ED25519PublicKey> publicKeyFromBytesRaw =
                 ED25519PublicKey::fromBytesRaw(publicKeyBytesDer),
               BadKeyException);

  const std::shared_ptr<ED25519PublicKey> publicKeyFromBytesRaw = ED25519PublicKey::fromBytesRaw(publicKeyBytes);

  ASSERT_NE(publicKeyFromBytesRaw, nullptr);
  EXPECT_EQ(publicKeyFromBytesRaw->toStringDer(), publicKeyFromBytesDer->toStringDer());
  EXPECT_EQ(publicKeyFromBytesRaw->toStringRaw(), publicKeyFromBytesDer->toStringRaw());
  EXPECT_EQ(publicKeyFromBytesRaw->toBytesDer(), publicKeyFromBytesDer->toBytesDer());
  EXPECT_EQ(publicKeyFromBytesRaw->toBytesRaw(), publicKeyFromBytesDer->toBytesRaw());
}

//-----
TEST_F(ED25519PublicKeyTest, Clone)
{
  const std::shared_ptr<PublicKey> publicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();
  ASSERT_NE(publicKey, nullptr);

  const std::unique_ptr<PublicKey> clonedPublicKey = publicKey->clone();
  ASSERT_NE(clonedPublicKey, nullptr);

  EXPECT_EQ(publicKey->toStringDer(), clonedPublicKey->toStringDer());
  EXPECT_EQ(publicKey->toStringRaw(), clonedPublicKey->toStringRaw());
  EXPECT_EQ(publicKey->toBytesDer(), clonedPublicKey->toBytesDer());
  EXPECT_EQ(publicKey->toBytesRaw(), clonedPublicKey->toBytesRaw());
}

//-----
TEST_F(ED25519PublicKeyTest, VerifyValidSignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign(bytesToSign);

  EXPECT_TRUE(getTestPublicKeyFromPrivate()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromString()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromProtobuf()->verifySignature(signature, bytesToSign));
}

//-----
TEST_F(ED25519PublicKeyTest, VerifyValidSignatureOfEmptyMessage)
{
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign({});

  EXPECT_TRUE(getTestPublicKeyFromPrivate()->verifySignature(signature, {}));
  EXPECT_TRUE(getTestPublicKeyFromString()->verifySignature(signature, {}));
  EXPECT_TRUE(getTestPublicKeyFromProtobuf()->verifySignature(signature, {}));
}

//-----
TEST_F(ED25519PublicKeyTest, VerifySignatureAgainstModifiedBytes)
{
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign({ 0x1, 0x2, 0x3 });
  const std::vector<unsigned char> modifiedBytes = { 0x1, 0x2, 0x3, 0x4 };

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature(signature, modifiedBytes));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature(signature, modifiedBytes));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature(signature, modifiedBytes));
}

//-----
TEST_F(ED25519PublicKeyTest, VerifyArbitrarySignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> arbitrarySignature = { 0x1, 0x2, 0x3, 0x4 };

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature(arbitrarySignature, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature(arbitrarySignature, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature(arbitrarySignature, bytesToSign));
}

//-----
TEST_F(ED25519PublicKeyTest, VerifyEmptySignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature({}, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature({}, bytesToSign));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature({}, bytesToSign));
}

//-----
TEST_F(ED25519PublicKeyTest, VerifyEmptyMessage)
{
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign({ 0x1, 0x2, 0x3 });

  EXPECT_FALSE(getTestPublicKeyFromPrivate()->verifySignature(signature, {}));
  EXPECT_FALSE(getTestPublicKeyFromString()->verifySignature(signature, {}));
  EXPECT_FALSE(getTestPublicKeyFromProtobuf()->verifySignature(signature, {}));
}

//-----
TEST_F(ED25519PublicKeyTest, ToString)
{
  const std::string derEncodingFromPrivate = getTestPublicKeyFromPrivate()->toStringDer();
  const std::string derEncodingFromLoaded = getTestPublicKeyFromString()->toStringDer();
  const std::string derEncodingFromProtobuf = getTestPublicKeyFromProtobuf()->toStringDer();

  EXPECT_EQ(derEncodingFromPrivate.size(),
            ED25519PublicKey::PUBLIC_KEY_SIZE * 2 + ED25519PublicKey::DER_ENCODED_PREFIX_HEX.size());
  EXPECT_EQ(derEncodingFromLoaded.size(),
            ED25519PublicKey::PUBLIC_KEY_SIZE * 2 + ED25519PublicKey::DER_ENCODED_PREFIX_HEX.size());
  EXPECT_EQ(derEncodingFromProtobuf.size(),
            ED25519PublicKey::PUBLIC_KEY_SIZE * 2 + ED25519PublicKey::DER_ENCODED_PREFIX_HEX.size());

  EXPECT_EQ(derEncodingFromPrivate, derEncodingFromLoaded);
  EXPECT_EQ(derEncodingFromPrivate, derEncodingFromProtobuf);

  const std::string rawEncodingFromPrivate = getTestPublicKeyFromPrivate()->toStringRaw();
  const std::string rawEncodingFromLoaded = getTestPublicKeyFromString()->toStringRaw();
  const std::string rawEncodingFromProtobuf = getTestPublicKeyFromProtobuf()->toStringRaw();

  EXPECT_EQ(rawEncodingFromPrivate.size(), ED25519PublicKey::PUBLIC_KEY_SIZE * 2);
  EXPECT_EQ(rawEncodingFromLoaded.size(), ED25519PublicKey::PUBLIC_KEY_SIZE * 2);
  EXPECT_EQ(rawEncodingFromProtobuf.size(), ED25519PublicKey::PUBLIC_KEY_SIZE * 2);

  EXPECT_EQ(rawEncodingFromPrivate, rawEncodingFromLoaded);
  EXPECT_EQ(rawEncodingFromPrivate, rawEncodingFromProtobuf);
}

//-----
TEST_F(ED25519PublicKeyTest, PublicKeyToProtobuf)
{
  // Given
  const std::shared_ptr<PublicKey> testPublicKey = getTestPublicKeyFromString();

  // When
  const std::unique_ptr<proto::Key> protobufEd25519PublicKey = testPublicKey->toProtobuf();
  const std::vector<unsigned char> protobufEd25519PublicKeyBytes = { protobufEd25519PublicKey->ed25519().cbegin(),
                                                                     protobufEd25519PublicKey->ed25519().cend() };

  // Then
  ASSERT_NE(protobufEd25519PublicKey, nullptr);
  EXPECT_TRUE(protobufEd25519PublicKey->has_ed25519());
  EXPECT_EQ(testPublicKey->toBytesRaw(), protobufEd25519PublicKeyBytes);
}

//-----
TEST_F(ED25519PublicKeyTest, PublicKeyFromProtobuf)
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

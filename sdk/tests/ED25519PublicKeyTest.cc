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
    ED25519PublicKey::fromString(mPublicKeyFromPrivate->toString());
  const std::shared_ptr<PublicKey> mPublicKeyFromProtobuf =
    PublicKey::fromProtobuf(*mPublicKeyFromString->toProtobuf());
};

TEST_F(ED25519PublicKeyTest, CopyAndMoveConstructors)
{
  ED25519PublicKey copiedPublicKey(*dynamic_cast<ED25519PublicKey*>(getTestPublicKeyFromPrivate().get()));
  EXPECT_EQ(copiedPublicKey.toString(), getTestPublicKeyFromPrivate()->toString());

  copiedPublicKey = *dynamic_cast<ED25519PublicKey*>(getTestPublicKeyFromString().get());
  EXPECT_EQ(copiedPublicKey.toString(), getTestPublicKeyFromString()->toString());

  ED25519PublicKey movedPublicKey(std::move(copiedPublicKey));
  EXPECT_EQ(movedPublicKey.toString(), getTestPublicKeyFromString()->toString());

  copiedPublicKey = std::move(movedPublicKey);
  EXPECT_EQ(copiedPublicKey.toString(), getTestPublicKeyFromString()->toString());
}

TEST_F(ED25519PublicKeyTest, ToString)
{
  const std::string derEncodingFromPrivate = getTestPublicKeyFromPrivate()->toString();
  const std::string derEncodingFromLoaded = getTestPublicKeyFromString()->toString();
  const std::string derEncodingFromProtobuf = getTestPublicKeyFromProtobuf()->toString();

  EXPECT_EQ(derEncodingFromPrivate.size(), 64);
  EXPECT_EQ(derEncodingFromLoaded.size(), 64);
  EXPECT_EQ(derEncodingFromProtobuf.size(), 64);

  EXPECT_EQ(derEncodingFromPrivate, derEncodingFromLoaded);
  EXPECT_EQ(derEncodingFromPrivate, derEncodingFromProtobuf);
}

// Tests serialization of Hedera::ED25519PublicKey -> proto::Key.
TEST_F(ED25519PublicKeyTest, ED25519PublicKeyToProtobuf)
{
  // Given
  const std::shared_ptr<PublicKey> testPublicKey = getTestPublicKeyFromPrivate();
  ED25519PublicKey copiedPublicKey(*dynamic_cast<ED25519PublicKey*>(testPublicKey.get()));

  // When
  const std::shared_ptr<proto::Key> protoPublicKey = copiedPublicKey.toProtobuf();

  // Then
  EXPECT_NE(protoPublicKey, nullptr);
  EXPECT_TRUE(protoPublicKey->has_ed25519());
  EXPECT_GT(protoPublicKey->ByteSize(), 0);
}

TEST_F(ED25519PublicKeyTest, VerifyValidSignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign(bytesToSign);

  EXPECT_TRUE(getTestPublicKeyFromPrivate()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromString()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromProtobuf()->verifySignature(signature, bytesToSign));
}

TEST_F(ED25519PublicKeyTest, VerifyValidSignatureOfEmptyMessage)
{
  const std::vector<unsigned char> bytesToSign;
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign(bytesToSign);

  EXPECT_TRUE(getTestPublicKeyFromPrivate()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromString()->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(getTestPublicKeyFromProtobuf()->verifySignature(signature, bytesToSign));
}

TEST_F(ED25519PublicKeyTest, VerifySignatureAgainstModifiedBytes)
{
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign({ 0x1, 0x2, 0x3 });
  const std::vector<unsigned char> modifiedBytes = { 0x1, 0x2, 0x3, 0x4 };

  EXPECT_THROW(getTestPublicKeyFromPrivate()->verifySignature(signature, modifiedBytes), std::runtime_error);
  EXPECT_THROW(getTestPublicKeyFromString()->verifySignature(signature, modifiedBytes), std::runtime_error);
  EXPECT_THROW(getTestPublicKeyFromProtobuf()->verifySignature(signature, modifiedBytes), std::runtime_error);
}

TEST_F(ED25519PublicKeyTest, VerifyArbitrarySignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> arbitrarySignature = { 0x1, 0x2, 0x3, 0x4 };

  EXPECT_THROW(getTestPublicKeyFromPrivate()->verifySignature(arbitrarySignature, bytesToSign), std::runtime_error);
  EXPECT_THROW(getTestPublicKeyFromString()->verifySignature(arbitrarySignature, bytesToSign), std::runtime_error);
  EXPECT_THROW(getTestPublicKeyFromProtobuf()->verifySignature(arbitrarySignature, bytesToSign), std::runtime_error);
}

TEST_F(ED25519PublicKeyTest, VerifyEmptySignature)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> emptySignature;

  EXPECT_THROW(getTestPublicKeyFromPrivate()->verifySignature(emptySignature, bytesToSign), std::runtime_error);
  EXPECT_THROW(getTestPublicKeyFromString()->verifySignature(emptySignature, bytesToSign), std::runtime_error);
  EXPECT_THROW(getTestPublicKeyFromProtobuf()->verifySignature(emptySignature, bytesToSign), std::runtime_error);
}

TEST_F(ED25519PublicKeyTest, VerifyEmptyMessage)
{
  const std::vector<unsigned char> signature = getTestPrivateKey()->sign({ 0x1, 0x2, 0x3 });
  const std::vector<unsigned char> emptyMessage;

  EXPECT_THROW(getTestPublicKeyFromPrivate()->verifySignature(signature, emptyMessage), std::runtime_error);
  EXPECT_THROW(getTestPublicKeyFromString()->verifySignature(signature, emptyMessage), std::runtime_error);
  EXPECT_THROW(getTestPublicKeyFromProtobuf()->verifySignature(signature, emptyMessage), std::runtime_error);
}

TEST_F(ED25519PublicKeyTest, FromString)
{
  // These are 2 versions of the same public key. The first conforms to the full RFC 8410 standard, the second is just
  // the public key.
  const std::string publicKeyStringExtended =
    "302A300506032B6570032100F83DEF42411E046461D5AEEAE9311C56F6612557F349F3412DBD95C9FE8B0265";
  const std::string publicKeyStringShort = "F83DEF42411E046461D5AEEAE9311C56F6612557F349F3412DBD95C9FE8B0265";

  const std::shared_ptr<ED25519PublicKey> publicKeyFromExtended = ED25519PublicKey::fromString(publicKeyStringExtended);
  const std::shared_ptr<ED25519PublicKey> publicKeyFromShort = ED25519PublicKey::fromString(publicKeyStringShort);

  EXPECT_NE(publicKeyFromExtended, nullptr);
  EXPECT_NE(publicKeyFromShort, nullptr);
  EXPECT_EQ(publicKeyFromExtended->toString(), publicKeyFromShort->toString());
}

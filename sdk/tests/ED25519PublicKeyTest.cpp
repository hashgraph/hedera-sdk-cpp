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

#include <gtest/gtest.h>
#include <memory>
#include <vector>

#include "helper/HexConverter.h"

#include "ED25519PrivateKey.h"
#include "ED25519PublicKey.h"

class ED25519PublicKeyTest : public ::testing::Test {
protected:
  std::shared_ptr<Hedera::ED25519PrivateKey> privateKey;
  std::shared_ptr<Hedera::PublicKey> publicKeyFromPrivate;
  std::shared_ptr<Hedera::ED25519PublicKey> publicKeyLoaded;

  void SetUp() override {
    // generate a private key
    privateKey = Hedera::ED25519PrivateKey::generatePrivateKey();

    publicKeyFromPrivate = privateKey->getPublicKey();
    publicKeyLoaded = Hedera::ED25519PublicKey::fromDEREncoding(
        publicKeyFromPrivate->toString());
  }
};

TEST_F(ED25519PublicKeyTest, ToString) {
  std::string derEncodingFromPrivate = publicKeyFromPrivate->toString();
  std::string derEncodingFromLoaded = publicKeyLoaded->toString();

  EXPECT_EQ(derEncodingFromPrivate.size(), 44);
  EXPECT_EQ(derEncodingFromLoaded.size(), 44);
  EXPECT_EQ(derEncodingFromPrivate, derEncodingFromLoaded);
}

TEST_F(ED25519PublicKeyTest, VerifyValidSignature) {
  std::vector<unsigned char> bytesToSign = {0x1, 0x2, 0x3};
  std::vector<unsigned char> signature = privateKey->sign(bytesToSign);

  EXPECT_TRUE(publicKeyFromPrivate->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(publicKeyLoaded->verifySignature(signature, bytesToSign));
}

TEST_F(ED25519PublicKeyTest, VerifyValidSignatureOfEmptyMessage) {
  std::vector<unsigned char> bytesToSign = std::vector<unsigned char>();
  std::vector<unsigned char> signature = privateKey->sign(bytesToSign);

  EXPECT_TRUE(publicKeyFromPrivate->verifySignature(signature, bytesToSign));
  EXPECT_TRUE(publicKeyLoaded->verifySignature(signature, bytesToSign));
}

TEST_F(ED25519PublicKeyTest, VerifySignatureAgainstModifiedBytes) {
  std::vector<unsigned char> signature = privateKey->sign({0x1, 0x2, 0x3});

  std::vector<unsigned char> modifiedBytes = {0x1, 0x2, 0x3, 0x4};

  EXPECT_FALSE(
      publicKeyFromPrivate->verifySignature(signature, modifiedBytes));
  EXPECT_FALSE(publicKeyLoaded->verifySignature(signature, modifiedBytes));
}

TEST_F(ED25519PublicKeyTest, VerifyArbitrarySignature) {
  std::vector<unsigned char> bytesToSign = {0x1, 0x2, 0x3};
  std::vector<unsigned char> arbitrarySignature = {0x1, 0x2, 0x3, 0x4};

  EXPECT_FALSE(
      publicKeyFromPrivate->verifySignature(arbitrarySignature, bytesToSign));
  EXPECT_FALSE(publicKeyLoaded->verifySignature(arbitrarySignature, bytesToSign));
}

TEST_F(ED25519PublicKeyTest, VerifyEmptySignature) {
  std::vector<unsigned char> bytesToSign = {0x1, 0x2, 0x3};
  std::vector<unsigned char> emptySignature = std::vector<unsigned char>();

  EXPECT_FALSE(
      publicKeyFromPrivate->verifySignature(emptySignature, bytesToSign));
  EXPECT_FALSE(publicKeyLoaded->verifySignature(emptySignature, bytesToSign));
}

TEST_F(ED25519PublicKeyTest, VerifyEmptyMessage) {
  std::vector<unsigned char> signature = privateKey->sign({0x1, 0x2, 0x3});
  std::vector<unsigned char> emptyMessage = std::vector<unsigned char>();

  EXPECT_FALSE(
      publicKeyFromPrivate->verifySignature(signature, emptyMessage));
  EXPECT_FALSE(publicKeyLoaded->verifySignature(signature, emptyMessage));
}

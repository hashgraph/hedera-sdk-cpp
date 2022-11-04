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

#include "ED25519PrivateKey.h"
#include "ED25519PublicKey.h"

class ED25519PrivateKeyTest : public ::testing::Test {
protected:
  std::shared_ptr<Hedera::ED25519PrivateKey> privateKeyGenerated;
  std::shared_ptr<Hedera::ED25519PrivateKey> privateKeyLoaded;

  void SetUp() override {
    // generate a private key
    privateKeyGenerated = Hedera::ED25519PrivateKey::generatePrivateKey();

    // serialize and then load private key back in
    privateKeyLoaded =
        Hedera::ED25519PrivateKey::fromString(privateKeyGenerated->toString());
  }
};

TEST_F(ED25519PrivateKeyTest, GetPublicKey) {
  // get the public keys from the private keys
  std::shared_ptr<Hedera::PublicKey> publicFromGenerated =
      privateKeyGenerated->getPublicKey();
  std::shared_ptr<Hedera::PublicKey> publicFromLoaded =
      privateKeyLoaded->getPublicKey();

  EXPECT_NE(publicFromGenerated, nullptr);
  EXPECT_NE(publicFromLoaded, nullptr);

  // make sure returned public keys are the same for generated or loaded private
  // keys
  EXPECT_EQ(publicFromGenerated->toString(), publicFromLoaded->toString());
}

TEST_F(ED25519PrivateKeyTest, Sign) {
  std::vector<unsigned char> bytesToSign = {0x1, 0x2, 0x3};
  std::vector<unsigned char> signatureFromGenerated =
      privateKeyGenerated->sign(bytesToSign);
  std::vector<unsigned char> signatureFromLoaded =
      privateKeyLoaded->sign(bytesToSign);

  EXPECT_EQ(signatureFromGenerated.size(), 64);
  EXPECT_EQ(signatureFromLoaded.size(), 64);
  EXPECT_EQ(signatureFromGenerated, signatureFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, SignEmptyBytes) {
  std::vector<unsigned char> bytesToSign = std::vector<unsigned char>();

  std::vector<unsigned char> signatureFromGenerated =
      privateKeyGenerated->sign(bytesToSign);
  std::vector<unsigned char> signatureFromLoaded =
      privateKeyLoaded->sign(bytesToSign);

  EXPECT_EQ(signatureFromGenerated.size(), 64);
  EXPECT_EQ(signatureFromLoaded.size(), 64);
  EXPECT_EQ(signatureFromGenerated, signatureFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, ToString) {
  std::string stringFromGenerated = privateKeyGenerated->toString();
  std::string stringFromLoaded = privateKeyLoaded->toString();

  EXPECT_EQ(stringFromGenerated.size(), 48);
  EXPECT_EQ(stringFromLoaded.size(), 48);
  EXPECT_EQ(stringFromGenerated, stringFromLoaded);
}

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

TEST(tests, ED25519PrivateKey_getPublicKey) {
  std::unique_ptr<Hedera::ED25519PrivateKey> generatedPrivateKey =
      std::make_unique<Hedera::ED25519PrivateKey>();

  // serialize and then load private key back in, to make sure nothing is
  // changed
  std::string privateKeyString = generatedPrivateKey->toString();
  std::unique_ptr<Hedera::ED25519PrivateKey> loadedPrivateKey =
      std::make_unique<Hedera::ED25519PrivateKey>(privateKeyString);

  // get the public keys from the private keys
  std::shared_ptr<Hedera::PublicKey> publicFromGenerated =
      generatedPrivateKey->getPublicKey();
  std::shared_ptr<Hedera::PublicKey> publicFromLoaded =
      loadedPrivateKey->getPublicKey();

  EXPECT_NE(publicFromGenerated, nullptr);
  EXPECT_NE(publicFromLoaded, nullptr);

  // make sure returned public keys are the same for generated or loaded private
  // keys
  EXPECT_EQ(publicFromGenerated->toString(), publicFromLoaded->toString());
}

TEST(tests, ED25519PrivateKey_sign) {
  std::unique_ptr<Hedera::ED25519PrivateKey> privateKey =
      std::make_unique<Hedera::ED25519PrivateKey>();

  std::vector<unsigned char> bytesToSign = {0x1, 0x2, 0x3};
  std::vector<unsigned char> signature = privateKey->sign(bytesToSign);

  EXPECT_EQ(signature.size(), 64);
}

TEST(tests, ED25519PrivateKey_signEmptyBytes) {
  std::unique_ptr<Hedera::ED25519PrivateKey> privateKey =
      std::make_unique<Hedera::ED25519PrivateKey>();

  std::vector<unsigned char> signature =
      privateKey->sign(std::vector<unsigned char>());

  EXPECT_EQ(signature.size(), 64);
}

TEST(tests, ED25519PrivateKey_toString) {
  std::unique_ptr<Hedera::ED25519PrivateKey> generatedPrivateKey =
      std::make_unique<Hedera::ED25519PrivateKey>();

  std::string generatedPrivateKeyString = generatedPrivateKey->toString();

  std::unique_ptr<Hedera::ED25519PrivateKey> loadedPrivateKey =
      std::make_unique<Hedera::ED25519PrivateKey>(generatedPrivateKeyString);

  std::string loadedPrivateKeyString = loadedPrivateKey->toString();

  EXPECT_EQ(generatedPrivateKeyString.size(), 32);
  EXPECT_EQ(loadedPrivateKeyString.size(), 32);
  EXPECT_EQ(generatedPrivateKey->toString(), loadedPrivateKey->toString());
}

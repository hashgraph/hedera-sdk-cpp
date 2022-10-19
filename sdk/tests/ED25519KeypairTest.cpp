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

#include "ED25519Keypair.h"
#include "ED25519PublicKey.h"

TEST(tests, getPublicKey) {
  std::unique_ptr<Hedera::ED25519Keypair> keypair =
      std::make_unique<Hedera::ED25519Keypair>();

  std::shared_ptr<Hedera::PublicKey> publicKey = keypair->getPublicKey();
  EXPECT_NE(publicKey, nullptr);
}

TEST(tests, sign) {
  std::unique_ptr<Hedera::ED25519Keypair> keypair =
      std::make_unique<Hedera::ED25519Keypair>();

  std::vector<unsigned char> bytesToSign = {0x1, 0x2, 0x3};
  std::vector<unsigned char> signature = keypair->sign(bytesToSign);

  EXPECT_EQ(signature.size(), 64);
}

TEST(tests, signEmptyBytes) {
  std::unique_ptr<Hedera::ED25519Keypair> keypair =
      std::make_unique<Hedera::ED25519Keypair>();

  std::vector<unsigned char> signature = keypair->sign(std::vector<unsigned char>());

  EXPECT_EQ(signature.size(), 64);
}

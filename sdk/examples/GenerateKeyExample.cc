/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
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
#include "ECDSAsecp256k1PrivateKey.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <iostream>

using namespace Hedera;

int main(int argc, char** argv)
{
  const std::unique_ptr<PrivateKey> ed25519Key = ED25519PrivateKey::generatePrivateKey();
  const std::unique_ptr<PrivateKey> ecdsaKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();

  std::cout << "Generated ED25519PrivateKey: " << ed25519Key->toStringRaw() << std::endl;
  std::cout << "Generated ED25519PublicKey: " << ed25519Key->getPublicKey()->toStringRaw() << std::endl;
  std::cout << "Generated ECDSAsecp256k1PrivateKey: " << ecdsaKey->toStringRaw() << std::endl;
  std::cout << "Generated ECDSAsecp256k1PublicKey: " << ecdsaKey->getPublicKey()->toStringRaw() << std::endl;

  return 0;
}

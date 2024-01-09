/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
  // Generate an ED25519 private
  const std::unique_ptr<PrivateKey> ed25519Key = ED25519PrivateKey::generatePrivateKey();

  // Generate an ECDSA private
  const std::unique_ptr<PrivateKey> ecdsaKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();

  // Print the ED25519 private key and public key in RAW and DER encoded formats
  std::cout << "Generated ED25519PrivateKey Raw: " << ed25519Key->toStringRaw() << std::endl;
  std::cout << "Generated ED25519PrivateKey DER encoded: " << ed25519Key->toStringDer() << std::endl;
  std::cout << "Generated ED25519PublicKey Raw: " << ed25519Key->getPublicKey()->toStringRaw() << std::endl;
  std::cout << "Generated ED25519PublicKey DER encoded: " << ed25519Key->getPublicKey()->toStringDer() << std::endl;
  std::cout << "\n";

  // Print the ECDSA private key and public key in RAW and DER encoded formats
  std::cout << "Generated ECDSAsecp256k1PrivateKey Raw: " << ecdsaKey->toStringRaw() << std::endl;
  std::cout << "Generated ECDSAsecp256k1PrivateKey DER encoded: " << ecdsaKey->toStringDer() << std::endl;
  std::cout << "Generated ECDSAsecp256k1PublicKey Raw: " << ecdsaKey->getPublicKey()->toStringRaw() << std::endl;
  std::cout << "Generated ECDSAsecp256k1PublicKey DER encoded: " << ecdsaKey->getPublicKey()->toStringDer()
            << std::endl;

  return 0;
}

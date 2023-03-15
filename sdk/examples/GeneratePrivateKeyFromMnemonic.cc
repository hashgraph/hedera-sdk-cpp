/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "MnemonicBIP39.h"

#include <iostream>
#include <string>

using namespace Hedera;

int main(int argc, char** argv)
{
  // Grab the passphrase from the command line if input
  std::string passphrase = "passphrase";
  if (argc > 1)
  {
    passphrase = argv[1];
  }

  // Generate and print a 12-word BIP39 mnemonic
  MnemonicBIP39 mnemonicBip39 = MnemonicBIP39::generate12WordBIP39Mnemonic();
  std::cout << "Generated 12-word MnemonicBIP39: " << mnemonicBip39.toString() << std::endl;

  // Generate and print a ED25519PrivateKey and ECDSAsecp256k1PrivateKey from the mnemonic with no passphrase
  std::unique_ptr<PrivateKey> ed25519PrivateKey = mnemonicBip39.toStandardEd25519PrivateKey();
  std::unique_ptr<PrivateKey> ecdsaSecp256k1PrivateKey = mnemonicBip39.toStandardECDSAsecp256k1PrivateKey();
  std::cout << "Generated ED25519PrivateKey from mnemonic with no passphrase: " << ed25519PrivateKey->toStringRaw()
            << std::endl;
  std::cout << "Generated ECDSAsecp256k1PrivateKey from mnemonic with no passphrase: "
            << ecdsaSecp256k1PrivateKey->toStringRaw() << std::endl;

  // Generate and print a ED25519PrivateKey and ECDSAsecp256k1PrivateKey from the mnemonic with a passphrase
  ed25519PrivateKey = mnemonicBip39.toStandardEd25519PrivateKey(passphrase);
  ecdsaSecp256k1PrivateKey = mnemonicBip39.toStandardECDSAsecp256k1PrivateKey(passphrase);
  std::cout << "Generated ED25519PrivateKey from mnemonic with passphrase '" << passphrase
            << "': " << ed25519PrivateKey->toStringRaw() << std::endl;
  std::cout << "Generated ECDSAsecp256k1PrivateKey from mnemonic with passphrase '" << passphrase
            << "': " << ecdsaSecp256k1PrivateKey->toStringRaw() << std::endl;

  // Start a new section of printing
  std::cout << std::endl;

  // Generate and print a 24-word BIP39 mnemonic
  mnemonicBip39 = MnemonicBIP39::generate24WordBIP39Mnemonic();
  std::cout << "Generated 24-word MnemonicBIP39: " << mnemonicBip39.toString() << std::endl;

  // Generate and print a ED25519PrivateKey and ECDSAsecp256k1PrivateKey from the mnemonic with no passphrase
  ed25519PrivateKey = mnemonicBip39.toStandardEd25519PrivateKey();
  ecdsaSecp256k1PrivateKey = mnemonicBip39.toStandardECDSAsecp256k1PrivateKey();
  std::cout << "Generated ED25519PrivateKey from mnemonic with no passphrase: " << ed25519PrivateKey->toStringRaw()
            << std::endl;
  std::cout << "Generated ECDSAsecp256k1PrivateKey from mnemonic with no passphrase: "
            << ecdsaSecp256k1PrivateKey->toStringRaw() << std::endl;

  // Generate and print a ED25519PrivateKey and ECDSAsecp256k1PrivateKey from the mnemonic with a passphrase
  ed25519PrivateKey = mnemonicBip39.toStandardEd25519PrivateKey(passphrase);
  ecdsaSecp256k1PrivateKey = mnemonicBip39.toStandardECDSAsecp256k1PrivateKey(passphrase);
  std::cout << "Generated ED25519PrivateKey from mnemonic with passphrase '" << passphrase
            << "': " << ed25519PrivateKey->toStringRaw() << std::endl;
  std::cout << "Generated ECDSAsecp256k1PrivateKey from mnemonic with passphrase '" << passphrase
            << "': " << ecdsaSecp256k1PrivateKey->toStringRaw() << std::endl;

  return 0;
}

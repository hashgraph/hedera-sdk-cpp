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

#ifndef HEDERA_SDK_CPP_MNEMONICBIP39_H
#define HEDERA_SDK_CPP_MNEMONICBIP39_H

#include "MnemonicAbstract.h"

#include <string>
#include <vector>

namespace Hedera
{

class MnemonicBIP39 : public MnemonicAbstract
{
public:
  static MnemonicBIP39 initializeBIP39Mnemonic(const std::vector<uint16_t>& wordIndices);
  static MnemonicBIP39 initializeBIP39Mnemonic(const std::vector<std::string>& words);
  static MnemonicBIP39 initializeBIP39Mnemonic(const std::string& fullMnemonic, const std::string& delimiter = " ");

  static MnemonicBIP39 generate12WordBIP39Mnemonic();
  static MnemonicBIP39 generate24WordBIP39Mnemonic();

  [[nodiscard]] std::vector<unsigned char> toSeed(const std::string& passphrase = "") const;

protected:
  [[nodiscard]] const std::vector<std::string>& getWordList() const override;

  [[nodiscard]] std::set<unsigned long> getAcceptableWordCounts() const override;

private:
  MnemonicBIP39() = default;

  static std::vector<uint16_t> entropyToWordIndices(const std::vector<unsigned char>& entropy);
};

} // Hedera

#endif // HEDERA_SDK_CPP_MNEMONICBIP39_H

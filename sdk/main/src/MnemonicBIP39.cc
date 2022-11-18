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

#include "MnemonicBIP39.h"

namespace Hedera
{
MnemonicBIP39 MnemonicBIP39::createBIP39Mnemonic(const std::vector<uint16_t>& wordIndices)
{
  MnemonicBIP39 outputMnemonic = MnemonicBIP39();
  outputMnemonic.initialize(wordIndices);

  return outputMnemonic;
}

MnemonicBIP39 MnemonicBIP39::createBIP39Mnemonic(const std::vector<std::string>& words)
{
  MnemonicBIP39 outputMnemonic = MnemonicBIP39();
  outputMnemonic.initialize(outputMnemonic.wordsToIndices(words));

  return outputMnemonic;
}

MnemonicBIP39 MnemonicBIP39::createBIP39Mnemonic(const std::string& fullMnemonic, const std::string& delimiter)
{
  MnemonicBIP39 outputMnemonic = MnemonicBIP39();
  outputMnemonic.initialize(outputMnemonic.wordsToIndices(splitMnemonicString(fullMnemonic, delimiter)));

  if (!outputMnemonic.verifyChecksum()) {
    throw std::invalid_argument("Invalid checksum");
  }

  return outputMnemonic;
}

const std::vector<std::string>& MnemonicBIP39::getWordList() const
{
  return MnemonicAbstract::bip39WordList;
}

std::set<unsigned long> MnemonicBIP39::getAcceptableWordCounts() const
{
  return std::set<unsigned long>{ 12, 24 };
}

} // Hedera
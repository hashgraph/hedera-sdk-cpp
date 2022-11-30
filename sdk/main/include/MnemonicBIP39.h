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

/**
 * A standard BIP39 mnemonic
 */
class MnemonicBIP39 : public MnemonicAbstract
{
public:
  /**
   * Initializes a mnemonic from a vector of word indices
   *
   * @param wordIndices the word indices to create the mnemonic from
   *
   * @return the new mnemonic object
   */
  static MnemonicBIP39 initializeBIP39Mnemonic(const std::vector<uint16_t>& wordIndices);

  /**
   * Initializes a mnemonic from a vector of word strings
   *
   * @param words the words to create the mnemonic from
   *
   * @return the new mnemonic object
   */
  static MnemonicBIP39 initializeBIP39Mnemonic(const std::vector<std::string>& words);

  /**
   * Initializes a mnemonic from a string containing all the words of a mnemonic phrase
   *
   * @param fullMnemonic the string to create the mnemonic from
   * @param delimiter the character delimiting the mnemonic string
   *
   * @return the new mnemonic object
   */
  static MnemonicBIP39 initializeBIP39Mnemonic(const std::string& fullMnemonic, const std::string& delimiter = " ");

  /**
   * Generates a new mnemonic phrase with 12 words
   *
   * @return the newly generated mnemonic object
   */
  static MnemonicBIP39 generate12WordBIP39Mnemonic();

  /**
   * Generates a new mnemonic phrase with 24 words
   *
   * @return the newly generated mnemonic object
   */
  static MnemonicBIP39 generate24WordBIP39Mnemonic();

  /**
   * Computes the seed that results from this mnemonic
   *
   * @param passphrase the passphrase to use in seed generation
   *
   * @return this mnemonic's seed, which the given passphrase
   */
  [[nodiscard]] std::vector<unsigned char> toSeed(const std::string& passphrase = "") const;

  /**
   * Computes the word indices that result from the input entropy
   *
   * @param entropy the entropy to compute the word indices from
   *
   * @return the word indices
   */
  static std::vector<uint16_t> entropyToWordIndices(const std::vector<unsigned char>& entropy);

protected:
  /**
   * Gets a reference to the BIP39 word list
   *
   * @return the BIP39 word list
   */
  [[nodiscard]] const std::vector<std::string>& getWordList() const override;

  /**
   * Gets the set of acceptable word counts. In the case of a BIP39 passphrase, this set will include 12 and 24
   *
   * @return the acceptable word counts
   */
  [[nodiscard]] std::set<unsigned long> getAcceptableWordCounts() const override;

private:
  /**
   * Hidden constructor
   */
  MnemonicBIP39() = default;
};

} // Hedera

#endif // HEDERA_SDK_CPP_MNEMONICBIP39_H

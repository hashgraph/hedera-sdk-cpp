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

#ifndef HEDERA_SDK_CPP_MNEMONICABSTRACT_H
#define HEDERA_SDK_CPP_MNEMONICABSTRACT_H

#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace Hedera
{

/**
 * The mnemonic represented by this class is in 1 of 3 formats
 * <p>
 * 1. The legacy v1 format. This mnemonic has 22 words from the "legacy-english.txt" word list
 * 2. The legacy v2 format. This mnemonic has 24 words from the "bip39-english.txt" word list. This format is
 * indistinguishable from the standard format: in order to correctly compute the private key from this mnemonic, the
 * user must know that they have a legacy phrase
 * 3. The standard format, as defined by BIP39. A BIP39 phrase may have either 12 or 24 words, from the
 * "bip39-english.txt" list
 */
class MnemonicAbstract
{
public:
  /**
   * Gets the string representation of the mnemonic (words separated by spaces)
   *
   * @return the string representation
   */
  [[nodiscard]] std::string toString() const;

private:
  /**
   * The words that make up this mnemonic
   */
  std::vector<uint16_t> wordIndices;

  /**
   * Reads in a word list file line by line
   *
   * @param fileName the file containing the word list
   *
   * @return a vector of strings, where each element is the string making up a line of the file
   */
  static std::vector<std::string> readWordListFromFile(const std::string& fileName);

  /**
   * Checks a vector of indices to determine if they are all valid in the applicable word list
   *
   * @param indices the indices to check the validity of
   *
   * @return true if all indices are valid for the applicable word list, otherwise false
   */
  [[nodiscard]] bool validateIndexInputs(const std::vector<uint16_t>& indices) const;

  /**
   * Gets the index of a given word in the applicable word list. Throws an exception if the word isn't in the applicable
   * list
   *
   * @param word the word to get the index of
   *
   * @return the index of the given word in the applicable word list
   */
  [[nodiscard]] uint16_t getIndexFromWordString(const std::string& word) const;

  /**
   * Gets the word at the given index in the applicable word list. Throws an exception if the index isn't valid
   *
   * @param index the index of the word to get
   *
   * @return the word at index
   */
  [[nodiscard]] std::string getWordFromIndex(uint16_t index) const;

  /**
   * Gets the word list for this mnemonic
   *
   * @return the word list
   */
  [[nodiscard]] virtual const std::vector<std::string>& getWordList() const = 0;

  /**
   * Gets a set of values, which represent acceptable numbers of words for this mnemonic. Some mnemonic types have only
   * 1 acceptable word count, others have multiple
   *
   * @return the set of acceptable word counts
   */
  [[nodiscard]] virtual std::set<unsigned long> getAcceptableWordCounts() const = 0;

protected:
  /**
   * The standard BIP39 word list
   */
  inline static const std::vector<std::string> bip39WordList = readWordListFromFile("bip39-english.txt");

  /**
   * The legacy hedera word list, which was in use prior to the switch to the standard word list
   */
  inline static const std::vector<std::string> legacyWordList = readWordListFromFile("legacy-english.txt");

  /**
   * Default constructor
   */
  MnemonicAbstract() = default;

  /**
   * Initialize this mnemonic with a vector of word indices. Throws an exception if the the provided indices aren't
   * valid
   *
   * @param indices the indices to initialize the mnemonic with
   */
  void initialize(const std::vector<uint16_t>& indices);

  /**
   * Splits a mnemonic string into individual words
   *
   * @param fullMnemonic a mnemonic, represented as a single string
   * @param delimiter the delimiter separating the words in the fullMnemonic
   *
   * @return the mnemonic, split into individual words
   */
  static std::vector<std::string> splitMnemonicString(const std::string& fullMnemonic, const std::string& delimiter);

  /**
   * Converts a vector of word strings into word indices
   *
   * @param words the words to convert
   *
   * @return a vector of word indices, corresponding to the input words
   */
  [[nodiscard]] std::vector<uint16_t> wordsToIndices(const std::vector<std::string>& words) const;

  /**
   * Checks whether the checksum word of this mnemonic is correct
   *
   * @return true if the checksum succeeds, otherwise false
   */
  [[nodiscard]] bool verifyChecksum() const;

  /**
   * Computes the entropy and checksum of the mnemonic, as described in BIP39
   *
   * @return a byte vector, representing the entropy and checksum of the mnemonic
   */
  [[nodiscard]] std::vector<unsigned char> computeEntropyAndChecksum() const;
};

} // Hedera

#endif // HEDERA_SDK_CPP_MNEMONICABSTRACT_H

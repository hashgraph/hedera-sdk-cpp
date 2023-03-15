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
#ifndef HEDERA_SDK_CPP_MNEMONIC_H_
#define HEDERA_SDK_CPP_MNEMONIC_H_

#include <cstdint>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace Hedera
{
/**
 * The mnemonic represented by this class is in 1 of 3 formats:
 *  1. The legacy v1 format. This mnemonic has 22 words from the "legacy-english.txt" word list
 *  2. The legacy v2 format. This mnemonic has 24 words from the "bip39-english.txt" word list. This format is
 *     indistinguishable from the standard format: in order to correctly compute the private key from this mnemonic, the
 *     user must know that they have a legacy phrase
 *  3. The standard format, as defined by BIP39. A BIP39 phrase may have either 12 or 24 words, from the
 *     "bip39-english.txt" list
 */
class Mnemonic
{
public:
  virtual ~Mnemonic() = default;

  /**
   * Get the string representation of this Mnemonic with the form "<word> <word> <word>..." (words separated by spaces).
   *
   * @return The string representation of this Mnemonic.
   */
  [[nodiscard]] std::string toString() const;

protected:
  Mnemonic() = default;

  /**
   * The standard BIP39 word list.
   */
  static const std::vector<std::string_view> BIP39_WORD_LIST;

  /**
   * Splits a mnemonic string into individual words.
   *
   * @param fullMnemonic The mnemonic, represented as a single string, to split.
   * @param delimiter The delimiter with which to separate the words.
   * @return A vector containing the individual words of the mnemonic.
   */
  static std::vector<std::string> splitMnemonicString(std::string_view fullMnemonic, std::string_view delimiter);

  /**
   * Computes the checksum byte from an array of entropy.
   *
   * @param entropy The array of entropy of which to compute the checksum.
   * @return The checksum of the entropy.
   */
  [[nodiscard]] static unsigned char computeChecksumFromEntropy(const std::vector<unsigned char>& entropy);

  /**
   * Initialize this Mnemonic with a vector of word indices.
   *
   * @param indices The indices with which to initialize this mnemonic.
   * @throws BadMnemonicException If the provided indices aren't valid.
   */
  void initialize(const std::vector<uint16_t>& indices);

  /**
   * Get the indices of the input words.
   *
   * @param words The words from which to get the indices.
   * @return A vector of indices of the input words.
   */
  [[nodiscard]] std::vector<uint16_t> wordsToIndices(const std::vector<std::string>& words) const;

  /**
   * Checks whether the checksum word of this Mnemonic is correct.
   *
   * @return \c TRUE if the checksum succeeds, otherwise \c FALSE.
   */
  [[nodiscard]] bool verifyChecksum() const;

  /**
   * Computes the entropy and checksum of the Mnemonic, as described in BIP39.
   *
   * @return A byte vector, representing the entropy and checksum of the mnemonic.
   */
  [[nodiscard]] std::vector<unsigned char> computeEntropyAndChecksum() const;

  /**
   * Get the word list for this Mnemonic.
   *
   * @return The words that make up this Mnemonic.
   */
  [[nodiscard]] virtual const std::vector<std::string_view>& getWordList() const = 0;

  /**
   * Get the values which represent acceptable word counts for this Mnemonic. Some mnemonic types have only 1 acceptable
   * word count, others have multiple.
   *
   * @return The set of acceptable word counts for this Mnemonic.
   */
  [[nodiscard]] virtual const std::set<unsigned long>& getAcceptableWordCounts() const = 0;

private:
  /**
   * Check a vector of indices to determine if they are all valid in the applicable word list.
   *
   * @param indices The indices of which to check the validity.
   * @return \c TRUE if all indices are valid for the applicable word list, otherwise \c FALSE.
   */
  [[nodiscard]] bool validateIndexInputs(const std::vector<uint16_t>& indices) const;

  /**
   * Get the index of a given word in the applicable word list.
   *
   * @param word The word of which to get the index.
   * @return The index of the given word in this Mnemonic's word list.
   * @throws BadMnemonicException If the input word is not in this Mnemonic's word list.
   */
  [[nodiscard]] uint16_t getIndexFromWordString(std::string_view word) const;

  /**
   * Get the word at the given index in this Mnemonic's word list.
   *
   * @param index The index of the word to get.
   * @return The word at input index.
   * @throws std::out_of_range If the index is out of the range of this Mnemonic's word list.
   */
  [[nodiscard]] std::string getWordFromIndex(uint16_t index) const;

  /**
   * The indices of the words that make up this Mnemonic. Each index in this vector represents a word of the Mnemonic.
   * 16 bit unsigned values are used, since BIP39 indices need 11 bits, and legacy word list indices need 12.
   */
  std::vector<uint16_t> mWordIndices;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_MNEMONIC_H_

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
#ifndef HEDERA_SDK_CPP_MNEMONIC_BIP39_H_
#define HEDERA_SDK_CPP_MNEMONIC_BIP39_H_

#include "Mnemonic.h"

#include <string>
#include <vector>

namespace Hedera
{
/**
 * A standard BIP39 mnemonic.
 */
class MnemonicBIP39 : public Mnemonic
{
public:
  /**
   * Initialize a MnemonicBIP39 from a vector of word indices.
   *
   * @param wordIndices The indices of the words from the BIP32 word list to use to create a MnemonicBIP39.
   * @return An initialized MnemonicBIP39.
   */
  static MnemonicBIP39 initializeBIP39Mnemonic(const std::vector<uint16_t>& wordIndices);

  /**
   * Initialize a MnemonicBIP39 from a vector of word strings.
   *
   * @param words The words from which to create a MnemonicBIP39.
   * @return An initialized MnemonicBIP39.
   */
  static MnemonicBIP39 initializeBIP39Mnemonic(const std::vector<std::string>& words);

  /**
   * Initialize a MnemonicBIP39 from a string containing all the words of a mnemonic phrase.
   *
   * @param fullMnemonic The string that contains all the words from which to create a MnemonicBIP39.
   * @param delimiter    The delimiting string for the mnemonic string.
   * @return An initialized MnemonicBIP39.
   * @throws std::invalid_argument If the resulting checksum is invalid.
   */
  static MnemonicBIP39 initializeBIP39Mnemonic(const std::string& fullMnemonic, const std::string& delimiter = " ");

  /**
   * Generate a MnemonicBIP39 phrase with 12 words.
   *
   * @return The newly-generated MnemonicBIP39.
   */
  static MnemonicBIP39 generate12WordBIP39Mnemonic();

  /**
   * Generate a MnemonicBIP39 phrase with 24 words.
   *
   * @return The newly-generated MnemonicBIP39.
   */
  static MnemonicBIP39 generate24WordBIP39Mnemonic();

  /**
   * Compute a seed that results from this MnemonicBIP39.
   *
   * @param passphrase The passphrase to use in seed generation.
   * @return This MnemonicBIP39's seed, given the input passphrase.
   * @throws std::runtime_error If OpenSSL HMAC generation fails.
   */
  [[nodiscard]] std::vector<unsigned char> toSeed(const std::string& passphrase = "") const;

  /**
   * Compute the word indices that result from the input entropy.
   *
   * @param entropy The entropy from which to compute the word indices.
   * @return A vector containing the word indices.
   */
  static std::vector<uint16_t> entropyToWordIndices(const std::vector<unsigned char>& entropy);

protected:
  /**
   * Derived from Mnemonic. Get a reference to the BIP39 word list.
   *
   * @return The BIP39 word list.
   */
  [[nodiscard]] const std::vector<std::string>& getWordList() const override;

  /**
   * Get the set of acceptable word counts. In the case of a BIP39 passphrase, this set will include 12 and 24.
   *
   * @return The set of acceptable word counts.
   */
  [[nodiscard]] const std::set<unsigned long>& getAcceptableWordCounts() const override;

private:
  /**
   * Hidden constructor. Static initialization/generation functions should be used to create MnemonicBIP39 objects.
   */
  MnemonicBIP39() = default;

  /**
   * Set of acceptable words counts for a MnemonicBIP39.
   */
  inline static const std::set<unsigned long> ACCEPTABLE_COUNTS = { 12U, 24U };
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_MNEMONIC_BIP39_H_

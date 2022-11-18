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
  [[nodiscard]] std::string toString() const;

private:
  /**
   * Reads in a word list file line by line
   *
   * @param fileName the file containing the word list
   *
   * @return a vector of strings, where each element is the string making up a line of the file
   */
  static std::vector<std::string> readWordListFromFile(const std::string& fileName)
  {
    std::vector<std::string> output = std::vector<std::string>();

    std::ifstream infile(fileName);
    if (infile.is_open())
    {
      std::string line;
      while (std::getline(infile, line))
      {
        output.push_back(line);
      }

      infile.close();
    }

    return output;
  }

  /**
   * The words that make up this mnemonic
   */
  std::vector<uint16_t> wordIndices;

  [[nodiscard]] bool validateIndexInputs(const std::vector<uint16_t>& indices) const;

  [[nodiscard]] uint16_t getIndexFromWordString(const std::string& word) const;

  [[nodiscard]] std::string getWordFromIndex(uint16_t index) const;

  [[nodiscard]] virtual const std::vector<std::string>& getWordList() const = 0;

  [[nodiscard]] virtual std::set<unsigned long> getAcceptableWordCounts() const = 0;

protected:
  inline static const std::vector<std::string> bip39WordList = readWordListFromFile("bip39-english.txt");
  inline static const std::vector<std::string> legacyWordList = readWordListFromFile("legacy-english.txt");

  MnemonicAbstract() = default;

  bool initialize(const std::vector<uint16_t>& indices);

  static std::vector<std::string> splitMnemonicString(const std::string& fullMnemonic, const std::string& delimiter);

  [[nodiscard]] std::vector<uint16_t> wordsToIndices(const std::vector<std::string>& words) const;

  [[nodiscard]] bool verifyChecksum() const;

  [[nodiscard]] std::vector<unsigned char> computeEntropyAndChecksum() const;
};

} // Hedera

#endif // HEDERA_SDK_CPP_MNEMONICABSTRACT_H

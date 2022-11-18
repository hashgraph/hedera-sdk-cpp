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

#include "MnemonicAbstract.h"
#include "helper/OpenSSLHasher.h"
#include "helper/HexConverter.h"

namespace Hedera
{

bool MnemonicAbstract::initialize(const std::vector<uint16_t>& indices)
{
  if (!validateIndexInputs(indices))
  {
    throw std::invalid_argument("Invalid indices provided");
  }

  wordIndices = indices;
}

bool MnemonicAbstract::validateIndexInputs(const std::vector<uint16_t>& indices) const
{
  if (!getAcceptableWordCounts().contains(indices.size()))
  {
    return false;
  }

  const uint16_t wordListSize = getWordList().size();

  return std::none_of(indices.begin(), indices.end(), [wordListSize](uint16_t index) { return index >= wordListSize; });
}

std::vector<std::string> MnemonicAbstract::splitMnemonicString(const std::string& fullMnemonic,
                                                               const std::string& delimiter)
{
  std::string workingString = fullMnemonic;

  std::vector<std::string> output = {};

  size_t position = 0;
  while ((position = workingString.find(delimiter)) != std::string::npos)
  {
    output.push_back(workingString.substr(0, position));
    workingString.erase(0, position + delimiter.length());
  }

  output.push_back(workingString.substr(0, std::string::npos));

  return output;
}

std::vector<uint16_t> MnemonicAbstract::wordsToIndices(const std::vector<std::string>& words) const
{
  std::vector<uint16_t> output = {};

  for (const std::string& word : words)
  {
    output.push_back(getIndexFromWordString(word));
  }

  return output;
}

uint16_t MnemonicAbstract::getIndexFromWordString(const std::string& word) const
{
  const std::vector<std::string>& wordList = getWordList();

  auto positionIterator = std::find(wordList.begin(), wordList.end(), word);

  if (positionIterator == wordList.end())
  {
    throw std::invalid_argument("Invalid word");
  }

  return positionIterator - wordList.begin();
}

bool MnemonicAbstract::verifyChecksum() const
{
  const std::vector<unsigned char>& entropyAndChecksum = wordsToEntropyAndChecksum();

  const std::vector<unsigned char> entropy = { entropyAndChecksum.begin(), entropyAndChecksum.end() - 1 };
  unsigned char checksum = entropyAndChecksum[entropyAndChecksum.size() - 1];

  return OpenSSLHasher::computeSHA256(entropy)[0] == checksum;
}

std::vector<unsigned char> MnemonicAbstract::wordsToEntropyAndChecksum() const
{
  // The algorithm described below is the reverse algorithm of `entropyToWords()`
  // Recall, since each mnemonic word index is < 2048, it can be contained in an 11 bit unsigned integer. In the
  // description below, an X represents an unset bit, and an underscore represents a meaningful bit. Numbers in
  // parentheses are for reference in the algorithm implementation
  //
  // Algorithm start:
  // (1) The 11 bits of the first mnemonic word index is 'or'ed into 32 bit scratch
  // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX -> XXXXXXXXXXXXXXXXXXXXX___________
  //                      ^^^^^^^^^^^
  //                      word index
  //
  // (2) Then, chunks of 8 bits are peeled off and put into the buffer. After the first mnemonic word, there will be 1
  // completely set byte ready to peel off, and 3 bits remaining that can't be peeled off yet
  // XXXXXXXXXXXXXXXXXXXXX___(________) -> XXXXXXXXXXXXXXXXXXXXXXXXXXXXX___
  //                          ^^^^^^^^                                  ^^^
  //                     pushback to buffer                not enough bits to make a byte
  //
  // Once there aren't enough bits left to peel off a full byte, we are ready to 'or' the next word index
  // into scratch.
  //
  // (3) Scratch is shifted left by 11 bits to make room for the next word index, while preserving the
  // remaining meaningful bits from before
  // XXXXXXXXXXXXXXXXXXXXXXXXXXXXX___ -> XXXXXXXXXXXXXXXXXX___XXXXXXXXXXX
  //                              ^^^
  //                       must be preserved
  //
  // (1) And the next word index is 'or'ed in
  // XXXXXXXXXXXXXXXXXX___XXXXXXXXXXX -> XXXXXXXXXXXXXXXXXX______________
  //                      ^^^^^^^^^^^
  //                    next word index
  //
  // (2) Now we can peel off more chunks of 8 bits, until no full chunks of 8 remain
  // XXXXXXXXXXXXXXXXXX______(________) -> XXXXXXXXXXXXXXXXXXXXXXXXXX______
  //                          ^^^^^^^^                               ^^^^^^
  //                     pushback to buffer              not enough bits to make a byte
  //
  // Repeat until all mnemonic words are handled.
  //
  // (4) For 12 word mnemonics, the final byte of the buffer only has 4 meaningful bits ____XXXX
  //                                                                                ^^^^
  // This is handled separately, after having iterated all mnemonic words

  std::vector<unsigned char> buffer = {};

  unsigned int scratch = 0;
  unsigned int offset = 0;
  for (uint16_t wordIndex : wordIndices)
  {
    scratch <<= 11; // (3)

    scratch |= wordIndex; // (1)
    offset += 11;

    while (offset >= 8) // (2)
    {
      buffer.push_back((unsigned char)(scratch >> (offset - 8)));
      offset -= 8;
    }
  }

  if (offset != 0) // (4)
  {
    buffer.push_back((unsigned char)(scratch << offset));
  }

  return buffer;
}

} // Hedera
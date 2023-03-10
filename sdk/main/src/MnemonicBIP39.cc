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
#include "MnemonicBIP39.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ED25519PrivateKey.h"
#include "exceptions/BadMnemonicException.h"
#include "exceptions/OpenSSLException.h"
#include "impl/DerivationPathUtils.h"
#include "impl/OpenSSLUtils.h"

#include <openssl/evp.h>

namespace Hedera
{
namespace
{
// The number of bytes of entropy to use to generate a 12-word MnemonicBIP39.
constexpr const int NUMBER_OF_ENTROPY_BYTES_12_WORD = 16;
// The number of bytes of entropy to use to generate a 24-word MnemonicBIP39.
constexpr const int NUMBER_OF_ENTROPY_BYTES_24_WORD = 32;
// The number of bytes in a MnemonicBIP39 seed.
constexpr const size_t SEED_SIZE = 64;
// The number of iterations to use when generating a MnemonicBIP39 seed.
constexpr const int SEED_ITERATIONS = 2048;
// The set of acceptable words counts for a MnemonicBIP39.
const std::set<unsigned long> ACCEPTABLE_COUNTS = { 12U, 24U };
}

//-----
MnemonicBIP39 MnemonicBIP39::initializeBIP39Mnemonic(const std::vector<uint16_t>& wordIndices)
{
  MnemonicBIP39 outputMnemonic;
  outputMnemonic.initialize(wordIndices);

  if (!outputMnemonic.verifyChecksum())
  {
    throw BadMnemonicException("Invalid checksum");
  }

  return outputMnemonic;
}

//-----
MnemonicBIP39 MnemonicBIP39::initializeBIP39Mnemonic(const std::vector<std::string>& words)
{
  MnemonicBIP39 outputMnemonic;
  outputMnemonic.initialize(outputMnemonic.wordsToIndices(words));

  if (!outputMnemonic.verifyChecksum())
  {
    throw BadMnemonicException("Invalid checksum");
  }

  return outputMnemonic;
}

//-----
MnemonicBIP39 MnemonicBIP39::initializeBIP39Mnemonic(std::string_view fullMnemonic, std::string_view delimiter)
{
  MnemonicBIP39 outputMnemonic;
  outputMnemonic.initialize(outputMnemonic.wordsToIndices(splitMnemonicString(fullMnemonic, delimiter)));

  if (!outputMnemonic.verifyChecksum())
  {
    throw BadMnemonicException("Invalid checksum");
  }

  return outputMnemonic;
}

//-----
MnemonicBIP39 MnemonicBIP39::generate12WordBIP39Mnemonic()
{
  // BIP39 dictates 16 bytes of entropy for 12 words
  return initializeBIP39Mnemonic(
    entropyToWordIndices(internal::OpenSSLUtils::getRandomBytes(NUMBER_OF_ENTROPY_BYTES_12_WORD)));
}

//-----
MnemonicBIP39 MnemonicBIP39::generate24WordBIP39Mnemonic()
{
  // BIP39 dictates 32 bytes of entropy for 24 words
  return initializeBIP39Mnemonic(
    entropyToWordIndices(internal::OpenSSLUtils::getRandomBytes(NUMBER_OF_ENTROPY_BYTES_24_WORD)));
}

//-----
std::unique_ptr<PrivateKey> MnemonicBIP39::toStandardEd25519PrivateKey(std::string_view passphrase,
                                                                       uint32_t index) const
{
  return ED25519PrivateKey::fromSeed(toSeed(passphrase))->derive(44)->derive(3030)->derive(0)->derive(0)->derive(index);
}

//-----
std::unique_ptr<PrivateKey> MnemonicBIP39::toStandardECDSAsecp256k1PrivateKey(std::string_view passphrase,
                                                                              uint32_t index) const
{
  return ECDSAsecp256k1PrivateKey::fromSeed(toSeed(passphrase))
    ->derive(internal::DerivationPathUtils::getHardenedIndex(44))
    ->derive(internal::DerivationPathUtils::getHardenedIndex(3030))
    ->derive(internal::DerivationPathUtils::getHardenedIndex(0))
    ->derive(0)
    ->derive(index);
}

//-----
std::vector<unsigned char> MnemonicBIP39::toSeed(std::string_view passphrase) const
{
  const internal::OpenSSLUtils::EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_CTX_new"));
  }

  const internal::OpenSSLUtils::EVP_MD messageDigest(EVP_MD_fetch(nullptr, "SHA512", nullptr));
  if (!messageDigest)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_fetch"));
  }

  if (EVP_DigestInit(messageDigestContext.get(), messageDigest.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestInit"));
  }

  std::vector<unsigned char> seed(SEED_SIZE);

  const std::string mnemonicString = toString();
  const std::string saltStr = std::string("mnemonic").append(passphrase);

  if (const std::vector<unsigned char> salt = { saltStr.cbegin(), saltStr.cend() };
      PKCS5_PBKDF2_HMAC(mnemonicString.c_str(),
                        static_cast<int>(mnemonicString.length()),
                        salt.data(),
                        static_cast<int>(salt.size()),
                        SEED_ITERATIONS,
                        messageDigest.get(),
                        static_cast<int>(seed.size()),
                        seed.data()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("PKCS5_PBKDF2_HMAC"));
  }

  return seed;
}

//-----
std::vector<uint16_t> MnemonicBIP39::entropyToWordIndices(const std::vector<unsigned char>& entropy)
{
  std::vector<unsigned char> entropyAndChecksum = entropy;

  entropyAndChecksum.push_back(computeChecksumFromEntropy(entropy));

  // The algorithm described below is the reverse algorithm of `MnemonicAbstract::computeEntropyAndChecksum`
  // Recall, since each mnemonic word index is < 2048 in the BIP39 list, it can be contained in an 11 bit unsigned
  // integer. In the description below, an X represents an unset bit, and an underscore represents a meaningful bit. The
  // scratch variable is being represented by the Xs and underscores. Numbers in parentheses are for reference in the
  // algorithm implementation
  //
  // The algorithm begins with a byte array (produced above) that contains the entropy, plus a 1 byte checksum. Tthe
  // total array size is 17 or 33, for 12 and 24 word mnemonics respectively.
  //
  // Algorithm start:
  // (1) The 8 bits of the first byte in the entropyAndChecksum array are `or`ed into 32 bit scratch
  // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX -> XXXXXXXXXXXXXXXXXXXXXXXX________
  //                         ^^^^^^^^
  //                           byte
  //
  // Each word index being produced is composed of 11 bits. Therefore, the first byte alone isn't enough to produce the
  // first index
  //
  // (2) Scratch is shifted left by 8 bits to make room for the next byte
  // XXXXXXXXXXXXXXXXXXXXXXXX________ -> XXXXXXXXXXXXXXXX________XXXXXXXX
  //                         ^^^^^^^^
  //                    must be preserved
  //
  // (1) The next byte is `or`ed in
  // XXXXXXXXXXXXXXXX________XXXXXXXX -> XXXXXXXXXXXXXXXX________________
  //                         ^^^^^^^^
  //                         next byte
  //
  // (3) Once at least 11 bits have been set, it is time to peel off the first index
  // XXXXXXXXXXXXXXXX_____(___________) -> XXXXXXXXXXXXXXXXXXXXXXXXXXX_____
  //                       ^^^^^^^^^^^
  //         convert to int and add to index list
  //
  // Repeat until all bytes have been handled

  std::vector<uint16_t> wordIndicesOut;

  unsigned int scratch = 0;
  unsigned int offset = 0;
  for (unsigned char byte : entropyAndChecksum)
  {
    scratch <<= 8;   // (2)
    scratch |= byte; // (1)

    offset += 8;

    if (offset >= 11) // (4)
    {
      // zero out all bits except the 11 we care about
      wordIndicesOut.push_back((scratch >> (offset - 11)) & 0x7FF);
      offset -= 11;
    }
  }

  return wordIndicesOut;
}

//-----
const std::vector<std::string_view>& MnemonicBIP39::getWordList() const
{
  return Mnemonic::BIP39_WORD_LIST;
}

//-----
const std::set<unsigned long>& MnemonicBIP39::getAcceptableWordCounts() const
{
  return ACCEPTABLE_COUNTS;
}

} // Hedera
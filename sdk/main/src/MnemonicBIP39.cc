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
#include "impl/DerivationPathUtils.h"
#include "impl/OpenSSLHasher.h"
#include "impl/OpenSSLRandom.h"

#include <bit>
#include <openssl/evp.h>

namespace Hedera
{
//-----
MnemonicBIP39 MnemonicBIP39::initializeBIP39Mnemonic(const std::vector<uint16_t>& wordIndices)
{
  MnemonicBIP39 outputMnemonic;
  outputMnemonic.initialize(wordIndices);

  if (!outputMnemonic.verifyChecksum())
  {
    throw std::invalid_argument("Invalid checksum");
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
    throw std::invalid_argument("Invalid checksum");
  }

  return outputMnemonic;
}

//-----
MnemonicBIP39 MnemonicBIP39::initializeBIP39Mnemonic(const std::string& fullMnemonic, const std::string& delimiter)
{
  MnemonicBIP39 outputMnemonic;
  outputMnemonic.initialize(outputMnemonic.wordsToIndices(splitMnemonicString(fullMnemonic, delimiter)));

  if (!outputMnemonic.verifyChecksum())
  {
    throw std::invalid_argument("Invalid checksum");
  }

  return outputMnemonic;
}

//-----
MnemonicBIP39 MnemonicBIP39::generate12WordBIP39Mnemonic()
{
  // BIP39 dictates 16 bytes of entropy for 12 words
  return initializeBIP39Mnemonic(entropyToWordIndices(internal::OpenSSLRandom::getRandomBytes(16)));
}

//-----
MnemonicBIP39 MnemonicBIP39::generate24WordBIP39Mnemonic()
{
  // BIP39 dictates 32 bytes of entropy for 24 words
  return initializeBIP39Mnemonic(entropyToWordIndices(internal::OpenSSLRandom::getRandomBytes(32)));
}

//-----
std::unique_ptr<ED25519PrivateKey> MnemonicBIP39::toStandardEd25519PrivateKey(const std::string& passphrase,
                                                                              int index) const
{
  return ED25519PrivateKey::fromSeed(toSeed(passphrase))->derive(40)->derive(3030)->derive(0)->derive(0)->derive(index);
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> MnemonicBIP39::toStandardECDSAsecp256k1PrivateKey(
  const std::string& passphrase,
  int index) const
{
  return ECDSAsecp256k1PrivateKey::fromSeed(toSeed(passphrase))
    ->derive(internal::DerivationPathUtils::getHardenedIndex(40))
    ->derive(internal::DerivationPathUtils::getHardenedIndex(3030))
    ->derive(internal::DerivationPathUtils::getHardenedIndex(0))
    ->derive(0)
    ->derive(index);
}

//-----
std::vector<unsigned char> MnemonicBIP39::toSeed(const std::string& passphrase) const
{
  EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_new();

  if (!messageDigestContext)
  {
    throw std::runtime_error("Digest context construction failed");
  }

  EVP_MD* messageDigest = EVP_MD_fetch(nullptr, "SHA512", nullptr);

  if (messageDigest == nullptr)
  {
    EVP_MD_CTX_free(messageDigestContext);
    throw std::runtime_error("Digest construction failed");
  }

  if (EVP_DigestInit(messageDigestContext, messageDigest) <= 0)
  {

    EVP_MD_CTX_free(messageDigestContext);
    EVP_MD_free(messageDigest);
    throw std::runtime_error("Digest init failed");
  }

  std::vector<unsigned char> seed(64);

  const std::string mnemonicString = toString();
  const char* mnemonicStringAddress = mnemonicString.c_str();

  const std::string salt = "mnemonic" + passphrase;

  if (auto saltAddress = reinterpret_cast<const unsigned char*>(salt.c_str());
      PKCS5_PBKDF2_HMAC(mnemonicStringAddress,
                        static_cast<int>(mnemonicString.length()),
                        saltAddress,
                        static_cast<int>(salt.length()),
                        2048,
                        messageDigest,
                        static_cast<int>(seed.size()),
                        &seed.front()) <= 0)
  {
    EVP_MD_CTX_free(messageDigestContext);
    EVP_MD_free(messageDigest);
    throw std::runtime_error("PKCS5_PBKDF2_HMAC failed");
  }

  EVP_MD_CTX_free(messageDigestContext);
  EVP_MD_free(messageDigest);

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

  std::vector<uint16_t> wordIndicesOut = {};

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
const std::vector<std::string>& MnemonicBIP39::getWordList() const
{
  return Mnemonic::BIP39_WORD_LIST;
}

//-----
const std::set<unsigned long>& MnemonicBIP39::getAcceptableWordCounts() const
{
  return ACCEPTABLE_COUNTS;
}

} // Hedera
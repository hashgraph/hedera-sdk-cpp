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

#include "ED25519PrivateKey.h"
#include "helper/DerivationPathUtils.h"
#include "helper/HexConverter.h"
#include "helper/OpenSSLHasher.h"

#include <iostream>
#include <openssl/hmac.h>
#include <openssl/x509.h>
#include <utility>

namespace Hedera
{

ED25519PrivateKey::ED25519PrivateKey(const ED25519PrivateKey& other)
  : keypair(bytesToPKEY(prependAlgorithmIdentifier(other.toBytes())))
  , publicKey(other.publicKey)
  , chainCode()
{
}

ED25519PrivateKey::ED25519PrivateKey(EVP_PKEY* keypair)
  : keypair(keypair)
  , publicKey(ED25519PublicKey::fromBytes(getPublicKeyBytes()))
  , chainCode()
{
}

ED25519PrivateKey::ED25519PrivateKey(EVP_PKEY* keypair, std::vector<unsigned char> chainCode)
  : keypair(keypair)
  , publicKey(ED25519PublicKey::fromBytes(getPublicKeyBytes()))
  , chainCode(std::move(chainCode))
{
}

std::vector<unsigned char> ED25519PrivateKey::getPublicKeyBytes() const
{
  int bytesLength = i2d_PUBKEY(this->keypair, nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);
  unsigned char* rawPublicKeyBytes = &publicKeyBytes.front();

  if (i2d_PUBKEY(this->keypair, &rawPublicKeyBytes) <= 0)
  {
    std::cout << "getPublicKeyBytes I2D error" << std::endl;
  }

  return publicKeyBytes;
}

ED25519PrivateKey::~ED25519PrivateKey()
{
  EVP_PKEY_free(this->keypair);
}

std::shared_ptr<PublicKey> ED25519PrivateKey::getPublicKey() const
{
  return publicKey;
}

std::vector<unsigned char> ED25519PrivateKey::sign(const std::vector<unsigned char>& bytesToSign) const
{
  EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_new();

  if (!messageDigestContext)
  {
    std::cout << "Digest context construction failed" << std::endl;
  }

  if (EVP_DigestSignInit(messageDigestContext, nullptr, nullptr, nullptr, this->keypair) <= 0)
  {
    std::cout << "Digest sign init failed" << std::endl;
  }

  size_t signatureLength;
  /* Calculate the required size for the signature */
  if (EVP_DigestSign(messageDigestContext, nullptr, &signatureLength, &bytesToSign.front(), bytesToSign.size()) <= 0)
  {
    std::cout << "Failed to calculate signature length" << std::endl;
  }

  std::vector<unsigned char> signature = std::vector<unsigned char>(signatureLength);

  if (EVP_DigestSign(
        messageDigestContext, &signature.front(), &signatureLength, &bytesToSign.front(), bytesToSign.size()) <= 0)
  {
    std::cout << "Signing failed" << std::endl;
  }

  EVP_MD_CTX_free(messageDigestContext);

  return signature;
}

std::string ED25519PrivateKey::toString() const
{
  return HexConverter::base64ToHex(toBytes());
}

std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromString(const std::string& keyString)
{
  return std::make_unique<ED25519PrivateKey>(ED25519PrivateKey(bytesToPKEY(HexConverter::hexToBase64(keyString))));
}

std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::generatePrivateKey()
{
  EVP_PKEY* keypair = EVP_PKEY_new();
  EVP_PKEY_CTX* keyAlgorithmContext = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr);

  if (!keyAlgorithmContext)
  {
    std::cout << "CONTEXT INVALID" << std::endl;
  }

  if (EVP_PKEY_keygen_init(keyAlgorithmContext) <= 0)
  {
    std::cout << "INIT ERROR" << std::endl;
  }

  if (EVP_PKEY_generate(keyAlgorithmContext, &keypair) <= 0)
  {
    std::cout << "GENERATE ERROR" << std::endl;
  }

  EVP_PKEY_CTX_free(keyAlgorithmContext);

  return std::make_unique<ED25519PrivateKey>(ED25519PrivateKey(keypair));
}

std::vector<unsigned char> ED25519PrivateKey::toBytes() const
{
  int bytesLength = i2d_PrivateKey(this->keypair, nullptr);

  std::vector<unsigned char> outputBytes(bytesLength);
  unsigned char* rawBytes = &outputBytes.front();

  if (i2d_PrivateKey(this->keypair, &rawBytes) <= 0)
  {
    std::cout << "ED25519PrivateKey toBytes I2D error" << std::endl;
  }

  // don't return the algorithm identification bytes
  return { outputBytes.begin() + 16, outputBytes.end() };
}

EVP_PKEY* ED25519PrivateKey::bytesToPKEY(const std::vector<unsigned char>& keyBytes)
{
  std::vector<unsigned char> fullKeyBytes;
  // If there are only 32 key bytes, we need to add the algorithm identifier bytes, so that OpenSSL can correctly decode
  if (keyBytes.size() == 32)
  {
    fullKeyBytes = prependAlgorithmIdentifier(keyBytes);
  }
  else
  {
    fullKeyBytes = keyBytes;
  }

  const unsigned char* rawKeyBytes = &fullKeyBytes.front();

  return d2i_PrivateKey(EVP_PKEY_ED25519, nullptr, &rawKeyBytes, (long)fullKeyBytes.size());
}

std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromBIP39Mnemonic(const MnemonicBIP39& mnemonic,
                                                                        const std::string& passphrase)
{
  return fromSeed(mnemonic.toSeed(passphrase));
}

std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromHMACOutput(const std::vector<unsigned char>& hmacOutput)
{
  if (hmacOutput.size() != 64) {
    throw std::runtime_error("Unexpected size for hmac output");
  }

  // the first 32 bytes of the hmac are the new key material. the algorithm identifier must come first, though
  std::vector<unsigned char> fullKey = prependAlgorithmIdentifier({ hmacOutput.begin(), hmacOutput.begin() + 32 });

  // chain code is the next 32 bytes of the computed hmac
  std::vector<unsigned char> chainCode(hmacOutput.begin() + 32, hmacOutput.end());

  return std::unique_ptr<ED25519PrivateKey>(new ED25519PrivateKey(bytesToPKEY(fullKey), chainCode));
}

std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromSeed(const std::vector<unsigned char>& seed)
{
  std::string keyString = "ed25519 seed"; // as defined by SLIP 0010

  return fromHMACOutput(OpenSSLHasher::computeSHA512HMAC({ keyString.begin(), keyString.end() }, seed));
}

std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::derive(const uint32_t childIndex) const
{
  if (chainCode.size() != 32)
  {
    throw std::runtime_error("Key doesn't support derivation");
  }

  if (!DerivationPathUtils::isHardenedChildIndex(childIndex))
  {
    throw std::runtime_error("Key supports only hardened derivation");
  }

  // as per SLIP0010, private key must be padded to 33 bytes
  std::vector<unsigned char> data = { 0x0 };

  const std::vector<unsigned char> keyBytes = toBytes();
  data.insert(data.end(), keyBytes.begin(), keyBytes.end());

  // converts unsigned 32 bit int index into big endian byte array (ser32 function from BIP 32)
  std::vector<unsigned char> indexVector = {};
  for (int byteIndex = 3; byteIndex >= 0; --byteIndex)
  {
    indexVector.push_back((childIndex >> (byteIndex << 3)) & 0xFF);
  }
  data.insert(data.end(), indexVector.begin(), indexVector.end());

  return fromHMACOutput(OpenSSLHasher::computeSHA512HMAC(chainCode, data));
}

std::vector<unsigned char> ED25519PrivateKey::prependAlgorithmIdentifier(const std::vector<unsigned char>& keyBytes)
{
  // full key will begin with the algorithm identifier bytes
  std::vector<unsigned char> fullKey = ALGORITHM_IDENTIFIER_BYTES;

  // insert the raw key bytes onto the end of the full key
  fullKey.insert(fullKey.end(), keyBytes.begin(), keyBytes.end());

  return fullKey;
}

std::vector<unsigned char> ED25519PrivateKey::getChainCode() const
{
  return chainCode;
}
}

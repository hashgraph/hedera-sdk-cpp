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
#include "helper/HexConverter.h"
#include "openssl/x509.h"
#include <iostream>

#include "helper/HexConverter.h"

namespace Hedera
{

ED25519PrivateKey::ED25519PrivateKey(EVP_PKEY* keypair)
{
  this->keypair = keypair;

  int bytesLength = i2d_PUBKEY(this->keypair, nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);
  unsigned char* rawPublicKeyBytes = &publicKeyBytes.front();

  if (i2d_PUBKEY(this->keypair, &rawPublicKeyBytes) <= 0)
  {
    std::cout << "I2D error" << std::endl;
  }

  this->publicKey = ED25519PublicKey::fromDEREncoding(publicKeyBytes);
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
  int bytesLength = i2d_PrivateKey(this->keypair, nullptr);

  std::vector<unsigned char> bytes(bytesLength);
  unsigned char* rawBytes = &bytes.front();

  if (i2d_PrivateKey(this->keypair, &rawBytes) <= 0)
  {
    std::cout << "I2D error" << std::endl;
  }

  return HexConverter::bytesToHex(bytes);
}

std::shared_ptr<ED25519PrivateKey> ED25519PrivateKey::generatePrivateKey()
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

  return std::make_shared<ED25519PrivateKey>(keypair);
}

std::shared_ptr<ED25519PrivateKey> ED25519PrivateKey::fromDEREncoding(const std::string& derEncodedKey)
{
  std::vector<unsigned char> keyBytes = HexConverter::hexToBytes(derEncodedKey);
  const unsigned char* rawKeyBytes = &keyBytes.front();

  return std::make_shared<ED25519PrivateKey>(
    d2i_PrivateKey(EVP_PKEY_ED25519, nullptr, &rawKeyBytes, (long)keyBytes.size()));
}
}

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

#include "ED25519Keypair.h"
#include "helper/HexConverter.h"
#include "openssl/x509.h"
#include <iostream>

namespace Hedera
{
ED25519Keypair::ED25519Keypair()
{
  this->keypair = nullptr;

  EVP_PKEY_CTX* keyAlgorithmContext = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr);

  if (!keyAlgorithmContext)
  {
    std::cout << "CONTEXT INVALID" << std::endl;
  }

  if (EVP_PKEY_keygen_init(keyAlgorithmContext) <= 0)
  {
    std::cout << "INIT ERROR" << std::endl;
  }

  if (EVP_PKEY_generate(keyAlgorithmContext, &this->keypair) <= 0)
  {
    std::cout << "GENERATE ERROR" << std::endl;
  }

  EVP_PKEY_CTX_free(keyAlgorithmContext);

  size_t publicKeyLength = 32;
  std::vector<unsigned char> rawPublicKey(publicKeyLength);

  if (EVP_PKEY_get_raw_public_key(this->keypair, &rawPublicKey.front(), &publicKeyLength) <= 0)
  {
    std::cout << "GET RAW PUBLIC KEY ERROR" << std::endl;
  }

  this->publicKey = std::make_shared<ED25519PublicKey>(rawPublicKey);
}

ED25519Keypair::~ED25519Keypair()
{
  EVP_PKEY_free(this->keypair);
}

std::shared_ptr<PublicKey> ED25519Keypair::getPublicKey() const
{
  return publicKey;
}

std::vector<unsigned char> ED25519Keypair::sign(const std::vector<unsigned char>& bytesToSign) const
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
}

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

#include "ED25519PublicKey.h"

#include "helper/HexConverter.h"
#include "openssl/x509.h"

#include <iostream>
#include <openssl/err.h>

namespace Hedera
{
ED25519PublicKey::ED25519PublicKey(EVP_PKEY* publicKey)
{
  this->publicKey = publicKey;
}

ED25519PublicKey::~ED25519PublicKey()
{
  EVP_PKEY_free(this->publicKey);
}

proto::Key* ED25519PublicKey::toProtobuf() const
{
  return nullptr;
}

std::string ED25519PublicKey::toString() const
{
  int bytesLength = i2d_PUBKEY(this->publicKey, nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);
  unsigned char* rawPublicKeyBytes = &publicKeyBytes.front();

  if (i2d_PUBKEY(this->publicKey, &rawPublicKeyBytes) <= 0)
  {
    std::cout << "I2D error" << std::endl;
  }

  return HexConverter::bytesToHex(publicKeyBytes);
}

bool ED25519PublicKey::verifySignature(const std::vector<unsigned char>& signatureBytes,
                                       const std::vector<unsigned char>& signedBytes) const
{
  EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_new();

  if (!messageDigestContext)
  {
    std::cout << "Digest context construction failed" << std::endl;
  }

  if (EVP_DigestVerifyInit(messageDigestContext, nullptr, nullptr, nullptr, this->publicKey) <= 0)
  {
    std::cout << "Digest verify init failed" << std::endl;
  }

  int verificationResult = EVP_DigestVerify(
    messageDigestContext, &signatureBytes.front(), signatureBytes.size(), &signedBytes.front(), signedBytes.size());

  if (verificationResult <= 0)
  {
    std::cout << "Failed to verify signature with code [" << verificationResult << "]" << std::endl;
  }

  EVP_MD_CTX_free(messageDigestContext);

  return verificationResult == 1;
}

std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromDEREncoding(const std::string& derEncodedKey)
{
  return fromDEREncoding(HexConverter::hexToBytes(derEncodedKey));
}

std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromDEREncoding(const std::vector<unsigned char>& derEncodedKey)
{
  const unsigned char* rawKeyBytes = &derEncodedKey.front();

  return std::make_shared<ED25519PublicKey>(d2i_PUBKEY(nullptr, &rawKeyBytes, (long)derEncodedKey.size()));
}

} // namespace Hedera

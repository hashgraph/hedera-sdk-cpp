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
#include "proto/basic_types.pb.h"

#include <iostream>
#include <openssl/err.h>

namespace Hedera
{
ED25519PublicKey::ED25519PublicKey(const ED25519PublicKey& other)
{
  this->publicKey = bytesToPKEY(prependAlgorithmIdentifier(other.toBytes()));
}

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
  auto* keyProtobuf = new proto::Key();

  std::vector<unsigned char> rawBytes = toBytes();
  auto* stringPointer = new std::string({ rawBytes.cbegin(), rawBytes.cend() });

  keyProtobuf->set_allocated_ed25519(stringPointer);

  return keyProtobuf;
}

std::string ED25519PublicKey::toString() const
{
  return HexConverter::base64ToHex(toBytes());
}

std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromString(const std::string& keyString)
{
  return fromBytes(HexConverter::hexToBase64(keyString));
}

std::vector<unsigned char> ED25519PublicKey::toBytes() const
{
  int bytesLength = i2d_PUBKEY(this->publicKey, nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);
  unsigned char* rawPublicKeyBytes = &publicKeyBytes.front();

  if (i2d_PUBKEY(this->publicKey, &rawPublicKeyBytes) <= 0)
  {
    std::cout << "ED25519PublicKey to bytes I2D error" << std::endl;
  }

  // don't return the algorithm identification bytes
  return { publicKeyBytes.begin() + 12, publicKeyBytes.end() };
}

std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromBytes(const std::vector<unsigned char>& keyBytes)
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

  return std::make_shared<ED25519PublicKey>(ED25519PublicKey(bytesToPKEY(fullKeyBytes)));
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

EVP_PKEY* ED25519PublicKey::bytesToPKEY(const std::vector<unsigned char>& keyBytes)
{
  const unsigned char* rawKeyBytes = &keyBytes.front();

  return d2i_PUBKEY(nullptr, &rawKeyBytes, (long)keyBytes.size());
}

std::vector<unsigned char> ED25519PublicKey::prependAlgorithmIdentifier(const std::vector<unsigned char>& keyBytes)
{
  // full key will begin with the algorithm identifier bytes
  std::vector<unsigned char> fullKey = ALGORITHM_IDENTIFIER_BYTES;

  // insert the raw key bytes onto the end of the full key
  fullKey.insert(fullKey.end(), keyBytes.begin(), keyBytes.end());

  return fullKey;
}

} // namespace Hedera

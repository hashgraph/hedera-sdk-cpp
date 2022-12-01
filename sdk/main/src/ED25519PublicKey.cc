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
#include "impl/HexConverter.h"

#include <iostream>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <proto/basic_types.pb.h>

namespace Hedera
{
namespace
{
inline const std::string DER_PREFIX_HEX = "302A300506032B6570032100";
inline const std::vector<unsigned char> DER_PREFIX_BYTES = internal::HexConverter::hexToBase64(DER_PREFIX_HEX);
}

//-----
ED25519PublicKey::~ED25519PublicKey()
{
  EVP_PKEY_free(mPublicKey);
}

//-----
ED25519PublicKey::ED25519PublicKey(const ED25519PublicKey& other)
  : mPublicKey(bytesToPKEY(other.toBytes()))
{
}

//-----
ED25519PublicKey& ED25519PublicKey::operator=(const ED25519PublicKey& other)
{
  if (this != &other)
  {
    mPublicKey = bytesToPKEY(other.toBytes());
  }

  return *this;
}

//-----
ED25519PublicKey::ED25519PublicKey(ED25519PublicKey&& other) noexcept
  : mPublicKey(other.mPublicKey)
{
  other.mPublicKey = nullptr;
}

//-----
ED25519PublicKey& ED25519PublicKey::operator=(ED25519PublicKey&& other) noexcept
{
  mPublicKey = other.mPublicKey;
  other.mPublicKey = nullptr;
  return *this;
}

//-----
std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromString(const std::string& keyString)
{
  std::string fullKeyString = keyString;

  // key size of 64 means RFC 8410 prefix is missing. add it before making calls to OpenSSL
  if (keyString.size() == 64)
  {
    fullKeyString = DER_PREFIX_HEX + keyString;
  }

  return fromBytes(internal::HexConverter::hexToBase64(fullKeyString));
}

//-----
std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromBytes(const std::vector<unsigned char>& keyBytes)
{
  std::vector<unsigned char> fullKeyBytes;

  // If there are only 32 key bytes, we need to add the DER prefex, so that OpenSSL can correctly decode
  if (keyBytes.size() == 32)
  {
    fullKeyBytes = DER_PREFIX_BYTES;
    fullKeyBytes.insert(fullKeyBytes.end(), keyBytes.begin(), keyBytes.end());
  }
  else
  {
    fullKeyBytes = keyBytes;
  }

  return std::make_shared<ED25519PublicKey>(ED25519PublicKey(bytesToPKEY(fullKeyBytes)));
}

//-----
std::unique_ptr<PublicKey> ED25519PublicKey::clone() const
{
  return std::make_unique<ED25519PublicKey>(*this);
}

//----
std::unique_ptr<proto::Key> ED25519PublicKey::toProtobuf() const
{
  auto keyProtobuf = std::make_unique<proto::Key>();

  // raw bytes, which include the ed25519 DER encoding prefix
  std::vector<unsigned char> rawBytes = toBytes();

  // discard the 12 prefix bytes, leaving behind only the 32 pubkey bytes
  keyProtobuf->set_allocated_ed25519(new std::string({ rawBytes.cbegin() + 12, rawBytes.cend() }));

  return keyProtobuf;
}

//-----
bool ED25519PublicKey::verifySignature(const std::vector<unsigned char>& signatureBytes,
                                       const std::vector<unsigned char>& signedBytes) const
{
  EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_new();

  if (!messageDigestContext)
  {
    throw std::runtime_error("Digest context construction failed");
  }

  if (EVP_DigestVerifyInit(messageDigestContext, nullptr, nullptr, nullptr, mPublicKey) <= 0)
  {
    EVP_MD_CTX_free(messageDigestContext);
    throw std::runtime_error("Digest verify initialization failed");
  }

  int verificationResult = EVP_DigestVerify(
    messageDigestContext, &signatureBytes.front(), signatureBytes.size(), &signedBytes.front(), signedBytes.size());

  if (verificationResult <= 0)
  {
    std::cout << "Failed to verify signature with code [" << verificationResult << ']' << std::endl;
  }

  EVP_MD_CTX_free(messageDigestContext);

  return verificationResult == 1;
}

//-----
std::string ED25519PublicKey::toString() const
{
  return internal::HexConverter::base64ToHex(toBytes());
}

//-----
ED25519PublicKey::ED25519PublicKey(EVP_PKEY* publicKey)
  : mPublicKey(publicKey)
{
}

//-----
std::vector<unsigned char> ED25519PublicKey::toBytes() const
{
  int bytesLength = i2d_PUBKEY(mPublicKey, nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &publicKeyBytes.front(); i2d_PUBKEY(mPublicKey, &rawPublicKeyBytes) <= 0)
  {
    throw std::runtime_error("ED25519PublicKey serialization error");
  }

  return publicKeyBytes;
}

//-----
EVP_PKEY* ED25519PublicKey::bytesToPKEY(const std::vector<unsigned char>& keyBytes)
{
  const unsigned char* rawKeyBytes = &keyBytes.front();
  return d2i_PUBKEY(nullptr, &rawKeyBytes, static_cast<long>(keyBytes.size()));
}

} // namespace Hedera

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
#include "exceptions/BadKeyException.h"
#include "exceptions/OpenSSLException.h"
#include "impl/HexConverter.h"
#include "impl/OpenSSLUtils.h"

#include <openssl/x509.h>
#include <proto/basic_types.pb.h>

namespace Hedera
{
namespace
{
// The number of bytes in an ED25519PublicKey.
constexpr const size_t PUBLIC_KEY_SIZE = 32;
// The algorithm identifier bytes for an ED25519PublicKey.
const std::vector<unsigned char> ALGORITHM_IDENTIFIER_BYTES = { 0x30, 0x2A, 0x30, 0x05, 0x06, 0x03,
                                                                0x2B, 0x65, 0x70, 0x03, 0x21, 0x00 };
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
  : mPublicKey(std::move(other.mPublicKey))
{
}

//-----
ED25519PublicKey& ED25519PublicKey::operator=(ED25519PublicKey&& other) noexcept
{
  mPublicKey = std::move(other.mPublicKey);
  return *this;
}

//-----
std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromString(std::string_view keyString)
{
  try
  {
    return fromBytes(internal::HexConverter::hexToBytes(keyString));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(openSSLException.what());
  }
}

//-----
std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromBytes(const std::vector<unsigned char>& keyBytes)
{
  try
  {
    const ED25519PublicKey ed25519PublicKey(bytesToPKEY(keyBytes));
    return (ed25519PublicKey.mPublicKey) ? std::make_shared<ED25519PublicKey>(ed25519PublicKey) : nullptr;
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(openSSLException.what());
  }
  catch (const std::invalid_argument& invalidArgument)
  {
    throw BadKeyException(invalidArgument.what());
  }
}

//-----
std::unique_ptr<PublicKey> ED25519PublicKey::clone() const
{
  return std::make_unique<ED25519PublicKey>(*this);
}

//-----
bool ED25519PublicKey::verifySignature(const std::vector<unsigned char>& signatureBytes,
                                       const std::vector<unsigned char>& signedBytes) const
{
  const internal::OpenSSLUtils::EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_CTX_new"));
  }

  if (EVP_DigestVerifyInit(messageDigestContext.get(), nullptr, nullptr, nullptr, mPublicKey.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestVerifyInit"));
  }

  const int verificationResult = EVP_DigestVerify(messageDigestContext.get(),
                                                  (!signatureBytes.empty()) ? &signatureBytes.front() : nullptr,
                                                  signatureBytes.size(),
                                                  (!signedBytes.empty()) ? &signedBytes.front() : nullptr,
                                                  signedBytes.size());

  // any value other than 0 or 1 means an error occurred
  if (verificationResult != 0 && verificationResult != 1)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestVerify"));
  }

  return verificationResult == 1;
}

//----
std::unique_ptr<proto::Key> ED25519PublicKey::toProtobuf() const
{
  auto keyProtobuf = std::make_unique<proto::Key>();
  const std::vector<unsigned char> rawBytes = toBytes();
  keyProtobuf->set_allocated_ed25519(new std::string({ rawBytes.cbegin(), rawBytes.cend() }));
  return keyProtobuf;
}

//-----
std::string ED25519PublicKey::toString() const
{
  return internal::HexConverter::bytesToHex(toBytes());
}

//-----
std::vector<unsigned char> ED25519PublicKey::toBytes() const
{
  int bytesLength = i2d_PUBKEY(mPublicKey.get(), nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &publicKeyBytes.front(); i2d_PUBKEY(mPublicKey.get(), &rawPublicKeyBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PUBKEY"));
  }

  // don't return the algorithm identification bytes
  return { publicKeyBytes.begin() + static_cast<long>(ALGORITHM_IDENTIFIER_BYTES.size()), publicKeyBytes.end() };
}

//-----
internal::OpenSSLUtils::EVP_PKEY ED25519PublicKey::bytesToPKEY(std::vector<unsigned char> keyBytes)
{
  if (keyBytes.size() == PUBLIC_KEY_SIZE)
  {
    keyBytes.insert(keyBytes.begin(), ALGORITHM_IDENTIFIER_BYTES.cbegin(), ALGORITHM_IDENTIFIER_BYTES.cend());
  }
  else if (keyBytes.size() != PUBLIC_KEY_SIZE + ALGORITHM_IDENTIFIER_BYTES.size())
  {
    throw std::invalid_argument("bytesToPKEY input bytes size [" + std::to_string(keyBytes.size()) +
                                "] is invalid: must be either [" + std::to_string(PUBLIC_KEY_SIZE) + "] or [" +
                                std::to_string(PUBLIC_KEY_SIZE + ALGORITHM_IDENTIFIER_BYTES.size()) + "]");
  }

  const unsigned char* rawKeyBytes = &keyBytes.front();
  internal::OpenSSLUtils::EVP_PKEY key(d2i_PUBKEY(nullptr, &rawKeyBytes, static_cast<long>(keyBytes.size())));
  if (!key)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("d2i_PUBKEY"));
  }

  return key;
}

//-----
ED25519PublicKey::ED25519PublicKey(internal::OpenSSLUtils::EVP_PKEY&& publicKey)
  : mPublicKey(std::move(publicKey))
{
}

} // namespace Hedera

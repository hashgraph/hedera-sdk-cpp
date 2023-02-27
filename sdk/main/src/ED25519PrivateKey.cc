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
#include "ED25519PrivateKey.h"
#include "exceptions/BadKeyException.h"
#include "exceptions/OpenSSLException.h"
#include "exceptions/UninitializedException.h"
#include "impl/DerivationPathUtils.h"
#include "impl/HexConverter.h"

#include <openssl/x509.h>

namespace Hedera
{
namespace
{
// The number of bytes in an ED25519PrivateKey.
constexpr const size_t PRIVATE_KEY_SIZE = 32;
// The number of bytes in an ED25519PrivateKey chain code.
constexpr const size_t CHAIN_CODE_SIZE = 32;
// The seed to use to compute the SHA512 HMAC, as defined in SLIP 10.
const std::vector<unsigned char> SLIP10_SEED = { 'e', 'd', '2', '5', '5', '1', '9', ' ', 's', 'e', 'e', 'd' };
// The algorithm identifier bytes for an ED25519PrivateKey.
const std::vector<unsigned char> ALGORITHM_IDENTIFIER_BYTES = { 0x30, 0x2E, 0x02, 0x01, 0x00, 0x30, 0x05, 0x06,
                                                                0x03, 0x2B, 0x65, 0x70, 0x04, 0x22, 0x04, 0x20 };
}

//-----
ED25519PrivateKey::ED25519PrivateKey(const ED25519PrivateKey& other)
  : PrivateKey()
  , mKeypair(bytesToPKEY(other.toBytes()))
  , mPublicKey(other.mPublicKey)
  , mChainCode(other.mChainCode)
{
}

//-----
ED25519PrivateKey& ED25519PrivateKey::operator=(const ED25519PrivateKey& other)
{
  if (this != &other)
  {
    mKeypair = bytesToPKEY(other.toBytes());
    mPublicKey = other.mPublicKey;
    mChainCode = other.mChainCode;
  }

  return *this;
}

//-----
ED25519PrivateKey::ED25519PrivateKey(ED25519PrivateKey&& other) noexcept
  : PrivateKey()
  , mKeypair(std::move(other.mKeypair))
  , mPublicKey(std::move(other.mPublicKey))
  , mChainCode(std::move(other.mChainCode))
{
}

//-----
ED25519PrivateKey& ED25519PrivateKey::operator=(ED25519PrivateKey&& other) noexcept
{
  mKeypair = std::move(other.mKeypair);
  mPublicKey = std::move(other.mPublicKey);
  mChainCode = std::move(other.mChainCode);
  return *this;
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::generatePrivateKey()
{
  const internal::OpenSSLUtils::EVP_PKEY_CTX keyAlgorithmContext(EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr));
  if (!keyAlgorithmContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_PKEY_CTX_new_id"));
  }

  if (EVP_PKEY_keygen_init(keyAlgorithmContext.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_PKEY_keygen_init"));
  }

  EVP_PKEY* keypair = nullptr;
  if (EVP_PKEY_generate(keyAlgorithmContext.get(), &keypair) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_PKEY_generate"));
  }

  return std::make_unique<ED25519PrivateKey>(ED25519PrivateKey(internal::OpenSSLUtils::EVP_PKEY(keypair)));
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromString(std::string_view keyString)
{
  try
  {
    return std::make_unique<ED25519PrivateKey>(
      ED25519PrivateKey(bytesToPKEY(internal::HexConverter::hexToBytes(keyString))));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(openSSLException.what());
  }
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromSeed(const std::vector<unsigned char>& seed)
{
  try
  {
    const std::vector<unsigned char> hmacOutput = internal::OpenSSLUtils::computeSHA512HMAC(SLIP10_SEED, seed);

    // The hmac is the key bytes followed by the chain code bytes
    return std::make_unique<ED25519PrivateKey>(ED25519PrivateKey(
      bytesToPKEY({ hmacOutput.begin(), hmacOutput.begin() + PRIVATE_KEY_SIZE }),
      { hmacOutput.begin() + PRIVATE_KEY_SIZE, hmacOutput.begin() + PRIVATE_KEY_SIZE + CHAIN_CODE_SIZE }));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(openSSLException.what());
  }
}

//-----
std::unique_ptr<PrivateKey> ED25519PrivateKey::clone() const
{
  return std::make_unique<ED25519PrivateKey>(*this);
}

//-----
std::shared_ptr<PublicKey> ED25519PrivateKey::getPublicKey() const
{
  return mPublicKey;
}

//-----
std::vector<unsigned char> ED25519PrivateKey::sign(const std::vector<unsigned char>& bytesToSign) const
{
  const internal::OpenSSLUtils::EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_CTX_new"));
  }

  if (EVP_DigestSignInit(messageDigestContext.get(), nullptr, nullptr, nullptr, mKeypair.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestSignInit"));
  }

  // Calculate the required size for the signature
  size_t signatureLength;
  if (EVP_DigestSign(messageDigestContext.get(),
                     nullptr,
                     &signatureLength,
                     (!bytesToSign.empty()) ? &bytesToSign.front() : nullptr,
                     bytesToSign.size()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestSign"));
  }

  std::vector<unsigned char> signature(signatureLength);
  if (EVP_DigestSign(messageDigestContext.get(),
                     &signature.front(),
                     &signatureLength,
                     (!bytesToSign.empty()) ? &bytesToSign.front() : nullptr,
                     bytesToSign.size()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestSign"));
  }

  return signature;
}

//-----
std::string ED25519PrivateKey::toString() const
{
  return internal::HexConverter::bytesToHex(toBytes());
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::derive(uint32_t childIndex) const
{
  if (mChainCode.empty())
  {
    throw UninitializedException("Key not initialized with chain code, unable to derive keys");
  }

  if (mChainCode.size() != CHAIN_CODE_SIZE)
  {
    throw BadKeyException("Key chain code malformed");
  }

  // as per SLIP0010, private key must be padded to 33 bytes
  std::vector<unsigned char> data = { 0x0 };
  data.reserve(37); // 37 bytes in byte data

  const std::vector<unsigned char> keyBytes = toBytes();
  data.insert(data.end(), keyBytes.cbegin(), keyBytes.cend());

  // Throws std::invalid_argument if childIndex is already hardened
  const std::vector<unsigned char> indexVector =
    internal::DerivationPathUtils::indexToBigEndianArray(internal::DerivationPathUtils::getHardenedIndex(childIndex));
  data.insert(data.end(), indexVector.cbegin(), indexVector.cend());

  const std::vector<unsigned char> hmacOutput = internal::OpenSSLUtils::computeSHA512HMAC(mChainCode, data);

  // The hmac is the key bytes followed by the chain code bytes
  return std::make_unique<ED25519PrivateKey>(ED25519PrivateKey(
    bytesToPKEY({ hmacOutput.begin(), hmacOutput.begin() + PRIVATE_KEY_SIZE }),
    { hmacOutput.begin() + PRIVATE_KEY_SIZE, hmacOutput.begin() + PRIVATE_KEY_SIZE + CHAIN_CODE_SIZE }));
}

//-----
std::vector<unsigned char> ED25519PrivateKey::toBytes() const
{
  int bytesLength = i2d_PrivateKey(mKeypair.get(), nullptr);

  std::vector<unsigned char> outputBytes(bytesLength);

  if (unsigned char* rawBytes = &outputBytes.front(); i2d_PrivateKey(mKeypair.get(), &rawBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PrivateKey"));
  }

  // don't return the algorithm identification bytes
  return { outputBytes.cbegin() + static_cast<long>(ALGORITHM_IDENTIFIER_BYTES.size()), outputBytes.cend() };
}

//-----
std::vector<unsigned char> ED25519PrivateKey::getChainCode() const
{
  return mChainCode;
}

//-----
internal::OpenSSLUtils::EVP_PKEY ED25519PrivateKey::bytesToPKEY(std::vector<unsigned char> keyBytes)
{
  // If there are only 32 key bytes, we need to add the algorithm identifier bytes, so that we can correctly decode
  if (keyBytes.size() == PRIVATE_KEY_SIZE)
  {
    keyBytes.insert(keyBytes.begin(), ALGORITHM_IDENTIFIER_BYTES.cbegin(), ALGORITHM_IDENTIFIER_BYTES.cend());
  }

  const unsigned char* rawKeyBytes = &keyBytes.front();
  internal::OpenSSLUtils::EVP_PKEY key(
    d2i_PrivateKey(EVP_PKEY_ED25519, nullptr, &rawKeyBytes, static_cast<long>(keyBytes.size())));
  if (!key)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("d2i_PrivateKey"));
  }

  return key;
}

//-----
ED25519PrivateKey::ED25519PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair)
  : PrivateKey()
  , mKeypair(std::move(keypair))
  , mPublicKey(ED25519PublicKey::fromBytes(getPublicKeyBytes()))
{
}

//-----
ED25519PrivateKey::ED25519PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair, std::vector<unsigned char> chainCode)
  : PrivateKey()
  , mKeypair(std::move(keypair))
  , mPublicKey(ED25519PublicKey::fromBytes(getPublicKeyBytes()))
  , mChainCode(std::move(chainCode))
{
}

//-----
std::vector<unsigned char> ED25519PrivateKey::getPublicKeyBytes() const
{
  int bytesLength = i2d_PUBKEY(mKeypair.get(), nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &publicKeyBytes.front(); i2d_PUBKEY(mKeypair.get(), &rawPublicKeyBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PUBKEY"));
  }

  return publicKeyBytes;
}

} // namespace Hedera

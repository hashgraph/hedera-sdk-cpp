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
#include "impl/DerivationPathUtils.h"
#include "impl/HexConverter.h"
#include "impl/OpenSSLHasher.h"

#include <openssl/x509.h>

namespace Hedera
{
namespace
{
const inline std::vector<unsigned char> ALGORITHM_IDENTIFIER_BYTES =
  internal::HexConverter::hexToBase64("302E020100300506032B657004220420");
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
  const internal::OpenSSL_EVP_PKEY_CTX keyAlgorithmContext(EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr));
  if (!keyAlgorithmContext)
  {
    throw std::runtime_error("Key algorithm context invalid");
  }

  if (EVP_PKEY_keygen_init(keyAlgorithmContext.get()) <= 0)
  {
    throw std::runtime_error("Keygen initialization error");
  }

  EVP_PKEY* keypair = nullptr;
  if (EVP_PKEY_generate(keyAlgorithmContext.get(), &keypair) <= 0)
  {
    throw std::runtime_error("Keypair generation error");
  }

  return std::make_unique<ED25519PrivateKey>(ED25519PrivateKey(internal::OpenSSL_EVP_PKEY(keypair)));
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromString(const std::string& keyString)
{
  return std::make_unique<ED25519PrivateKey>(
    ED25519PrivateKey(bytesToPKEY(internal::HexConverter::hexToBase64(keyString))));
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromSeed(const std::vector<unsigned char>& seed)
{
  static const std::string keyString = "ed25519 seed"; // as defined by SLIP 0010

  return fromHMACOutput(internal::OpenSSLHasher::computeSHA512HMAC({ keyString.begin(), keyString.end() }, seed));
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
  const internal::OpenSSL_EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw std::runtime_error("Digest context construction failed");
  }

  if (EVP_DigestSignInit(messageDigestContext.get(), nullptr, nullptr, nullptr, mKeypair.get()) <= 0)
  {
    throw std::runtime_error("Digest sign initialization failed");
  }

  // Calculate the required size for the signature
  size_t signatureLength;
  if (EVP_DigestSign(messageDigestContext.get(),
                     nullptr,
                     &signatureLength,
                     (!bytesToSign.empty()) ? &bytesToSign.front() : nullptr,
                     bytesToSign.size()) <= 0)
  {
    throw std::runtime_error("Failed to calculate signature length");
  }

  auto signature = std::vector<unsigned char>(signatureLength);
  if (EVP_DigestSign(messageDigestContext.get(),
                     &signature.front(),
                     &signatureLength,
                     (!bytesToSign.empty()) ? &bytesToSign.front() : nullptr,
                     bytesToSign.size()) <= 0)
  {
    throw std::runtime_error("Signature generation failed");
  }

  return signature;
}

//-----
std::string ED25519PrivateKey::toString() const
{
  return internal::HexConverter::base64ToHex(toBytes());
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::derive(const uint32_t childIndex) const
{
  if (mChainCode.size() != 32)
  {
    throw std::runtime_error("Key doesn't support derivation");
  }

  if (internal::DerivationPathUtils::isHardenedChildIndex(childIndex))
  {
    throw std::runtime_error("Child index should not be pre-hardened");
  }

  const uint32_t hardenedIndex = internal::DerivationPathUtils::getHardenedIndex(childIndex);

  // as per SLIP0010, private key must be padded to 33 bytes
  std::vector<unsigned char> data = { 0x0 };

  const std::vector<unsigned char> keyBytes = toBytes();
  data.insert(data.end(), keyBytes.begin(), keyBytes.end());

  // converts unsigned 32 bit int index into big endian byte array (ser32 function from BIP 32)
  std::vector<unsigned char> indexVector = internal::DerivationPathUtils::indexToBigEndianArray(hardenedIndex);
  data.insert(data.end(), indexVector.begin(), indexVector.end());

  return fromHMACOutput(internal::OpenSSLHasher::computeSHA512HMAC(mChainCode, data));
}

//-----
std::vector<unsigned char> ED25519PrivateKey::toBytes() const
{
  int bytesLength = i2d_PrivateKey(mKeypair.get(), nullptr);

  std::vector<unsigned char> outputBytes(bytesLength);

  if (unsigned char* rawBytes = &outputBytes.front(); i2d_PrivateKey(mKeypair.get(), &rawBytes) <= 0)
  {
    throw std::runtime_error("ED225519PrivateKey private key serialization error");
  }

  // don't return the algorithm identification bytes
  return { outputBytes.begin() + 16, outputBytes.end() };
}

//-----
std::vector<unsigned char> ED25519PrivateKey::getChainCode() const
{
  return mChainCode;
}

//-----
internal::OpenSSL_EVP_PKEY ED25519PrivateKey::bytesToPKEY(const std::vector<unsigned char>& keyBytes)
{
  std::vector<unsigned char> fullKeyBytes;
  // If there are only 32 key bytes, we need to add the algorithm identifier bytes, so that OpenSSL can correctly
  // decode
  if (keyBytes.size() == 32)
  {
    fullKeyBytes = prependAlgorithmIdentifier(keyBytes);
  }
  else
  {
    fullKeyBytes = keyBytes;
  }

  const unsigned char* rawKeyBytes = &fullKeyBytes.front();
  return internal::OpenSSL_EVP_PKEY(
    d2i_PrivateKey(EVP_PKEY_ED25519, nullptr, &rawKeyBytes, static_cast<long>(fullKeyBytes.size())));
}

//-----
std::vector<unsigned char> ED25519PrivateKey::prependAlgorithmIdentifier(const std::vector<unsigned char>& keyBytes)
{
  // full key will begin with the algorithm identifier bytes
  std::vector<unsigned char> fullKey = ALGORITHM_IDENTIFIER_BYTES;

  // insert the raw key bytes onto the end of the full key
  fullKey.insert(fullKey.end(), keyBytes.begin(), keyBytes.end());

  return fullKey;
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromHMACOutput(const std::vector<unsigned char>& hmacOutput)
{
  if (hmacOutput.size() != 64)
  {
    throw std::runtime_error("Unexpected size for hmac output");
  }

  // the first 32 bytes of the hmac are the new key material. the algorithm identifier must come first, though
  std::vector<unsigned char> key(hmacOutput.begin(), hmacOutput.begin() + 32);

  // chain code is the next 32 bytes of the computed hmac
  std::vector<unsigned char> chainCode(hmacOutput.begin() + 32, hmacOutput.end());

  return std::make_unique<ED25519PrivateKey>(ED25519PrivateKey(bytesToPKEY(key), chainCode));
}

//-----
ED25519PrivateKey::ED25519PrivateKey(internal::OpenSSL_EVP_PKEY&& keypair)
  : PrivateKey()
  , mKeypair(std::move(keypair))
  , mPublicKey(ED25519PublicKey::fromBytes(getPublicKeyBytes()))
{
}

//-----
ED25519PrivateKey::ED25519PrivateKey(internal::OpenSSL_EVP_PKEY&& keypair, std::vector<unsigned char> chainCode)
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
    throw std::runtime_error("ED225519PrivateKey public key serialization error");
  }

  return publicKeyBytes;
}

} // namespace Hedera

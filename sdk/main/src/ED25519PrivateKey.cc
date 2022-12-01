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
#include "impl/HexConverter.h"

#include <openssl/x509.h>

namespace Hedera
{
//-----
ED25519PrivateKey::~ED25519PrivateKey()
{
  EVP_PKEY_free(mKeypair);
}

//-----
ED25519PrivateKey::ED25519PrivateKey(const ED25519PrivateKey& other)
  : PrivateKey()
  , mKeypair(bytesToPKEY(other.toBytes()))
  , mPublicKey(other.mPublicKey)
{
}

//-----
ED25519PrivateKey& ED25519PrivateKey::operator=(const ED25519PrivateKey& other)
{
  if (this != &other)
  {
    mKeypair = bytesToPKEY(other.toBytes());
    mPublicKey = other.mPublicKey;
  }

  return *this;
}

//-----
ED25519PrivateKey::ED25519PrivateKey(ED25519PrivateKey&& other) noexcept
  : PrivateKey()
  , mKeypair(other.mKeypair)
  , mPublicKey(std::move(other.mPublicKey))
{
  other.mKeypair = nullptr;
}

//-----
ED25519PrivateKey& ED25519PrivateKey::operator=(ED25519PrivateKey&& other) noexcept
{
  mKeypair = other.mKeypair;
  mPublicKey = other.mPublicKey;
  other.mKeypair = nullptr;
  return *this;
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::generatePrivateKey()
{
  EVP_PKEY* keypair = EVP_PKEY_new();
  EVP_PKEY_CTX* keyAlgorithmContext = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr);

  if (!keyAlgorithmContext)
  {
    EVP_PKEY_free(keypair);
    throw std::runtime_error("Key algorithm context invalid");
  }

  if (EVP_PKEY_keygen_init(keyAlgorithmContext) <= 0)
  {
    EVP_PKEY_free(keypair);
    EVP_PKEY_CTX_free(keyAlgorithmContext);
    throw std::runtime_error("Keygen initialization error");
  }

  if (EVP_PKEY_generate(keyAlgorithmContext, &keypair) <= 0)
  {
    EVP_PKEY_free(keypair);
    EVP_PKEY_CTX_free(keyAlgorithmContext);
    throw std::runtime_error("Keypair generation error");
  }

  EVP_PKEY_CTX_free(keyAlgorithmContext);

  return std::make_unique<ED25519PrivateKey>(ED25519PrivateKey(keypair));
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromString(const std::string& keyString)
{
  std::string fullKeyString = keyString;

  // key size of 64 means RFC 8410 prefix is missing. add it before making calls to OpenSSL
  if (keyString.size() == 64)
  {
    fullKeyString = "302E020100300506032B657004220420" + keyString;
  }

  return std::make_unique<ED25519PrivateKey>(
    ED25519PrivateKey(bytesToPKEY(internal::HexConverter::hexToBase64(fullKeyString))));
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
  EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_new();

  if (!messageDigestContext)
  {
    throw std::runtime_error("Digest context construction failed");
  }

  if (EVP_DigestSignInit(messageDigestContext, nullptr, nullptr, nullptr, mKeypair) <= 0)
  {
    EVP_MD_CTX_free(messageDigestContext);
    throw std::runtime_error("Digest sign initialization failed");
  }

  size_t signatureLength;
  /* Calculate the required size for the signature */
  if (EVP_DigestSign(messageDigestContext, nullptr, &signatureLength, &bytesToSign.front(), bytesToSign.size()) <= 0)
  {
    EVP_MD_CTX_free(messageDigestContext);
    throw std::runtime_error("Failed to calculate signature length");
  }

  auto signature = std::vector<unsigned char>(signatureLength);

  if (EVP_DigestSign(
        messageDigestContext, &signature.front(), &signatureLength, &bytesToSign.front(), bytesToSign.size()) <= 0)
  {
    EVP_MD_CTX_free(messageDigestContext);
    throw std::runtime_error("Signature generation failed");
  }

  EVP_MD_CTX_free(messageDigestContext);

  return signature;
}

//-----
std::string ED25519PrivateKey::toString() const
{
  return internal::HexConverter::base64ToHex(toBytes());
}

//-----
ED25519PrivateKey::ED25519PrivateKey(EVP_PKEY* keypair)
  : PrivateKey()
  , mKeypair(keypair)
  , mPublicKey(ED25519PublicKey::fromBytes(getPublicKeyBytes()))
{
}

//-----
std::vector<unsigned char> ED25519PrivateKey::toBytes() const
{
  int bytesLength = i2d_PrivateKey(mKeypair, nullptr);

  std::vector<unsigned char> outputBytes(bytesLength);

  if (unsigned char* rawBytes = &outputBytes.front(); i2d_PrivateKey(mKeypair, &rawBytes) <= 0)
  {
    throw std::runtime_error("ED225519PrivateKey private key serialization error");
  }

  return outputBytes;
}

//-----
std::vector<unsigned char> ED25519PrivateKey::getPublicKeyBytes() const
{
  int bytesLength = i2d_PUBKEY(mKeypair, nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &publicKeyBytes.front(); i2d_PUBKEY(mKeypair, &rawPublicKeyBytes) <= 0)
  {
    throw std::runtime_error("ED225519PrivateKey public key serialization error");
  }

  return publicKeyBytes;
}

//-----
EVP_PKEY* ED25519PrivateKey::bytesToPKEY(const std::vector<unsigned char>& keyBytes)
{
  const unsigned char* rawKeyBytes = &keyBytes.front();
  return d2i_PrivateKey(EVP_PKEY_ED25519, nullptr, &rawKeyBytes, (long)keyBytes.size());
}

} // namespace Hedera

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
#include "impl/PrivateKeyImpl.h"
#include "impl/Utilities.h"
#include "impl/openssl_utils/EVP_MD_CTX.h"
#include "impl/openssl_utils/EVP_PKEY_CTX.h"
#include "impl/openssl_utils/OpenSSLUtils.h"

#include <openssl/x509.h>

namespace Hedera
{
namespace
{
// The seed to use to compute the SHA512 HMAC, as defined in SLIP 10.
const std::vector<std::byte> SLIP10_SEED = { std::byte('e'), std::byte('d'), std::byte('2'), std::byte('5'),
                                             std::byte('5'), std::byte('1'), std::byte('9'), std::byte(' '),
                                             std::byte('s'), std::byte('e'), std::byte('e'), std::byte('d') };

/**
 * Create a wrapped OpenSSL key object from a byte vector (raw or DER-encoded) representing an ED25519PrivateKey.
 *
 * @param bytes The bytes representing a ED25519PrivateKey.
 * @return The newly created wrapped OpenSSL keypair object.
 * @throws OpenSSLException If OpenSSL is unable to create a keypair from the input bytes.
 */
[[nodiscard]] internal::OpenSSLUtils::EVP_PKEY bytesToPKEY(std::vector<std::byte> bytes)
{
  if (bytes.size() == ED25519PrivateKey::KEY_SIZE)
  {
    bytes = internal::Utilities::concatenateVectors({ ED25519PrivateKey::DER_ENCODED_PREFIX_BYTES, bytes });
  }

  const unsigned char* rawKeyBytes = internal::OpenSSLUtils::toUnsignedCharPtr(bytes.data());
  internal::OpenSSLUtils::EVP_PKEY key(
    d2i_PrivateKey(EVP_PKEY_ED25519, nullptr, &rawKeyBytes, static_cast<long>(bytes.size())));
  if (!key)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("d2i_PrivateKey"));
  }

  return key;
}
} // namespace

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::generatePrivateKey()
{
  internal::OpenSSLUtils::EVP_PKEY_CTX keyAlgorithmContext(EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, nullptr));
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
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromString(std::string_view key)
{
  if (key.size() == KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size() || key.size() == KEY_SIZE * 2)
  {
    try
    {
      return std::make_unique<ED25519PrivateKey>(
        ED25519PrivateKey(bytesToPKEY(internal::HexConverter::hexToBytes(key))));
    }
    catch (const OpenSSLException& openSSLException)
    {
      throw BadKeyException(std::string("ED25519PrivateKey cannot be realized from input string: ") +
                            openSSLException.what());
    }
  }
  else
  {
    throw BadKeyException("ED25519PrivateKey cannot be realized from input string: input string size should be " +
                          std::to_string(KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size()) + " or " +
                          std::to_string(KEY_SIZE * 2));
  }
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromBytes(const std::vector<std::byte>& bytes)
{
  if (bytes.size() == KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size() || bytes.size() == KEY_SIZE)
  {
    try
    {
      return std::make_unique<ED25519PrivateKey>(ED25519PrivateKey(bytesToPKEY(bytes)));
    }
    catch (const OpenSSLException& openSSLException)
    {
      throw BadKeyException(std::string("ED25519PrivateKey cannot be realized from input bytes: ") +
                            openSSLException.what());
    }
  }
  else
  {
    throw BadKeyException("ED25519PrivateKey cannot be realized from input bytes: input byte array size should be " +
                          std::to_string(KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size()) + " or " +
                          std::to_string(KEY_SIZE));
  }
}

//-----
std::unique_ptr<ED25519PrivateKey> ED25519PrivateKey::fromSeed(const std::vector<std::byte>& seed)
{
  try
  {
    const std::vector<std::byte> hmacOutput = internal::OpenSSLUtils::computeSHA512HMAC(SLIP10_SEED, seed);

    // The hmac is the key bytes followed by the chain code bytes
    return std::make_unique<ED25519PrivateKey>(
      ED25519PrivateKey(bytesToPKEY({ hmacOutput.cbegin(), hmacOutput.cbegin() + KEY_SIZE }),
                        { hmacOutput.cbegin() + KEY_SIZE, hmacOutput.cbegin() + KEY_SIZE + CHAIN_CODE_SIZE }));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(std::string("ED25519PrivateKey cannot be realized from input seed bytes: ") +
                          openSSLException.what());
  }
}

//-----
std::unique_ptr<PrivateKey> ED25519PrivateKey::clone() const
{
  return std::make_unique<ED25519PrivateKey>(*this);
}

//-----
std::vector<std::byte> ED25519PrivateKey::sign(const std::vector<std::byte>& bytesToSign) const
{
  internal::OpenSSLUtils::EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_CTX_new"));
  }

  if (EVP_DigestSignInit(messageDigestContext.get(), nullptr, nullptr, nullptr, getInternalKey().get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestSignInit"));
  }

  // Calculate the required size for the signature
  size_t signatureLength;
  if (EVP_DigestSign(messageDigestContext.get(),
                     nullptr,
                     &signatureLength,
                     internal::OpenSSLUtils::toUnsignedCharPtr(bytesToSign.data()),
                     bytesToSign.size()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestSign"));
  }

  std::vector<std::byte> signature(signatureLength);
  if (EVP_DigestSign(messageDigestContext.get(),
                     internal::OpenSSLUtils::toUnsignedCharPtr(signature.data()),
                     &signatureLength,
                     internal::OpenSSLUtils::toUnsignedCharPtr(bytesToSign.data()),
                     bytesToSign.size()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestSign"));
  }

  return signature;
}

//-----
std::string ED25519PrivateKey::toStringDer() const
{
  return internal::HexConverter::bytesToHex(toBytesDer());
}

//-----
std::string ED25519PrivateKey::toStringRaw() const
{
  return internal::HexConverter::bytesToHex(toBytesRaw());
}

//-----
std::vector<std::byte> ED25519PrivateKey::toBytesDer() const
{
  int bytesLength = i2d_PrivateKey(getInternalKey().get(), nullptr);

  std::vector<std::byte> outputBytes(bytesLength);

  if (unsigned char* rawBytes = internal::OpenSSLUtils::toUnsignedCharPtr(outputBytes.data());
      i2d_PrivateKey(getInternalKey().get(), &rawBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PrivateKey"));
  }

  return outputBytes;
}

//-----
std::vector<std::byte> ED25519PrivateKey::toBytesRaw() const
{
  return internal::Utilities::removePrefix(toBytesDer(), static_cast<long>(DER_ENCODED_PREFIX_BYTES.size()));
}

//-----
std::unique_ptr<PrivateKey> ED25519PrivateKey::derive(uint32_t childIndex) const
{
  if (getChainCode().empty())
  {
    throw UninitializedException("Key not initialized with chain code, unable to derive keys");
  }

  // As per SLIP0010, private key must be padded to 33 bytes
  const std::vector<std::byte> hmacOutput = internal::OpenSSLUtils::computeSHA512HMAC(
    getChainCode(),
    internal::Utilities::concatenateVectors({ { std::byte(0x0) },
                                              toBytesRaw(),
                                              internal::DerivationPathUtils::indexToBigEndianArray(
                                                internal::DerivationPathUtils::getHardenedIndex(childIndex)) }));

  // The hmac is the key bytes followed by the chain code bytes
  return std::make_unique<ED25519PrivateKey>(
    ED25519PrivateKey(bytesToPKEY({ hmacOutput.cbegin(), hmacOutput.cbegin() + KEY_SIZE }),
                      { hmacOutput.cbegin() + KEY_SIZE, hmacOutput.cbegin() + KEY_SIZE + CHAIN_CODE_SIZE }));
}

//-----
ED25519PrivateKey::ED25519PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& key, std::vector<std::byte> chainCode)
  : PrivateKey(std::move(key), std::move(chainCode))
{
}

} // namespace Hedera

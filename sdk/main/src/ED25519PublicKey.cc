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
//-----
ED25519PublicKey::ED25519PublicKey(const ED25519PublicKey& other)
  : mPublicKey(rawBytesToPKEY(other.toBytesRaw()))
{
}

//-----
ED25519PublicKey& ED25519PublicKey::operator=(const ED25519PublicKey& other)
{
  if (this != &other)
  {
    mPublicKey = rawBytesToPKEY(other.toBytesRaw());
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
std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromString(std::string_view key)
{
  if (key.size() == PUBLIC_KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size())
  {
    return fromStringDer(key);
  }
  else if (key.size() == PUBLIC_KEY_SIZE * 2)
  {
    return fromStringRaw(key);
  }
  else
  {
    throw BadKeyException("ED25519PublicKey cannot be realized from input string: input string size should be " +
                          std::to_string(PUBLIC_KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size()) + " or " +
                          std::to_string(PUBLIC_KEY_SIZE * 2));
  }
}

//-----
std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromStringDer(std::string_view key)
{
  if (key.size() != PUBLIC_KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size())
  {
    throw BadKeyException(
      "ED25519PublicKey cannot be realized from input string: DER encoded ED25519PublicKey hex string size should be " +
      std::to_string(PUBLIC_KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size()));
  }

  // Remove the DER-encoded header
  key.remove_prefix(DER_ENCODED_PREFIX_HEX.size());
  return fromStringRaw(key);
}

//-----
std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromStringRaw(std::string_view key)
{
  if (key.size() != PUBLIC_KEY_SIZE * 2)
  {
    throw BadKeyException(
      "ED25519PublicKey cannot be realized from input string: raw ED25519PublicKey string size should be " +
      std::to_string(PUBLIC_KEY_SIZE * 2));
  }

  try
  {
    return std::make_shared<ED25519PublicKey>(
      ED25519PublicKey(rawBytesToPKEY(internal::HexConverter::hexToBytes(key))));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(std::string("ED25519PublicKey cannot be realized from input string: ") +
                          openSSLException.what());
  }
  catch (const std::invalid_argument& invalidArgument)
  {
    throw BadKeyException(std::string("ED25519PublicKey cannot be realized from input string: ") +
                          invalidArgument.what());
  }
}

//-----
std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromBytes(const std::vector<unsigned char>& bytes)
{
  if (bytes.size() == PUBLIC_KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size())
  {
    return fromBytesDer(bytes);
  }
  else if (bytes.size() == PUBLIC_KEY_SIZE)
  {
    return fromBytesRaw(bytes);
  }
  else
  {
    throw BadKeyException("ED25519PublicKey cannot be realized from input bytes: input byte array size should be " +
                          std::to_string(PUBLIC_KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size()) + " or " +
                          std::to_string(PUBLIC_KEY_SIZE));
  }
}

//-----
std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromBytesDer(const std::vector<unsigned char>& bytes)
{
  if (bytes.size() != PUBLIC_KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size())
  {
    throw BadKeyException(
      "ED25519PublicKey cannot be realized from input bytes: DER encoded ED25519PublicKey byte array should contain " +
      std::to_string(PUBLIC_KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size()) + " bytes");
  }

  // Remove the DER-encoded header
  return fromBytesRaw({ bytes.cbegin() + static_cast<long>(DER_ENCODED_PREFIX_BYTES.size()), bytes.cend() });
}

//-----
std::shared_ptr<ED25519PublicKey> ED25519PublicKey::fromBytesRaw(const std::vector<unsigned char>& bytes)
{
  if (bytes.size() != PUBLIC_KEY_SIZE)
  {
    throw BadKeyException(
      "ED25519PublicKey cannot be realized from input bytes: raw ED25519PublicKey byte array should contain " +
      std::to_string(PUBLIC_KEY_SIZE) + " bytes");
  }

  try
  {
    return std::make_shared<ED25519PublicKey>(ED25519PublicKey(rawBytesToPKEY(bytes)));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(std::string("ED25519PublicKey cannot be realized from input bytes: ") +
                          openSSLException.what());
  }
  catch (const std::invalid_argument& invalidArgument)
  {
    throw BadKeyException(std::string("ED25519PublicKey cannot be realized from input bytes: ") +
                          invalidArgument.what());
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

//-----
std::string ED25519PublicKey::toStringDer() const
{
  return internal::HexConverter::bytesToHex(toBytesDer());
}

//-----
std::string ED25519PublicKey::toStringRaw() const
{
  return internal::HexConverter::bytesToHex(toBytesRaw());
}

//-----
std::vector<unsigned char> ED25519PublicKey::toBytesDer() const
{
  int bytesLength = i2d_PUBKEY(mPublicKey.get(), nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &publicKeyBytes.front(); i2d_PUBKEY(mPublicKey.get(), &rawPublicKeyBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PUBKEY"));
  }

  return publicKeyBytes;
}

//-----
std::vector<unsigned char> ED25519PublicKey::toBytesRaw() const
{
  std::vector<unsigned char> bytes = toBytesDer();
  return { bytes.cbegin() + static_cast<long>(DER_ENCODED_PREFIX_BYTES.size()), bytes.cend() };
}

//----
std::unique_ptr<proto::Key> ED25519PublicKey::toProtobuf() const
{
  auto keyProtobuf = std::make_unique<proto::Key>();
  const std::vector<unsigned char> rawBytes = toBytesRaw();
  keyProtobuf->set_allocated_ed25519(new std::string({ rawBytes.cbegin(), rawBytes.cend() }));
  return keyProtobuf;
}

//-----
internal::OpenSSLUtils::EVP_PKEY ED25519PublicKey::rawBytesToPKEY(std::vector<unsigned char> bytes)
{
  if (bytes.size() == PUBLIC_KEY_SIZE)
  {
    bytes.insert(bytes.begin(), DER_ENCODED_PREFIX_BYTES.cbegin(), DER_ENCODED_PREFIX_BYTES.cend());
  }
  else if (bytes.size() != PUBLIC_KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size())
  {
    throw std::invalid_argument("Input bytes size [" + std::to_string(bytes.size()) + "] is invalid: must be either [" +
                                std::to_string(PUBLIC_KEY_SIZE) + "] or [" +
                                std::to_string(PUBLIC_KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size()) + "]");
  }

  const unsigned char* bytesPtr = &bytes.front();
  internal::OpenSSLUtils::EVP_PKEY key(d2i_PUBKEY(nullptr, &bytesPtr, static_cast<long>(bytes.size())));
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

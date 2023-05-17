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
#include "ECDSAsecp256k1PrivateKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "exceptions/BadKeyException.h"
#include "exceptions/OpenSSLException.h"
#include "exceptions/UninitializedException.h"
#include "impl/DerivationPathUtils.h"
#include "impl/HexConverter.h"
#include "impl/PrivateKeyImpl.h"
#include "impl/Utilities.h"
#include "impl/openssl_utils/BIGNUM.h"
#include "impl/openssl_utils/ECDSA_SIG.h"
#include "impl/openssl_utils/EVP_MD.h"
#include "impl/openssl_utils/EVP_MD_CTX.h"
#include "impl/openssl_utils/OSSL_LIB_CTX.h"
#include "impl/openssl_utils/OpenSSLUtils.h"

#include <openssl/ec.h>
#include <openssl/x509.h>
#include <proto/basic_types.pb.h>

namespace Hedera
{
namespace
{
// The seed to use to compute the SHA512 HMAC, as defined in BIP32.
const std::vector<std::byte> BIP32_SEED = { std::byte('B'), std::byte('i'), std::byte('t'), std::byte('c'),
                                            std::byte('o'), std::byte('i'), std::byte('n'), std::byte(' '),
                                            std::byte('s'), std::byte('e'), std::byte('e'), std::byte('d') };
// The ASN.1 algorithm identifier prefix bytes for an ECDSAsecp256k1PrivateKey.
const std::vector<std::byte> ASN1_PREFIX_BYTES = { std::byte(0x30), std::byte(0x2E), std::byte(0x02), std::byte(0x01),
                                                   std::byte(0x01), std::byte(0x04), std::byte(0x20) };
// The ASN.1 algorithm identifier suffix bytes for an ECDSAsecp256k1PrivateKey.
const std::vector<std::byte> ASN1_SUFFIX_BYTES = { std::byte(0xA0), std::byte(0x07), std::byte(0x06),
                                                   std::byte(0x05), std::byte(0x2B), std::byte(0x81),
                                                   std::byte(0x04), std::byte(0x00), std::byte(0x0A) };
// The order of the secp256k1 curve.
const internal::OpenSSLUtils::BIGNUM CURVE_ORDER =
  internal::OpenSSLUtils::BIGNUM::fromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");

/**
 * Create a wrapped OpenSSL key object from a byte vector (raw or DER-encoded) representing an ECDSAsecp256k1PrivateKey.
 *
 * @param bytes The bytes representing a ECDSAsecp256k1PrivateKey.
 * @return The newly created wrapped OpenSSL keypair object.
 * @throws OpenSSLException If OpenSSL is unable to create a keypair from the input bytes.
 */
[[nodiscard]] internal::OpenSSLUtils::EVP_PKEY bytesToPKEY(const std::vector<std::byte>& bytes)
{
  const std::vector<std::byte> formattedBytes = internal::Utilities::concatenateVectors(
    { ASN1_PREFIX_BYTES,
      // Remove the DER-encoding header if need be
      (bytes.size() == ECDSAsecp256k1PrivateKey::KEY_SIZE + ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_BYTES.size() &&
       internal::Utilities::isPrefixOf(bytes, ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_BYTES))
        ? internal::Utilities::removePrefix(
            bytes, static_cast<long>(ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_BYTES.size()))
        : bytes,
      ASN1_SUFFIX_BYTES });

  auto rawKeyBytes = internal::Utilities::toTypePtr<unsigned char>(formattedBytes.data());
  internal::OpenSSLUtils::EVP_PKEY key(
    d2i_PrivateKey(EVP_PKEY_EC, nullptr, &rawKeyBytes, static_cast<long>(formattedBytes.size())));
  if (!key)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("d2i_PrivateKey"));
  }

  return key;
}

} // namespace

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::generatePrivateKey()
{
  internal::OpenSSLUtils::EVP_PKEY key(EVP_EC_gen("secp256k1"));
  if (!key)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_EC_gen"));
  }

  return std::make_unique<ECDSAsecp256k1PrivateKey>(ECDSAsecp256k1PrivateKey(std::move(key)));
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromString(std::string_view key)
{
  if (key.size() != KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size() && key.size() != KEY_SIZE * 2)
  {
    throw BadKeyException(
      "ECDSAsecp256k1PrivateKey cannot be realized from input string: input string size should be " +
      std::to_string(KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size()) + " or " + std::to_string(KEY_SIZE * 2));
  }

  try
  {
    return std::make_unique<ECDSAsecp256k1PrivateKey>(
      ECDSAsecp256k1PrivateKey(bytesToPKEY(internal::HexConverter::hexToBytes(key))));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(std::string("ECDSAsecp256k1PrivateKey cannot be realized from input string: ") +
                          openSSLException.what());
  }
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromBytes(const std::vector<std::byte>& bytes)
{
  if (bytes.size() != KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size() && bytes.size() != KEY_SIZE)
  {
    throw BadKeyException(
      "ECDSAsecp256k1PrivateKey cannot be realized from input bytes: input byte array size should be " +
      std::to_string(KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size()) + " or " + std::to_string(KEY_SIZE));
  }

  try
  {
    return std::make_unique<ECDSAsecp256k1PrivateKey>(ECDSAsecp256k1PrivateKey(bytesToPKEY(bytes)));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(std::string("ECDSAsecp256k1PrivateKey cannot be realized from input bytes: ") +
                          openSSLException.what());
  }
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromSeed(const std::vector<std::byte>& seed)
{
  try
  {
    const std::vector<std::byte> hmacOutput = internal::OpenSSLUtils::computeSHA512HMAC(BIP32_SEED, seed);

    // The hmac is the key bytes followed by the chain code bytes
    return std::make_unique<ECDSAsecp256k1PrivateKey>(
      ECDSAsecp256k1PrivateKey(bytesToPKEY({ hmacOutput.cbegin(), hmacOutput.cbegin() + KEY_SIZE }),
                               { hmacOutput.cbegin() + KEY_SIZE, hmacOutput.cbegin() + KEY_SIZE + CHAIN_CODE_SIZE }));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(openSSLException.what());
  }
}

//-----
std::unique_ptr<Key> ECDSAsecp256k1PrivateKey::clone() const
{
  return std::make_unique<ECDSAsecp256k1PrivateKey>(*this);
}

//-----
std::unique_ptr<proto::Key> ECDSAsecp256k1PrivateKey::toProtobuf() const
{
  return getPublicKey()->toProtobuf();
}

//-----
std::unique_ptr<PrivateKey> ECDSAsecp256k1PrivateKey::derive(uint32_t childIndex) const
{
  if (getChainCode().empty())
  {
    throw UninitializedException("Key not initialized with chain code, unable to derive keys");
  }

  const std::vector<std::byte> hmacOutput = internal::OpenSSLUtils::computeSHA512HMAC(
    getChainCode(),
    internal::Utilities::concatenateVectors(
      { (internal::DerivationPathUtils::isHardenedChildIndex(childIndex))
          ? internal::Utilities::concatenateVectors({ { std::byte(0x0) }, toBytesRaw() })
          : getPublicKey()->toBytesRaw(),
        internal::DerivationPathUtils::indexToBigEndianArray(childIndex) }));

  // Modular add the private key bytes computed from the HMAC to the existing private key (using the secp256k1 curve
  // order as the modulo), and compute the new chain code from the HMAC
  return std::make_unique<ECDSAsecp256k1PrivateKey>(ECDSAsecp256k1PrivateKey(
    bytesToPKEY(internal::OpenSSLUtils::BIGNUM::fromBytes({ hmacOutput.cbegin(), hmacOutput.cbegin() + KEY_SIZE })
                  .modularAdd(internal::OpenSSLUtils::BIGNUM::fromBytes(toBytesRaw()), CURVE_ORDER)
                  .toBytes()),
    { hmacOutput.cbegin() + KEY_SIZE, hmacOutput.cbegin() + KEY_SIZE + CHAIN_CODE_SIZE }));
}

//-----
std::vector<std::byte> ECDSAsecp256k1PrivateKey::sign(const std::vector<std::byte>& bytesToSign) const
{
  internal::OpenSSLUtils::OSSL_LIB_CTX libraryContext(OSSL_LIB_CTX_new());
  if (!libraryContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OSSL_LIB_CTX_new"));
  }

  const internal::OpenSSLUtils::EVP_MD messageDigest(EVP_MD_fetch(libraryContext.get(), "KECCAK-256", nullptr));
  if (!messageDigest)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_fetch"));
  }

  internal::OpenSSLUtils::EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_CTX_new"));
  }

  if (EVP_DigestSignInit(messageDigestContext.get(), nullptr, messageDigest.get(), nullptr, getInternalKey().get()) <=
      0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestSignInit"));
  }

  // 72 is the maximum required size. actual signature may be slightly smaller
  size_t signatureLength = MAX_SIGNATURE_SIZE;
  std::vector<std::byte> signature(signatureLength);

  if (EVP_DigestSign(messageDigestContext.get(),
                     internal::Utilities::toTypePtr<unsigned char>(signature.data()),
                     &signatureLength,
                     internal::Utilities::toTypePtr<unsigned char>(bytesToSign.data()),
                     bytesToSign.size()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestSign"));
  }

  // we have the signature complete, now we need to turn it into its raw form of (r,s)

  const unsigned char* signaturePointer = internal::Utilities::toTypePtr<unsigned char>(signature.data());
  const internal::OpenSSLUtils::ECDSA_SIG signatureObject(
    d2i_ECDSA_SIG(nullptr, &signaturePointer, static_cast<long>(signatureLength)));
  if (!signatureObject)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("d2i_ECDSA_SIG"));
  }

  const BIGNUM* signatureR = ECDSA_SIG_get0_r(signatureObject.get());
  if (!signatureR)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("ECDSA_SIG_get0_r"));
  }

  const BIGNUM* signatureS = ECDSA_SIG_get0_s(signatureObject.get());
  if (!signatureS)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("ECDSA_SIG_get0_s"));
  }

  // signature is returned in the raw, 64 byte form (r, s)
  std::vector<std::byte> outputArray(RAW_SIGNATURE_SIZE);

  if (BN_bn2binpad(signatureR, internal::Utilities::toTypePtr<unsigned char>(outputArray.data()), R_SIZE) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_bn2binpad"));
  }

  if (BN_bn2binpad(signatureS, internal::Utilities::toTypePtr<unsigned char>(outputArray.data()) + R_SIZE, S_SIZE) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_bn2binpad"));
  }

  return outputArray;
}

//-----
std::string ECDSAsecp256k1PrivateKey::toStringDer() const
{
  return internal::HexConverter::bytesToHex(toBytesDer());
}

//-----
std::string ECDSAsecp256k1PrivateKey::toStringRaw() const
{
  return internal::HexConverter::bytesToHex(toBytesRaw());
}

//-----
std::vector<std::byte> ECDSAsecp256k1PrivateKey::toBytesDer() const
{
  return internal::Utilities::concatenateVectors({ DER_ENCODED_PREFIX_BYTES, toBytesRaw() });
}

//-----
std::vector<std::byte> ECDSAsecp256k1PrivateKey::toBytesRaw() const
{
  std::vector<std::byte> outputBytes(i2d_PrivateKey(getInternalKey().get(), nullptr));

  if (auto rawBytes = internal::Utilities::toTypePtr<unsigned char>(outputBytes.data());
      i2d_PrivateKey(getInternalKey().get(), &rawBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PrivateKey"));
  }

  // The return value of i2d_PrivateKey can be either 48 or 118 bytes, depending on how the private key was constructed.
  // This difference doesn't change anything with the return here: the first 7 bytes of each are algorithm identifiers,
  // the next 32 are private key bytes, and the rest are for other purposes.
  return { outputBytes.cbegin() + static_cast<long>(ASN1_PREFIX_BYTES.size()),
           outputBytes.cbegin() + static_cast<long>(ASN1_PREFIX_BYTES.size()) + KEY_SIZE };
}

//-----
ECDSAsecp256k1PrivateKey::ECDSAsecp256k1PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& key,
                                                   std::vector<std::byte> chainCode)
  : PrivateKey(std::move(key), std::move(chainCode))
{
}

} // namespace Hedera

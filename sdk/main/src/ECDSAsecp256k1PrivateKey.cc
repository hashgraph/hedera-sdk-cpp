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
#include "exceptions/BadKeyException.h"
#include "exceptions/OpenSSLException.h"
#include "exceptions/UninitializedException.h"
#include "impl/DerivationPathUtils.h"
#include "impl/HexConverter.h"
#include "impl/OpenSSLUtils.h"
#include "impl/Utilities.h"

#include <openssl/ec.h>
#include <openssl/x509.h>

namespace Hedera
{
namespace
{
// The number of bytes in an ECDSAsecp256k1PrivateKey.
constexpr const size_t PRIVATE_KEY_SIZE = 32;
// The number of bytes in an ECDSAsecp256k1PrivateKey chain code.
constexpr const size_t CHAIN_CODE_SIZE = 32;
// The seed to use to compute the SHA512 HMAC, as defined in BIP32.
const std::vector<unsigned char> BIP32_SEED = { 'B', 'i', 't', 'c', 'o', 'i', 'n', ' ', 's', 'e', 'e', 'd' };
// The ASN.1 algorithm identifier prefix bytes for an ECDSAsecp256k1PrivateKey.
const std::vector<unsigned char> ASN1_PREFIX_BYTES = { 0x30, 0x2E, 0x02, 0x01, 0x01, 0x04, 0x20 };
// The ASN.1 algorithm identifier prefix hex for an ECDSAsecp256k1PrivateKey.
const std::string ASN1_PREFIX_HEX = "302E0201010420";
// The ASN.1 algorithm identifier suffix bytes for an ECDSAsecp256k1PrivateKey.
const std::vector<unsigned char> ASN1_SUFFIX_BYTES = { 0xA0, 0x07, 0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x0A };
// The ASN.1 algorithm identifier suffix hex for an ECDSAsecp256k1PrivateKey.
const std::string ASN1_SUFFIX_HEX = "A00706052B8104000A";
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
[[nodiscard]] internal::OpenSSLUtils::EVP_PKEY bytesToPKEY(const std::vector<unsigned char>& bytes)
{
  const std::vector<unsigned char> formattedBytes = internal::Utilities::concatenateVectors(
    ASN1_PREFIX_BYTES,
    (bytes.size() == PRIVATE_KEY_SIZE + ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_BYTES.size())
      ? internal::Utilities::removePrefix(bytes,
                                          static_cast<long>(ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_BYTES.size()))
      : bytes,
    ASN1_SUFFIX_BYTES);

  const unsigned char* rawKeyBytes = formattedBytes.data();
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
  if (key.size() == PRIVATE_KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size() || key.size() == PRIVATE_KEY_SIZE * 2)
  {
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
  else
  {
    throw BadKeyException(
      "ECDSAsecp256k1PrivateKey cannot be realized from input string: input string size should be " +
      std::to_string(PRIVATE_KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size()) + " or " +
      std::to_string(PRIVATE_KEY_SIZE * 2));
  }
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromStringDer(std::string_view key)
{
  if (key.size() != PRIVATE_KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size())
  {
    throw BadKeyException("ECDSAsecp256k1PrivateKey cannot be realized from input string: DER encoded "
                          "ECDSAsecp256k1PrivateKey hex string size should be " +
                          std::to_string(PRIVATE_KEY_SIZE * 2 + DER_ENCODED_PREFIX_HEX.size()));
  }

  return fromString(key);
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromStringRaw(std::string_view key)
{
  if (key.size() != PRIVATE_KEY_SIZE * 2)
  {
    throw BadKeyException("ECDSAsecp256k1PrivateKey cannot be realized from input string: raw ECDSAsecp256k1PrivateKey "
                          "string size should be " +
                          std::to_string(PRIVATE_KEY_SIZE * 2));
  }

  return fromString(key);
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromBytes(const std::vector<unsigned char>& bytes)
{
  if (bytes.size() == PRIVATE_KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size() || bytes.size() == PRIVATE_KEY_SIZE)
  {
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
  else
  {
    throw BadKeyException(
      "ECDSAsecp256k1PrivateKey cannot be realized from input bytes: input byte array size should be " +
      std::to_string(PRIVATE_KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size()) + " or " + std::to_string(PRIVATE_KEY_SIZE));
  }
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromBytesDer(
  const std::vector<unsigned char>& bytes)
{
  if (bytes.size() != PRIVATE_KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size())
  {
    throw BadKeyException("ECDSAsecp256k1PrivateKey cannot be realized from input bytes: DER encoded "
                          "ECDSAsecp256k1PrivateKey byte array should contain " +
                          std::to_string(PRIVATE_KEY_SIZE + DER_ENCODED_PREFIX_BYTES.size()) + " bytes");
  }

  return fromBytes(bytes);
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromBytesRaw(
  const std::vector<unsigned char>& bytes)
{
  if (bytes.size() != PRIVATE_KEY_SIZE)
  {
    throw BadKeyException("ECDSAsecp256k1PrivateKey cannot be realized from input bytes: raw ECDSAsecp256k1PrivateKey "
                          "byte array should contain " +
                          std::to_string(PRIVATE_KEY_SIZE) + " bytes");
  }

  return fromBytes(bytes);
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromSeed(const std::vector<unsigned char>& seed)
{
  try
  {
    const std::vector<unsigned char> hmacOutput = internal::OpenSSLUtils::computeSHA512HMAC(BIP32_SEED, seed);

    // The hmac is the key bytes followed by the chain code bytes
    return std::make_unique<ECDSAsecp256k1PrivateKey>(ECDSAsecp256k1PrivateKey(
      bytesToPKEY({ hmacOutput.cbegin(), hmacOutput.cbegin() + PRIVATE_KEY_SIZE }),
      { hmacOutput.cbegin() + PRIVATE_KEY_SIZE, hmacOutput.cbegin() + PRIVATE_KEY_SIZE + CHAIN_CODE_SIZE }));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(openSSLException.what());
  }
}

//-----
std::unique_ptr<PrivateKey> ECDSAsecp256k1PrivateKey::clone() const
{
  return std::make_unique<ECDSAsecp256k1PrivateKey>(*this);
}

//-----
std::unique_ptr<PrivateKey> ECDSAsecp256k1PrivateKey::derive(uint32_t childIndex) const
{
  if (getChainCode().empty())
  {
    throw UninitializedException("Key not initialized with chain code, unable to derive keys");
  }

  if (getChainCode().size() != CHAIN_CODE_SIZE)
  {
    throw BadKeyException("Key chain code malformed");
  }

  const std::vector<unsigned char> hmacOutput = internal::OpenSSLUtils::computeSHA512HMAC(
    getChainCode(),
    (internal::DerivationPathUtils::isHardenedChildIndex(childIndex))
      ? internal::Utilities::concatenateVectors(
          { 0x0 }, toBytesRaw(), internal::DerivationPathUtils::indexToBigEndianArray(childIndex))
      : internal::Utilities::concatenateVectors(
          ECDSAsecp256k1PublicKey::compressBytes(internal::Utilities::removePrefix(
            getPublicKeyBytes(),
            static_cast<long>(ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size()))),
          internal::DerivationPathUtils::indexToBigEndianArray(childIndex)));

  // Modular add the private key bytes computed from the HMAC to the existing private key (using the secp256k1 curve
  // order as the modulo), and compute the new chain code from the HMAC
  return std::make_unique<ECDSAsecp256k1PrivateKey>(ECDSAsecp256k1PrivateKey(
    bytesToPKEY(
      internal::OpenSSLUtils::BIGNUM::fromBytes({ hmacOutput.cbegin(), hmacOutput.cbegin() + PRIVATE_KEY_SIZE })
        .modularAdd(internal::OpenSSLUtils::BIGNUM::fromBytes(toBytesRaw()), CURVE_ORDER)
        .toBytes()),
    { hmacOutput.cbegin() + PRIVATE_KEY_SIZE, hmacOutput.cbegin() + PRIVATE_KEY_SIZE + CHAIN_CODE_SIZE }));
}

//-----
std::vector<unsigned char> ECDSAsecp256k1PrivateKey::sign(const std::vector<unsigned char>& bytesToSign) const
{
  const internal::OpenSSLUtils::OSSL_LIB_CTX libraryContext(OSSL_LIB_CTX_new());
  if (!libraryContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OSSL_LIB_CTX_new"));
  }

  const internal::OpenSSLUtils::EVP_MD messageDigest(EVP_MD_fetch(libraryContext.get(), "KECCAK-256", nullptr));
  if (!messageDigest)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_fetch"));
  }

  const internal::OpenSSLUtils::EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_CTX_new"));
  }

  if (EVP_DigestSignInit(messageDigestContext.get(), nullptr, messageDigest.get(), nullptr, getKeypair().get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestSignInit"));
  }

  // 72 is the maximum required size. actual signature may be slightly smaller
  size_t signatureLength = DER_ENCODED_SIGNATURE_SIZE;
  std::vector<unsigned char> signature(signatureLength);

  if (EVP_DigestSign(messageDigestContext.get(),
                     &signature.front(),
                     &signatureLength,
                     (!bytesToSign.empty()) ? &bytesToSign.front() : nullptr,
                     bytesToSign.size()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestSign"));
  }

  // we have the signature complete, now we need to turn it into its raw form of (r,s)

  const unsigned char* signaturePointer = &signature.front();
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
  std::vector<unsigned char> outputArray(RAW_SIGNATURE_SIZE);

  if (BN_bn2binpad(signatureR, &outputArray.front(), R_SIZE) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_bn2binpad"));
  }

  if (BN_bn2binpad(signatureS, &outputArray.front() + R_SIZE, S_SIZE) <= 0)
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
std::vector<unsigned char> ECDSAsecp256k1PrivateKey::toBytesDer() const
{
  return internal::Utilities::concatenateVectors(DER_ENCODED_PREFIX_BYTES, toBytesRaw());
}

//-----
std::vector<unsigned char> ECDSAsecp256k1PrivateKey::toBytesRaw() const
{
  int bytesLength = i2d_PrivateKey(getKeypair().get(), nullptr);

  std::vector<unsigned char> outputBytes(bytesLength);

  if (unsigned char* rawBytes = &outputBytes.front(); i2d_PrivateKey(getKeypair().get(), &rawBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PrivateKey"));
  }

  // The return value of i2d_PrivateKey can be either 48 or 118 bytes, depending on how the private key was constructed.
  // This difference doesn't change anything with the return here: the first 7 bytes of each are algorithm identifiers,
  // the next 32 are private key bytes, and the rest are for other purposes.
  return { outputBytes.cbegin() + static_cast<long>(ASN1_PREFIX_BYTES.size()),
           outputBytes.cbegin() + static_cast<long>(ASN1_PREFIX_BYTES.size()) + PRIVATE_KEY_SIZE };
}

//-----
ECDSAsecp256k1PrivateKey::ECDSAsecp256k1PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair,
                                                   std::vector<unsigned char> chainCode)
  : PrivateKey(std::move(keypair), std::move(chainCode))
{
}

} // namespace Hedera

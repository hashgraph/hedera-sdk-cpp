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
// The number of bytes in the algorithm identifier for an ECDSAsecp256k1PrivateKey.
constexpr const size_t ALGORITHM_IDENTIFIER_SIZE = 7;
// The ASN.1 prefix bytes
const std::vector<unsigned char> ASN1_PREFIX_BYTES = { 0x30, 0x2E, 0x02, 0x01, 0x01, 0x04, 0x20 };
// The ASN.1 suffix bytes
const std::vector<unsigned char> ASN1_SUFFIX_BYTES = { 0xA0, 0x07, 0x06, 0x05, 0x2B, 0x81, 0x04, 0x00, 0x0A };
// The order of the secp256k1 curve
const internal::OpenSSLUtils::BIGNUM CURVE_ORDER =
  internal::OpenSSLUtils::BIGNUM::fromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
}

//-----
ECDSAsecp256k1PrivateKey::ECDSAsecp256k1PrivateKey(const ECDSAsecp256k1PrivateKey& other)
  : PrivateKey()
  , mKeypair(bytesToPKEY(other.toBytes()))
  , mPublicKey(other.mPublicKey)
  , mChainCode(other.mChainCode)
{
}

//-----
ECDSAsecp256k1PrivateKey& ECDSAsecp256k1PrivateKey::operator=(const ECDSAsecp256k1PrivateKey& other)
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
ECDSAsecp256k1PrivateKey::ECDSAsecp256k1PrivateKey(ECDSAsecp256k1PrivateKey&& other) noexcept
  : PrivateKey()
  , mKeypair(std::move(other.mKeypair))
  , mPublicKey(std::move(other.mPublicKey))
  , mChainCode(std::move(other.mChainCode))
{
}

//-----
ECDSAsecp256k1PrivateKey& ECDSAsecp256k1PrivateKey::operator=(ECDSAsecp256k1PrivateKey&& other) noexcept
{
  mKeypair = std::move(other.mKeypair);
  mPublicKey = std::move(other.mPublicKey);
  mChainCode = std::move(other.mChainCode);
  return *this;
}

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
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromString(std::string_view keyString)
{
  try
  {
    return std::make_unique<ECDSAsecp256k1PrivateKey>(
      ECDSAsecp256k1PrivateKey(bytesToPKEY(internal::HexConverter::hexToBytes(keyString))));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(openSSLException.what());
  }
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromSeed(const std::vector<unsigned char>& seed)
{
  try
  {
    // "Bitcoin seed" is the seed to use to compute the SHA512 HMAC, as defined in BIP32.
    const std::vector<unsigned char> hmacOutput =
      internal::OpenSSLUtils::computeSHA512HMAC({ 'B', 'i', 't', 'c', 'o', 'i', 'n', ' ', 's', 'e', 'e', 'd' }, seed);

    // The hmac is the key bytes followed by the chain code bytes
    return std::make_unique<ECDSAsecp256k1PrivateKey>(ECDSAsecp256k1PrivateKey(
      bytesToPKEY({ hmacOutput.begin(), hmacOutput.begin() + PRIVATE_KEY_SIZE }),
      { hmacOutput.begin() + PRIVATE_KEY_SIZE, hmacOutput.begin() + PRIVATE_KEY_SIZE + CHAIN_CODE_SIZE }));
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
std::shared_ptr<PublicKey> ECDSAsecp256k1PrivateKey::getPublicKey() const
{
  return mPublicKey;
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

  if (EVP_DigestSignInit(messageDigestContext.get(), nullptr, messageDigest.get(), nullptr, mKeypair.get()) <= 0)
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
std::string ECDSAsecp256k1PrivateKey::toString() const
{
  return internal::HexConverter::bytesToHex(toBytes());
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::derive(uint32_t childIndex) const
{
  if (mChainCode.empty())
  {
    throw UninitializedException("Key not initialized with chain code, unable to derive keys");
  }

  if (mChainCode.size() != CHAIN_CODE_SIZE)
  {
    throw BadKeyException("Key chain code malformed");
  }

  // converts unsigned 32 bit int index into big endian byte array
  const std::vector<unsigned char> indexBytes = internal::DerivationPathUtils::indexToBigEndianArray(childIndex);

  std::vector<unsigned char> data;
  data.reserve(37); // 37 bytes in byte data

  if (internal::DerivationPathUtils::isHardenedChildIndex(childIndex))
  {
    // as per BIP32, private key must be padded to 33 bytes
    data.push_back(0x0);

    // hardened derivation takes private key into HMAC
    const std::vector<unsigned char> keyBytes = toBytes();
    data.insert(data.end(), keyBytes.cbegin(), keyBytes.cend());
    data.insert(data.end(), indexBytes.cbegin(), indexBytes.cend());
  }
  else
  {
    const std::vector<unsigned char> publicKeyBytes = getPublicKeyBytes();
    data.insert(data.end(), publicKeyBytes.cbegin(), publicKeyBytes.cend());
    data.insert(data.end(), indexBytes.cbegin(), indexBytes.cend());
  }

  const std::vector<unsigned char> hmacOutput = internal::OpenSSLUtils::computeSHA512HMAC(mChainCode, data);

  // Modular add the private key bytes computed from the HMAC to the existing private key (using the secp256k1 curve
  // order as the modulo), and compute the new chain code from the HMAC
  return std::make_unique<ECDSAsecp256k1PrivateKey>(ECDSAsecp256k1PrivateKey(
    bytesToPKEY(internal::OpenSSLUtils::BIGNUM::fromBytes({ hmacOutput.begin(), hmacOutput.begin() + PRIVATE_KEY_SIZE })
                  .modularAdd(internal::OpenSSLUtils::BIGNUM::fromBytes(toBytes()), CURVE_ORDER)
                  .toBytes()),
    { hmacOutput.begin() + PRIVATE_KEY_SIZE, hmacOutput.begin() + PRIVATE_KEY_SIZE + CHAIN_CODE_SIZE }));
}

//-----
std::vector<unsigned char> ECDSAsecp256k1PrivateKey::toBytes() const
{
  int bytesLength = i2d_PrivateKey(mKeypair.get(), nullptr);

  std::vector<unsigned char> outputBytes(bytesLength);

  if (unsigned char* rawBytes = &outputBytes.front(); i2d_PrivateKey(mKeypair.get(), &rawBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PrivateKey"));
  }

  // only return the 32 private key bytes
  // the return value of i2d_PrivateKey can be either 48 or 118 bytes, depending on how the private key was constructed
  // this difference doesn't change anything with the return here: the first 7 bytes of each are algorithm identifiers,
  // the next 32 are private key bytes, and the rest are for other purposes
  return { outputBytes.begin() + ALGORITHM_IDENTIFIER_SIZE,
           outputBytes.begin() + ALGORITHM_IDENTIFIER_SIZE + PRIVATE_KEY_SIZE };
}

//-----
std::vector<unsigned char> ECDSAsecp256k1PrivateKey::getChainCode() const
{
  return mChainCode;
}

//-----
internal::OpenSSLUtils::EVP_PKEY ECDSAsecp256k1PrivateKey::bytesToPKEY(std::vector<unsigned char> keyBytes)
{
  // If there are only 32 key bytes, we need to add the algorithm identifier bytes, so that we can correctly decode
  if (keyBytes.size() == PRIVATE_KEY_SIZE)
  {
    keyBytes.insert(keyBytes.begin(), ASN1_PREFIX_BYTES.cbegin(), ASN1_PREFIX_BYTES.cend());
    keyBytes.insert(keyBytes.end(), ASN1_SUFFIX_BYTES.cbegin(), ASN1_SUFFIX_BYTES.cend());
  }

  const unsigned char* rawKeyBytes = &keyBytes.front();
  internal::OpenSSLUtils::EVP_PKEY key(
    d2i_PrivateKey(EVP_PKEY_EC, nullptr, &rawKeyBytes, static_cast<long>(keyBytes.size())));
  if (!key)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("d2i_PrivateKey"));
  }

  return key;
}

//-----
ECDSAsecp256k1PrivateKey::ECDSAsecp256k1PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair)
  : PrivateKey()
  , mKeypair(std::move(keypair))
  , mPublicKey(ECDSAsecp256k1PublicKey::fromBytes(getPublicKeyBytes()))
{
}

//-----
ECDSAsecp256k1PrivateKey::ECDSAsecp256k1PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair,
                                                   std::vector<unsigned char>&& chainCode)
  : mKeypair(std::move(keypair))
  , mPublicKey(ECDSAsecp256k1PublicKey::fromBytes(getPublicKeyBytes()))
  , mChainCode(std::move(chainCode))
{
}

//-----
std::vector<unsigned char> ECDSAsecp256k1PrivateKey::getPublicKeyBytes() const
{
  int bytesLength = i2d_PUBKEY(mKeypair.get(), nullptr);

  std::vector<unsigned char> keyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &keyBytes.front(); i2d_PUBKEY(mKeypair.get(), &rawPublicKeyBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PUBKEY"));
  }

  // first 23 bytes are the ASN.1 prefix, and the remaining 65 bytes are the uncompressed pubkey
  return ECDSAsecp256k1PublicKey::compressBytes(
    { keyBytes.cbegin() + static_cast<long>(ECDSAsecp256k1PublicKey::UNCOMPRESSED_KEY_ASN1_PREFIX_BYTES.size()),
      keyBytes.cend() });
}

} // namespace Hedera

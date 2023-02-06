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
#include "MnemonicBIP39.h"
#include "impl/DerivationPathUtils.h"
#include "impl/HexConverter.h"
#include "impl/OpenSSLHasher.h"
#include "impl/OpenSSLObjectWrapper.h"

#include <openssl/ec.h>
#include <openssl/x509.h>

namespace Hedera
{
namespace
{
const inline size_t PRIVATE_KEY_SIZE = 32;
const inline size_t CHAIN_CODE_SIZE = 32;
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
  internal::OpenSSL_EVP_PKEY key(EVP_EC_gen("secp256k1"));
  if (!key)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_EC_gen"));
  }

  return std::make_unique<ECDSAsecp256k1PrivateKey>(ECDSAsecp256k1PrivateKey(std::move(key)));
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromString(const std::string& keyString)
{
  return std::make_unique<ECDSAsecp256k1PrivateKey>(
    ECDSAsecp256k1PrivateKey(bytesToPKEY(internal::HexConverter::hexToBase64(keyString))));
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromBIP39Mnemonic(const MnemonicBIP39& mnemonic,
                                                                                      const std::string& passphrase)
{
  return fromSeed(mnemonic.toSeed(passphrase));
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::fromSeed(const std::vector<unsigned char>& seed)
{
  static const std::string keyString = "Bitcoin seed"; // as defined by BIP 32

  const std::vector<unsigned char> hmacOutput =
    internal::OpenSSLHasher::computeSHA512HMAC({ keyString.begin(), keyString.end() }, seed);

  // the first 32 bytes of the hmac are the new key material
  std::vector<unsigned char> key(hmacOutput.begin(), hmacOutput.begin() + PRIVATE_KEY_SIZE);

  // chain code is the next 32 bytes of the computed hmac
  std::vector<unsigned char> chainCode(hmacOutput.begin() + CHAIN_CODE_SIZE, hmacOutput.end());

  return std::make_unique<ECDSAsecp256k1PrivateKey>(ECDSAsecp256k1PrivateKey(bytesToPKEY(key), chainCode));
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
  const internal::OpenSSL_OSSL_LIB_CTX libraryContext(OSSL_LIB_CTX_new());
  if (!libraryContext)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("OSSL_LIB_CTX_new"));
  }

  const internal::OpenSSL_EVP_MD messageDigest(EVP_MD_fetch(libraryContext.get(), "KECCAK-256", nullptr));
  if (!messageDigest)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_MD_fetch"));
  }

  const internal::OpenSSL_EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_MD_CTX_new"));
  }

  if (EVP_DigestSignInit(messageDigestContext.get(), nullptr, messageDigest.get(), nullptr, mKeypair.get()) <= 0)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_DigestSignInit"));
  }

  // 72 is the maximum required size. actual signature may be slightly smaller
  size_t signatureLength = 72;
  auto signature = std::vector<unsigned char>(signatureLength);

  if (EVP_DigestSign(messageDigestContext.get(),
                     &signature.front(),
                     &signatureLength,
                     (!bytesToSign.empty()) ? &bytesToSign.front() : nullptr,
                     bytesToSign.size()) <= 0)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_DigestSign"));
  }

  // we have the signature complete, now we need to turn it into its raw form of (r,s)

  const unsigned char* signaturePointer = &signature.front();
  const internal::OpenSSL_ECDSA_SIG signatureObject(
    d2i_ECDSA_SIG(nullptr, &signaturePointer, static_cast<long>(signatureLength)));
  if (!signatureObject)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("d2i_ECDSA_SIG"));
  }

  const BIGNUM* signatureR = ECDSA_SIG_get0_r(signatureObject.get());
  if (!signatureR)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("ECDSA_SIG_get0_r"));
  }

  const BIGNUM* signatureS = ECDSA_SIG_get0_s(signatureObject.get());
  if (!signatureS)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("ECDSA_SIG_get0_s"));
  }

  // signature is returned in the raw, 64 byte form (r, s)
  std::vector<unsigned char> outputArray(64);

  if (BN_bn2binpad(signatureR, &outputArray.front(), 32) <= 0)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_bn2binpad"));
  }

  if (BN_bn2binpad(signatureS, &outputArray.front() + 32, 32) <= 0)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_bn2binpad"));
  }

  return outputArray;
}

//-----
std::string ECDSAsecp256k1PrivateKey::toString() const
{
  return internal::HexConverter::base64ToHex(toBytes());
}

//-----
std::unique_ptr<ECDSAsecp256k1PrivateKey> ECDSAsecp256k1PrivateKey::derive(const uint32_t childIndex) const
{
  if (mChainCode.size() != CHAIN_CODE_SIZE)
  {
    throw std::runtime_error("Key doesn't support derivation");
  }

  // converts unsigned 32 bit int index into big endian byte array
  std::vector<unsigned char> indexBytes = internal::DerivationPathUtils::indexToBigEndianArray(childIndex);

  std::vector<unsigned char> data;

  if (internal::DerivationPathUtils::isHardenedChildIndex(childIndex))
  {
    // as per BIP32, private key must be padded to 33 bytes
    data.push_back(0x0);

    // hardened derivation takes private key into HMAC
    const std::vector<unsigned char> keyBytes = toBytes();
    data.insert(data.end(), keyBytes.begin(), keyBytes.end());

    data.insert(data.end(), indexBytes.begin(), indexBytes.end());
  }
  else
  {
    const std::vector<unsigned char> publicKeyBytes = getPublicKeyBytes();
    data.insert(data.end(), publicKeyBytes.begin(), publicKeyBytes.end());
    data.insert(data.end(), indexBytes.begin(), indexBytes.end());
  }

  const std::vector<unsigned char> hmacOutput = internal::OpenSSLHasher::computeSHA512HMAC(mChainCode, data);

  // the first 32 bytes of the hmac are used to compute the key
  const internal::OpenSSL_BIGNUM keyTerm1 =
    internal::OpenSSL_BIGNUM::fromBytes({ hmacOutput.begin(), hmacOutput.begin() + PRIVATE_KEY_SIZE });
  // first 32 bytes of hmac are added to existing private key
  const internal::OpenSSL_BIGNUM keyTerm2 = internal::OpenSSL_BIGNUM::fromBytes(toBytes());

  // this is the order of the curve
  static const internal::OpenSSL_BIGNUM modulo =
    internal::OpenSSL_BIGNUM::fromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");

  // chain code is the last 32 bytes of the computed hmac
  std::vector<unsigned char> chainCode(hmacOutput.begin() + CHAIN_CODE_SIZE, hmacOutput.end());

  return std::make_unique<ECDSAsecp256k1PrivateKey>(
    ECDSAsecp256k1PrivateKey(bytesToPKEY(keyTerm1.modularAdd(keyTerm2, modulo).toBytes()), chainCode));
}

//-----
std::vector<unsigned char> ECDSAsecp256k1PrivateKey::toBytes() const
{
  int bytesLength = i2d_PrivateKey(mKeypair.get(), nullptr);

  std::vector<unsigned char> outputBytes(bytesLength);

  if (unsigned char* rawBytes = &outputBytes.front(); i2d_PrivateKey(mKeypair.get(), &rawBytes) <= 0)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("i2d_PrivateKey"));
  }

  // only return the 32 private key bytes
  // the return value of i2d_PrivateKey can be either 48 or 118 bytes, depending on how the private key was constructed
  // this difference doesn't change anything with the return here: the first 7 bytes of each are algorithm identifiers,
  // the next 32 are private key bytes, and the rest are for other purposes
  return { outputBytes.begin() + 7, outputBytes.begin() + 7 + PRIVATE_KEY_SIZE };
}

//-----
std::vector<unsigned char> ECDSAsecp256k1PrivateKey::getChainCode() const
{
  return mChainCode;
}

//-----
internal::OpenSSL_EVP_PKEY ECDSAsecp256k1PrivateKey::bytesToPKEY(const std::vector<unsigned char>& keyBytes)
{
  std::vector<unsigned char> fullKeyBytes;
  // If there are only 32 key bytes, we need to add the algorithm identifier bytes, so that we can correctly decode
  if (keyBytes.size() == PRIVATE_KEY_SIZE)
  {
    // start key with ASN.1 prefix
    fullKeyBytes = internal::HexConverter::hexToBase64("302e0201010420");

    // add on actual key bytes
    fullKeyBytes.insert(fullKeyBytes.end(), keyBytes.begin(), keyBytes.end());

    // end key with ASN.1 suffix
    const std::vector<unsigned char> suffix = internal::HexConverter::hexToBase64("a00706052b8104000a");
    fullKeyBytes.insert(fullKeyBytes.end(), suffix.begin(), suffix.end());
  }
  else
  {
    fullKeyBytes = keyBytes;
  }

  const unsigned char* rawKeyBytes = &fullKeyBytes.front();
  internal::OpenSSL_EVP_PKEY key(
    d2i_PrivateKey(EVP_PKEY_EC, nullptr, &rawKeyBytes, static_cast<long>(fullKeyBytes.size())));
  if (!key)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("d2i_PrivateKey"));
  }

  return key;
}

//-----
ECDSAsecp256k1PrivateKey::ECDSAsecp256k1PrivateKey(internal::OpenSSL_EVP_PKEY&& keypair)
  : PrivateKey()
  , mKeypair(std::move(keypair))
  , mPublicKey(ECDSAsecp256k1PublicKey::fromBytes(getPublicKeyBytes()))
{
}

//-----
ECDSAsecp256k1PrivateKey::ECDSAsecp256k1PrivateKey(internal::OpenSSL_EVP_PKEY&& keypair,
                                                   std::vector<unsigned char> chainCode)
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
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("i2d_PUBKEY"));
  }

  // first 23 characters are the ASN.1 prefix, and the remaining 65 bytes are the uncompressed pubkey
  return ECDSAsecp256k1PublicKey::compressBytes({ keyBytes.begin() + 23, keyBytes.end() });
}

} // namespace Hedera

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
#include "ECDSAPrivateKey.h"
#include "impl/BigNumber.h"
#include "impl/DerivationPathUtils.h"
#include "impl/HexConverter.h"
#include "impl/OpenSSLHasher.h"

#include <openssl/err.h>
#include <openssl/x509.h>

namespace Hedera
{
namespace
{
const inline size_t PRIVATE_KEY_SIZE = 32;
const inline size_t CHAIN_CODE_SIZE = 32;
}

//-----
ECDSAPrivateKey::~ECDSAPrivateKey()
{
  EVP_PKEY_free(mKeypair);
}

//-----
ECDSAPrivateKey::ECDSAPrivateKey(const ECDSAPrivateKey& other)
  : PrivateKey()
  , mKeypair(bytesToPKEY(other.toBytes()))
  , mPublicKey(other.mPublicKey)
  , mChainCode(other.mChainCode)
{
}

//-----
ECDSAPrivateKey& ECDSAPrivateKey::operator=(const ECDSAPrivateKey& other)
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
ECDSAPrivateKey::ECDSAPrivateKey(ECDSAPrivateKey&& other) noexcept
  : PrivateKey()
  , mKeypair(other.mKeypair)
  , mPublicKey(std::move(other.mPublicKey))
  , mChainCode(std::move(other.mChainCode))
{
  other.mKeypair = nullptr;
}

//-----
ECDSAPrivateKey& ECDSAPrivateKey::operator=(ECDSAPrivateKey&& other) noexcept
{
  mKeypair = other.mKeypair;
  other.mKeypair = nullptr;

  mPublicKey = std::move(other.mPublicKey);
  mChainCode = std::move(other.mChainCode);
  return *this;
}

//-----
std::unique_ptr<ECDSAPrivateKey> ECDSAPrivateKey::generatePrivateKey()
{
  EVP_PKEY* keypair = EVP_EC_gen("secp256k1");

  if (keypair == nullptr)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_EC_gen"));
  }

  return std::make_unique<ECDSAPrivateKey>(ECDSAPrivateKey(keypair));
}

//-----
std::unique_ptr<ECDSAPrivateKey> ECDSAPrivateKey::fromString(const std::string& keyString)
{
  return std::make_unique<ECDSAPrivateKey>(
    ECDSAPrivateKey(bytesToPKEY(internal::HexConverter::hexToBase64(keyString))));
}

//-----
std::unique_ptr<ECDSAPrivateKey> ECDSAPrivateKey::fromBIP39Mnemonic(const MnemonicBIP39& mnemonic,
                                                                    const std::string& passphrase)
{
  return fromSeed(mnemonic.toSeed(passphrase));
}

//-----
std::unique_ptr<ECDSAPrivateKey> ECDSAPrivateKey::fromSeed(const std::vector<unsigned char>& seed)
{
  static const std::string keyString = "Bitcoin seed"; // as defined by BIP 32

  const std::vector<unsigned char> hmacOutput =
    internal::OpenSSLHasher::computeSHA512HMAC({ keyString.begin(), keyString.end() }, seed);

  // the first 32 bytes of the hmac are the new key material
  std::vector<unsigned char> key(hmacOutput.begin(), hmacOutput.begin() + PRIVATE_KEY_SIZE);

  // chain code is the next 32 bytes of the computed hmac
  std::vector<unsigned char> chainCode(hmacOutput.begin() + CHAIN_CODE_SIZE, hmacOutput.end());

  return std::make_unique<ECDSAPrivateKey>(ECDSAPrivateKey(bytesToPKEY(key), chainCode));
}

//-----
std::unique_ptr<PrivateKey> ECDSAPrivateKey::clone() const
{
  return std::make_unique<ECDSAPrivateKey>(*this);
}

//-----
std::shared_ptr<PublicKey> ECDSAPrivateKey::getPublicKey() const
{
  return mPublicKey;
}

//-----
std::vector<unsigned char> ECDSAPrivateKey::sign(const std::vector<unsigned char>& bytesToSign) const
{
  EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_new();
  if (!messageDigestContext)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_MD_CTX_new"));
  }

  OSSL_LIB_CTX* libraryContext = OSSL_LIB_CTX_new();
  if (!libraryContext)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("OSSL_LIB_CTX_new"));
  }

  EVP_MD* messageDigest = EVP_MD_fetch(libraryContext, "KECCAK-256", nullptr);
  if (!messageDigest)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_MD_fetch"));
  }

  OSSL_LIB_CTX_free(libraryContext);

  int result = EVP_DigestSignInit(messageDigestContext, nullptr, messageDigest, nullptr, mKeypair);
  EVP_MD_free(messageDigest);

  if (result <= 0)
  {
    EVP_MD_CTX_free(messageDigestContext);
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_DigestSignInit"));
  }

  // 72 is the maximum required size. actual signature may be slightly smaller
  size_t signatureLength = 72;
  auto signature = std::vector<unsigned char>(signatureLength);

  if (EVP_DigestSign(
        messageDigestContext, &signature.front(), &signatureLength, &bytesToSign.front(), bytesToSign.size()) <= 0)
  {
    EVP_MD_CTX_free(messageDigestContext);
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_DigestSign"));
  }

  EVP_MD_CTX_free(messageDigestContext);

  // we have the signature complete, now we need to turn it into its raw form of (r,s)

  const unsigned char* signaturePointer = &signature.front();
  ECDSA_SIG* signatureObject = d2i_ECDSA_SIG(nullptr, &signaturePointer, (long)signatureLength);
  if (signatureObject == nullptr)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("d2i_ECDSA_SIG"));
  }

  const BIGNUM* signatureR = ECDSA_SIG_get0_r(signatureObject);
  if (signatureR == nullptr)
  {
    ECDSA_SIG_free(signatureObject);
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("ECDSA_SIG_get0_r"));
  }

  const BIGNUM* signatureS = ECDSA_SIG_get0_s(signatureObject);
  if (signatureS == nullptr)
  {
    ECDSA_SIG_free(signatureObject);
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("ECDSA_SIG_get0_s"));
  }

  // signature is returned in the raw, 64 byte form (r, s)
  std::vector<unsigned char> outputArray(64);

  if (BN_bn2binpad(signatureR, &outputArray.front(), 32) <= 0)
  {
    ECDSA_SIG_free(signatureObject);
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_bn2binpad"));
  }

  if (BN_bn2binpad(signatureS, &outputArray.front() + 32, 32) <= 0)
  {
    ECDSA_SIG_free(signatureObject);
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_bn2binpad"));
  }

  return outputArray;
}

//-----
std::string ECDSAPrivateKey::toString() const
{
  return internal::HexConverter::base64ToHex(toBytes());
}

//-----
std::unique_ptr<ECDSAPrivateKey> ECDSAPrivateKey::derive(const uint32_t childIndex) const
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
  internal::BigNumber keyTerm1 =
    internal::BigNumber::fromBytes({ hmacOutput.begin(), hmacOutput.begin() + PRIVATE_KEY_SIZE });
  // first 32 bytes of hmac are added to existing private key
  internal::BigNumber keyTerm2 = internal::BigNumber::fromBytes(toBytes());

  // this is the order of the curve
  static const internal::BigNumber modulo =
    internal::BigNumber::fromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");

  internal::BigNumber moduloSum = keyTerm1.modularAdd(keyTerm2, modulo);

  // chain code is the last 32 bytes of the computed hmac
  std::vector<unsigned char> chainCode(hmacOutput.begin() + CHAIN_CODE_SIZE, hmacOutput.end());

  return std::make_unique<ECDSAPrivateKey>(ECDSAPrivateKey(bytesToPKEY(moduloSum.toBytes()), chainCode));
}

//-----
std::vector<unsigned char> ECDSAPrivateKey::toBytes() const
{
  int bytesLength = i2d_PrivateKey(mKeypair, nullptr);

  std::vector<unsigned char> outputBytes(bytesLength);

  if (unsigned char* rawBytes = &outputBytes.front(); i2d_PrivateKey(mKeypair, &rawBytes) <= 0)
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
std::vector<unsigned char> ECDSAPrivateKey::getChainCode() const
{
  return mChainCode;
}

//-----
EVP_PKEY* ECDSAPrivateKey::bytesToPKEY(const std::vector<unsigned char>& keyBytes)
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

  EVP_PKEY* key = d2i_PrivateKey(EVP_PKEY_EC, nullptr, &rawKeyBytes, (long)fullKeyBytes.size());

  if (key == nullptr)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("d2i_PrivateKey"));
  }

  return key;
}

//-----
ECDSAPrivateKey::ECDSAPrivateKey(EVP_PKEY* keypair)
  : PrivateKey()
  , mKeypair(keypair)
  , mPublicKey(ECDSAPublicKey::fromBytes(getPublicKeyBytes()))
{
}

//-----
ECDSAPrivateKey::ECDSAPrivateKey(EVP_PKEY* keypair, std::vector<unsigned char> chainCode)
  : mKeypair(keypair)
  , mPublicKey(ECDSAPublicKey::fromBytes(getPublicKeyBytes()))
  , mChainCode(std::move(chainCode))
{
}

//-----
std::vector<unsigned char> ECDSAPrivateKey::getPublicKeyBytes() const
{
  int bytesLength = i2d_PUBKEY(mKeypair, nullptr);

  std::vector<unsigned char> keyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &keyBytes.front(); i2d_PUBKEY(mKeypair, &rawPublicKeyBytes) <= 0)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("i2d_PUBKEY"));
  }

  // first 23 characters are the ASN.1 prefix, and the remaining 65 bytes are the uncompressed pubkey
  return ECDSAPublicKey::compressBytes({ keyBytes.begin() + 23, keyBytes.end() });
}

} // namespace Hedera

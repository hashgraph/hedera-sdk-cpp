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
#include "ECDSAPublicKey.h"
#include "impl/HexConverter.h"
#include "impl/OpenSSLHasher.h"

#include <openssl/x509.h>
#include <proto/basic_types.pb.h>

#include <openssl/decoder.h>
#include <openssl/ec.h>

namespace Hedera
{
namespace
{
// size of an uncompressed public key without ASN1 prefix
const inline size_t UNCOMPRESSED_KEY_SIZE = 65;
// size of a compressed public key without ASN1 prefix
const inline size_t COMPRESSED_KEY_SIZE = 33;
const inline std::string UNCOMPRESSED_KEY_ASN1_PREFIX = "3056301006072A8648CE3D020106052B8104000A034200";
}

//-----
ECDSAPublicKey::~ECDSAPublicKey()
{
  EVP_PKEY_free(mPublicKey);
}

//-----
ECDSAPublicKey::ECDSAPublicKey(const ECDSAPublicKey& other)
  : mPublicKey(bytesToPKEY(other.toBytes()))
{
}

//-----
ECDSAPublicKey& ECDSAPublicKey::operator=(const ECDSAPublicKey& other)
{
  if (this != &other)
  {
    mPublicKey = bytesToPKEY(other.toBytes());
  }

  return *this;
}

//-----
ECDSAPublicKey::ECDSAPublicKey(ECDSAPublicKey&& other) noexcept
  : mPublicKey(other.mPublicKey)
{
  other.mPublicKey = nullptr;
}

//-----
ECDSAPublicKey& ECDSAPublicKey::operator=(ECDSAPublicKey&& other) noexcept
{
  mPublicKey = other.mPublicKey;
  other.mPublicKey = nullptr;

  return *this;
}

//-----
std::shared_ptr<ECDSAPublicKey> ECDSAPublicKey::fromString(const std::string& keyString)
{
  return fromBytes(internal::HexConverter::hexToBase64(keyString));
}

//-----
std::shared_ptr<ECDSAPublicKey> ECDSAPublicKey::fromBytes(const std::vector<unsigned char>& keyBytes)
{
  const ECDSAPublicKey ecdsaPublicKey(bytesToPKEY(keyBytes));
  return (ecdsaPublicKey.mPublicKey) ? std::make_shared<ECDSAPublicKey>(ecdsaPublicKey) : nullptr;
}

//-----
std::unique_ptr<PublicKey> ECDSAPublicKey::clone() const
{
  return std::make_unique<ECDSAPublicKey>(*this);
}

//-----
bool ECDSAPublicKey::verifySignature(const std::vector<unsigned char>& signatureBytes,
                                     const std::vector<unsigned char>& signedBytes) const
{
  // incoming signatures are in the raw form (r, s), where r and s are each 32 bytes long
  if (signatureBytes.size() != 64)
  {
    return false;
  }

  // first, convert the incoming signature to DER format, so that it can be verified

  BIGNUM* signatureR = BN_bin2bn(&signatureBytes.front(), 32, nullptr);
  if (signatureR == nullptr)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_bin2bn"));
  }

  BIGNUM* signatureS = BN_bin2bn(&signatureBytes.front() + 32, 32, nullptr);
  if (signatureS == nullptr)
  {
    BN_free(signatureR);
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_bin2bn"));
  }

  ECDSA_SIG* signatureObject = ECDSA_SIG_new();

  // this set function transfers ownership of the big numbers to the signature object
  // after this call succeeds, signatureR and signatureS should NOT be freed manually
  if (ECDSA_SIG_set0(signatureObject, signatureR, signatureS) <= 0)
  {
    BN_free(signatureR);
    BN_free(signatureS);
    ECDSA_SIG_free(signatureObject);
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_bin2bn"));
  }

  // maximum length of DER encoded signature is 72
  std::vector<unsigned char> derEncodedSignature(72);
  unsigned char* encodedSignaturePointer = &derEncodedSignature.front();

  // keep track of how long the DER encoding actually is, since we'll need to tell the verification function
  int actualSignatureLength = i2d_ECDSA_SIG(signatureObject, &encodedSignaturePointer);
  if (actualSignatureLength <= 0)
  {
    ECDSA_SIG_free(signatureObject);
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("i2d_ECDSA_SIG"));
  }

  // wash our hands of the signature object, since we've successfully gotten the DER encoding
  ECDSA_SIG_free(signatureObject);

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

  int result = EVP_DigestVerifyInit(messageDigestContext, nullptr, messageDigest, nullptr, mPublicKey);
  EVP_MD_free(messageDigest);

  if (result <= 0)
  {
    EVP_MD_CTX_free(messageDigestContext);
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_DigestVerifyInit"));
  }

  int verificationResult = EVP_DigestVerify(messageDigestContext,
                                            &derEncodedSignature.front(),
                                            actualSignatureLength,
                                            (!signedBytes.empty()) ? &signedBytes.front() : nullptr,
                                            signedBytes.size());
  EVP_MD_CTX_free(messageDigestContext);

  // any value other than 0 or 1 means an error occurred
  if (verificationResult != 0 && verificationResult != 1)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EVP_DigestVerify"));
  }

  return verificationResult == 1;
}

//----
std::unique_ptr<proto::Key> ECDSAPublicKey::toProtobuf() const
{
  auto keyProtobuf = std::make_unique<proto::Key>();

  const std::vector<unsigned char> rawBytes = toBytes();
  keyProtobuf->set_allocated_ecdsa_secp256k1(new std::string({ rawBytes.cbegin(), rawBytes.cend() }));
  return keyProtobuf;
}

//-----
std::string ECDSAPublicKey::toString() const
{
  return internal::HexConverter::base64ToHex(toBytes());
}

//-----
std::vector<unsigned char> ECDSAPublicKey::toBytes() const
{
  int bytesLength = i2d_PUBKEY(mPublicKey, nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &publicKeyBytes.front(); i2d_PUBKEY(mPublicKey, &rawPublicKeyBytes) <= 0)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("i2d_PUBKEY"));
  }

  static const size_t asn1PrefixSize = UNCOMPRESSED_KEY_ASN1_PREFIX.size() / 2; // string has 2 chars per byte

  if (publicKeyBytes.size() != UNCOMPRESSED_KEY_SIZE + asn1PrefixSize)
  {
    throw std::runtime_error("Expected public key size [" + std::to_string(UNCOMPRESSED_KEY_SIZE + asn1PrefixSize) +
                             "]. Actual size was [" + std::to_string(publicKeyBytes.size()) + "]");
  }

  // don't return the algorithm identification bytes, and compress
  return compressBytes({ publicKeyBytes.begin() + (long)asn1PrefixSize, publicKeyBytes.end() });
}

//-----
EVP_PKEY* ECDSAPublicKey::bytesToPKEY(const std::vector<unsigned char>& inputKeyBytes)
{
  // OpenSSL requires that the bytes are uncompressed to construct the pkey output object
  // start the uncompressed bytes with the appropriate ASN1 prefix for an uncompressed public key
  std::vector<unsigned char> uncompressedKeyBytes = internal::HexConverter::hexToBase64(UNCOMPRESSED_KEY_ASN1_PREFIX);

  if (const size_t inputKeySize = inputKeyBytes.size(); inputKeySize == COMPRESSED_KEY_SIZE)
  {
    // get the uncompressed key representation, and append to the existing prefix
    const std::vector<unsigned char> rawUncompressedBytes = uncompressBytes(inputKeyBytes);
    uncompressedKeyBytes.insert(uncompressedKeyBytes.end(), rawUncompressedBytes.begin(), rawUncompressedBytes.end());
  }
  else if (inputKeySize == UNCOMPRESSED_KEY_SIZE)
  {
    // input bytes are already uncompressed, so simply append to existing prefix
    uncompressedKeyBytes.insert(uncompressedKeyBytes.end(), inputKeyBytes.begin(), inputKeyBytes.end());
  }
  else
  {
    throw std::invalid_argument("bytesToPKEY input bytes size [" + std::to_string(inputKeySize) +
                                "] is invalid: must be either [" + std::to_string(UNCOMPRESSED_KEY_SIZE) + "] or [" +
                                std::to_string(COMPRESSED_KEY_SIZE) + "]");
  }

  EVP_PKEY* pkey = nullptr;

  OSSL_DECODER_CTX* context =
    OSSL_DECODER_CTX_new_for_pkey(&pkey, "DER", nullptr, "EC", EVP_PKEY_PUBLIC_KEY, nullptr, nullptr);

  if (context == nullptr)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("OSSL_DECODER_CTX_new_for_pkey"));
  }

  size_t dataLength = uncompressedKeyBytes.size();
  if (const unsigned char* rawKeyBytes = &uncompressedKeyBytes.front();
      OSSL_DECODER_from_data(context, &rawKeyBytes, &dataLength) <= 0)
  {
    OSSL_DECODER_CTX_free(context);
    EVP_PKEY_free(pkey);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("OSSL_DECODER_from_data"));
  }

  OSSL_DECODER_CTX_free(context);

  return pkey;
}

//-----
ECDSAPublicKey::ECDSAPublicKey(EVP_PKEY* publicKey)
  : mPublicKey(publicKey)
{
}

std::vector<unsigned char> ECDSAPublicKey::compressBytes(const std::vector<unsigned char>& uncompressedBytes)
{
  // a public key is an (x, y) coordinate on the elliptic curve
  // the uncompressed key comes in the form [0x04][32 bytes of x coord][32 bytes of y coord]
  if (uncompressedBytes.size() != UNCOMPRESSED_KEY_SIZE)
  {
    throw std::invalid_argument("compressBytes input bytes size [" + std::to_string(uncompressedBytes.size()) +
                                "] is invalid: must be [" + std::to_string(UNCOMPRESSED_KEY_SIZE) + "]");
  }

  if (uncompressedBytes[0] != 0x4)
  {
    throw std::invalid_argument("Uncompressed bytes should begin with 0x04");
  }

  EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);

  if (group == nullptr)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EC_GROUP_new_by_curve_name"));
  }

  EC_POINT* uncompressedPoint = EC_POINT_new(group);

  if (uncompressedPoint == nullptr)
  {
    EC_GROUP_free(group);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EC_POINT_new"));
  }

  BN_CTX* context = BN_CTX_new();

  if (context == nullptr)
  {
    EC_GROUP_free(group);
    EC_POINT_free(uncompressedPoint);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_CTX_new"));
  }

  // parse the uncompressed point into an EC_POINT object
  if (EC_POINT_oct2point(group, uncompressedPoint, &uncompressedBytes.front(), UNCOMPRESSED_KEY_SIZE, context) <= 0)
  {
    EC_GROUP_free(group);
    EC_POINT_free(uncompressedPoint);
    BN_CTX_free(context);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EC_POINT_oct2point"));
  }

  std::vector<unsigned char> compressedBytes(COMPRESSED_KEY_SIZE);

  // serialize the point to compressed form
  if (EC_POINT_point2oct(group,
                         uncompressedPoint,
                         POINT_CONVERSION_COMPRESSED,
                         &compressedBytes.front(),
                         COMPRESSED_KEY_SIZE,
                         context) <= 0)
  {
    EC_GROUP_free(group);
    EC_POINT_free(uncompressedPoint);
    BN_CTX_free(context);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EC_POINT_point2oct"));
  }

  EC_GROUP_free(group);
  EC_POINT_free(uncompressedPoint);
  BN_CTX_free(context);

  return compressedBytes;
}

std::vector<unsigned char> ECDSAPublicKey::uncompressBytes(const std::vector<unsigned char>& compressedBytes)
{
  // a public key is an (x, y) coordinate on the elliptic curve
  // the compressed key comes in the form [0x02 or 0x03][32 bytes of x coord]
  if (compressedBytes.size() != COMPRESSED_KEY_SIZE)
  {
    throw std::invalid_argument("uncompressBytes input bytes size [" + std::to_string(compressedBytes.size()) +
                                "] is invalid: must be [" + std::to_string(COMPRESSED_KEY_SIZE) + "]");
  }

  if (compressedBytes[0] != 0x2 && compressedBytes[0] != 0x3)
  {
    throw std::invalid_argument("Compressed bytes should begin with 0x02 or 0x03");
  }

  EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);

  if (group == nullptr)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EC_GROUP_new_by_curve_name"));
  }

  EC_POINT* compressedPoint = EC_POINT_new(group);

  if (compressedPoint == nullptr)
  {
    EC_GROUP_free(group);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EC_POINT_new"));
  }

  BN_CTX* context = BN_CTX_new();

  if (context == nullptr)
  {
    EC_GROUP_free(group);
    EC_POINT_free(compressedPoint);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_CTX_new"));
  }

  // parse the compressed point into an EC_POINT object
  if (EC_POINT_oct2point(group, compressedPoint, &compressedBytes.front(), COMPRESSED_KEY_SIZE, context) <= 0)
  {
    EC_GROUP_free(group);
    EC_POINT_free(compressedPoint);
    BN_CTX_free(context);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EC_POINT_oct2point"));
  }

  std::vector<unsigned char> uncompressedBytes(UNCOMPRESSED_KEY_SIZE);

  // serialize the point to uncompressed form
  if (EC_POINT_point2oct(group,
                         compressedPoint,
                         POINT_CONVERSION_UNCOMPRESSED,
                         &uncompressedBytes.front(),
                         UNCOMPRESSED_KEY_SIZE,
                         context) <= 0)
  {
    EC_GROUP_free(group);
    EC_POINT_free(compressedPoint);
    BN_CTX_free(context);

    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("EC_POINT_point2oct"));
  }

  EC_GROUP_free(group);
  EC_POINT_free(compressedPoint);
  BN_CTX_free(context);

  return uncompressedBytes;
}

} // namespace Hedera

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

#include <iostream>
#include <openssl/core_names.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <proto/basic_types.pb.h>

#include <openssl/decoder.h>
#include <openssl/ec.h>

namespace Hedera
{
namespace
{
const inline size_t UNCOMPRESSED_KEY_SIZE = 65;
const inline size_t COMPRESSED_KEY_SIZE = 33;
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
  return std::make_shared<ECDSAPublicKey>(ECDSAPublicKey(bytesToPKEY(keyBytes)));
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
  EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_new();

  if (!messageDigestContext)
  {
    throw std::runtime_error("Digest context construction failed");
  }

  if (EVP_DigestVerifyInit(messageDigestContext, nullptr, nullptr, nullptr, mPublicKey) <= 0)
  {
    EVP_MD_CTX_free(messageDigestContext);
    throw std::runtime_error("Digest verify initialization failed");
  }

  int verificationResult = EVP_DigestVerify(
    messageDigestContext, &signatureBytes.front(), signatureBytes.size(), &signedBytes.front(), signedBytes.size());

  if (verificationResult <= 0)
  {
    char* err = ERR_error_string(verificationResult, nullptr);
    std::cout << std::endl << err << std::endl << std::endl;
    std::cout << "Failed to verify signature with code [" << verificationResult << ']' << std::endl;
  }

  EVP_MD_CTX_free(messageDigestContext);

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
    throw std::runtime_error("ECDSAPublicKey serialization error");
  }

  // don't return the algorithm identification bytes
  // returned compressed representation by default
  // TODO check sizes
  return compressBytes({ publicKeyBytes.begin() + 23, publicKeyBytes.end() });
}

//-----
EVP_PKEY* ECDSAPublicKey::bytesToPKEY(const std::vector<unsigned char>& inputKeyBytes)
{
  // start the uncompressed bytes with the appropriate ASN1 prefix for an uncompressed public key
  std::vector<unsigned char> uncompressedKeyBytes =
    internal::HexConverter::hexToBase64("3056301006072A8648CE3D020106052B8104000A034200");

  const size_t inputKeySize = inputKeyBytes.size();
  if (inputKeySize == COMPRESSED_KEY_SIZE)
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

  OSSL_DECODER_CTX* context;
  EVP_PKEY* pkey = nullptr;

  context = OSSL_DECODER_CTX_new_for_pkey(&pkey, "DER", nullptr, "EC", EVP_PKEY_PUBLIC_KEY, nullptr, nullptr);
  if (context == nullptr)
  {
    OSSL_DECODER_CTX_free(context);

    throw std::runtime_error("OSSL_DECODER_CTX_new_for_pkey failed");
  }

  const unsigned char* rawKeyBytes = &uncompressedKeyBytes.front();
  size_t dataLength = uncompressedKeyBytes.size();
  if (OSSL_DECODER_from_data(context, &rawKeyBytes, &dataLength) <= 0)
  {
    OSSL_DECODER_CTX_free(context);
    EVP_PKEY_free(pkey);

    throw std::runtime_error("OSSL_DECODER_from_data failed");
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
  // recall, a public key is simply an (x, y) coordinate on the elliptic curve
  // the compressed key comes in the form [0x02 or 0x03][32 bytes of x coord]
  if (uncompressedBytes.size() != UNCOMPRESSED_KEY_SIZE)
  {
    throw std::invalid_argument("compressBytes input bytes size [" + std::to_string(uncompressedBytes.size()) +
                                "] is invalid: must be [" + std::to_string(UNCOMPRESSED_KEY_SIZE) + "]");
  }

  EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);

  if (group == nullptr)
  {
    throw std::runtime_error("EC_GROUP_new_by_curve_name failed");
  }

  EC_POINT* uncompressedPoint = EC_POINT_new(group);

  if (uncompressedPoint == nullptr)
  {
    EC_GROUP_free(group);

    throw std::runtime_error("EC_POINT_new failed");
  }

  BN_CTX* context = BN_CTX_new();

  if (context == nullptr)
  {
    EC_GROUP_free(group);
    EC_POINT_free(uncompressedPoint);

    throw std::runtime_error("BN_CTX_new failed");
  }

  // parse the uncompressed point into an EC_POINT object
  if (EC_POINT_oct2point(group, uncompressedPoint, &uncompressedBytes.front(), UNCOMPRESSED_KEY_SIZE, context) <= 0)
  {
    EC_GROUP_free(group);
    EC_POINT_free(uncompressedPoint);
    BN_CTX_free(context);

    throw std::runtime_error("EC_POINT_oct2point failed");
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

    throw std::runtime_error("EC_POINT_point2oct failed");
  }

  EC_GROUP_free(group);
  EC_POINT_free(uncompressedPoint);
  BN_CTX_free(context);

  return compressedBytes;
}

std::vector<unsigned char> ECDSAPublicKey::uncompressBytes(const std::vector<unsigned char>& compressedBytes)
{
  // recall, a public key is simply an (x, y) coordinate on the elliptic curve
  // the compressed key comes in the form [0x02 or 0x03][32 bytes of x coord]
  if (compressedBytes.size() != COMPRESSED_KEY_SIZE)
  {
    throw std::invalid_argument("uncompressBytes input bytes size [" + std::to_string(compressedBytes.size()) +
                                "] is invalid: must be [" + std::to_string(COMPRESSED_KEY_SIZE) + "]");
  }

  EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);

  if (group == nullptr)
  {
    throw std::runtime_error("EC_GROUP_new_by_curve_name failed");
  }

  EC_POINT* compressedPoint = EC_POINT_new(group);

  if (compressedPoint == nullptr)
  {
    EC_GROUP_free(group);

    throw std::runtime_error("EC_POINT_new failed");
  }

  BN_CTX* context = BN_CTX_new();

  if (context == nullptr)
  {
    EC_GROUP_free(group);
    EC_POINT_free(compressedPoint);

    throw std::runtime_error("BN_CTX_new failed");
  }

  // parse the compressed point into an EC_POINT object
  if (EC_POINT_oct2point(group, compressedPoint, &compressedBytes.front(), COMPRESSED_KEY_SIZE, context) <= 0)
  {
    EC_GROUP_free(group);
    EC_POINT_free(compressedPoint);
    BN_CTX_free(context);

    throw std::runtime_error("EC_POINT_oct2point failed");
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

    throw std::runtime_error("EC_POINT_point2oct failed");
  }

  EC_GROUP_free(group);
  EC_POINT_free(compressedPoint);
  BN_CTX_free(context);

  return uncompressedBytes;
}

} // namespace Hedera

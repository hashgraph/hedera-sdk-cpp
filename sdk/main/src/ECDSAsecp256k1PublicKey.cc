/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "ECDSAsecp256k1PublicKey.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "EvmAddress.h"
#include "exceptions/BadKeyException.h"
#include "exceptions/OpenSSLException.h"
#include "impl/HexConverter.h"
#include "impl/PublicKeyImpl.h"
#include "impl/Utilities.h"
#include "impl/openssl_utils/BIGNUM.h"
#include "impl/openssl_utils/BN_CTX.h"
#include "impl/openssl_utils/ECDSA_SIG.h"
#include "impl/openssl_utils/EC_GROUP.h"
#include "impl/openssl_utils/EC_POINT.h"
#include "impl/openssl_utils/EVP_MD.h"
#include "impl/openssl_utils/EVP_MD_CTX.h"
#include "impl/openssl_utils/OSSL_DECODER_CTX.h"
#include "impl/openssl_utils/OSSL_LIB_CTX.h"
#include "impl/openssl_utils/OpenSSLUtils.h"

#include <openssl/decoder.h>
#include <openssl/ec.h>
#include <openssl/x509.h>
#include <proto/basic_types.pb.h>

namespace Hedera
{
namespace
{
/**
 * Create a wrapped OpenSSL keypair object from a byte vector (raw or DER-encoded) representing an
 * ECDSAsecp256k1PublicKey (compressed or uncompressed).
 *
 * @param bytes The bytes representing a ECDSAsecp256k1PublicKey.
 * @return The newly created wrapped OpenSSL keypair object.
 * @throws OpenSSLException If OpenSSL is unable to create a keypair from the input bytes.
 */
[[nodiscard]] internal::OpenSSLUtils::EVP_PKEY bytesToPKEY(const std::vector<std::byte>& bytes)
{
  // OpenSSL requires that the bytes are uncompressed and that they contain the appropriate ASN.1 prefix.
  std::vector<std::byte> uncompressedKeyBytes;
  if (bytes.size() == ECDSAsecp256k1PublicKey::UNCOMPRESSED_KEY_SIZE)
  {
    // Simply add the DER-encoding header to the bytes
    uncompressedKeyBytes = internal::Utilities::concatenateVectors(
      { ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES, bytes });
  }
  else if (bytes.size() == ECDSAsecp256k1PublicKey::COMPRESSED_KEY_SIZE +
                             ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES.size() &&
           internal::Utilities::isPrefixOf(bytes, ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES))
  {
    // Remove the DER-encoding header, uncompress the key bytes, then re-add the DER-encoding header
    uncompressedKeyBytes = internal::Utilities::concatenateVectors(
      { ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES,
        ECDSAsecp256k1PublicKey::uncompressBytes(internal::Utilities::removePrefix(
          bytes, static_cast<long>(ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES.size()))) });
  }
  else if (bytes.size() == ECDSAsecp256k1PublicKey::COMPRESSED_KEY_SIZE)
  {
    // Uncompress the key bytes, then add the DER-encoding header
    uncompressedKeyBytes =
      internal::Utilities::concatenateVectors({ ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES,
                                                ECDSAsecp256k1PublicKey::uncompressBytes(bytes) });
  }
  else
  {
    // Either already DER-encoded and uncompressed or malformed, which will throw
    uncompressedKeyBytes = bytes;
  }

  EVP_PKEY* pkey = nullptr;
  internal::OpenSSLUtils::OSSL_DECODER_CTX context(
    OSSL_DECODER_CTX_new_for_pkey(&pkey, "DER", nullptr, "EC", EVP_PKEY_PUBLIC_KEY, nullptr, nullptr));
  if (!context)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OSSL_DECODER_CTX_new_for_pkey"));
  }

  size_t dataLength = uncompressedKeyBytes.size();
  if (const auto* rawKeyBytes = internal::Utilities::toTypePtr<unsigned char>(uncompressedKeyBytes.data());
      OSSL_DECODER_from_data(context.get(), &rawKeyBytes, &dataLength) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OSSL_DECODER_from_data"));
  }

  return internal::OpenSSLUtils::EVP_PKEY(pkey);
}

} // namespace

//-----
std::unique_ptr<ECDSAsecp256k1PublicKey> ECDSAsecp256k1PublicKey::fromString(std::string_view key)
{
  if ((key.size() != COMPRESSED_KEY_SIZE * 2) && (key.size() != UNCOMPRESSED_KEY_SIZE * 2) &&
      (key.size() != COMPRESSED_KEY_SIZE * 2 + DER_ENCODED_COMPRESSED_PREFIX_HEX.size()) &&
      (key.size() != UNCOMPRESSED_KEY_SIZE * 2 + DER_ENCODED_UNCOMPRESSED_PREFIX_HEX.size()))
  {
    throw BadKeyException("ECDSAsecp256k1PublicKey cannot be realized from input string: input string size should be " +
                          std::to_string(UNCOMPRESSED_KEY_SIZE * 2 + DER_ENCODED_UNCOMPRESSED_PREFIX_HEX.size()) +
                          " or " + std::to_string(UNCOMPRESSED_KEY_SIZE * 2) + " for uncompressed keys, or " +
                          std::to_string(COMPRESSED_KEY_SIZE * 2 + DER_ENCODED_COMPRESSED_PREFIX_HEX.size()) + " or " +
                          std::to_string(COMPRESSED_KEY_SIZE * 2) + " for compressed keys");
  }

  try
  {
    return std::make_unique<ECDSAsecp256k1PublicKey>(
      ECDSAsecp256k1PublicKey(bytesToPKEY(internal::HexConverter::hexToBytes(key))));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(std::string("ECDSAsecp256k1PublicKey cannot be realized from input string: ") +
                          openSSLException.what());
  }
}

//-----
std::unique_ptr<ECDSAsecp256k1PublicKey> ECDSAsecp256k1PublicKey::fromBytes(const std::vector<std::byte>& bytes)
{
  if ((bytes.size() != COMPRESSED_KEY_SIZE) && (bytes.size() != UNCOMPRESSED_KEY_SIZE) &&
      (bytes.size() != COMPRESSED_KEY_SIZE + DER_ENCODED_COMPRESSED_PREFIX_BYTES.size()) &&
      (bytes.size() != UNCOMPRESSED_KEY_SIZE + DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size()))
  {
    throw BadKeyException(
      "ECDSAsecp256k1PublicKey cannot be realized from input bytes: input byte array size should be " +
      std::to_string(UNCOMPRESSED_KEY_SIZE + DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size()) + " or " +
      std::to_string(UNCOMPRESSED_KEY_SIZE) + " for uncompressed keys, or " +
      std::to_string(COMPRESSED_KEY_SIZE + DER_ENCODED_COMPRESSED_PREFIX_BYTES.size()) + " or " +
      std::to_string(COMPRESSED_KEY_SIZE) + " for compressed keys");
  }

  try
  {
    return std::make_unique<ECDSAsecp256k1PublicKey>(ECDSAsecp256k1PublicKey(bytesToPKEY(bytes)));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(std::string("ECDSAsecp256k1PublicKey cannot be realized from input bytes: ") +
                          openSSLException.what());
  }
}

//-----
std::vector<std::byte> ECDSAsecp256k1PublicKey::compressBytes(const std::vector<std::byte>& uncompressedBytes)
{
  // a public key is an (x, y) coordinate on the elliptic curve
  // the uncompressed key comes in the form [0x04][32 bytes of x coord][32 bytes of y coord]
  if (uncompressedBytes.size() != UNCOMPRESSED_KEY_SIZE)
  {
    throw std::invalid_argument("Input bytes size [" + std::to_string(uncompressedBytes.size()) +
                                "] is invalid: must be [" + std::to_string(UNCOMPRESSED_KEY_SIZE) + "]");
  }

  if (uncompressedBytes.at(0) != std::byte(0x04))
  {
    throw std::invalid_argument("Uncompressed bytes should begin with 0x04");
  }

  const internal::OpenSSLUtils::EC_GROUP group(EC_GROUP_new_by_curve_name(NID_secp256k1));
  if (!group)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_GROUP_new_by_curve_name"));
  }

  internal::OpenSSLUtils::EC_POINT uncompressedPoint(EC_POINT_new(group.get()));
  if (!uncompressedPoint)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_new"));
  }

  internal::OpenSSLUtils::BN_CTX context(BN_CTX_new());
  if (!context)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_CTX_new"));
  }

  // parse the uncompressed point into an EC_POINT object
  if (EC_POINT_oct2point(group.get(),
                         uncompressedPoint.get(),
                         internal::Utilities::toTypePtr<unsigned char>(uncompressedBytes.data()),
                         UNCOMPRESSED_KEY_SIZE,
                         context.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_oct2point"));
  }

  std::vector<std::byte> compressedBytes(COMPRESSED_KEY_SIZE);

  // serialize the point to compressed form
  if (EC_POINT_point2oct(group.get(),
                         uncompressedPoint.get(),
                         POINT_CONVERSION_COMPRESSED,
                         internal::Utilities::toTypePtr<unsigned char>(compressedBytes.data()),
                         COMPRESSED_KEY_SIZE,
                         context.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_point2oct"));
  }

  return compressedBytes;
}

//-----
std::vector<std::byte> ECDSAsecp256k1PublicKey::uncompressBytes(const std::vector<std::byte>& compressedBytes)
{
  // a public key is an (x, y) coordinate on the elliptic curve
  // the compressed key comes in the form [0x02 or 0x03][32 bytes of x coord]
  if (compressedBytes.size() != COMPRESSED_KEY_SIZE)
  {
    throw std::invalid_argument("Input bytes size [" + std::to_string(compressedBytes.size()) +
                                "] is invalid: must be [" + std::to_string(COMPRESSED_KEY_SIZE) + "]");
  }

  if (compressedBytes.at(0) != std::byte(0x02) && compressedBytes.at(0) != std::byte(0x03))
  {
    throw std::invalid_argument("Compressed bytes should begin with 0x02 or 0x03");
  }

  const internal::OpenSSLUtils::EC_GROUP group(EC_GROUP_new_by_curve_name(NID_secp256k1));
  if (!group)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_GROUP_new_by_curve_name"));
  }

  internal::OpenSSLUtils::EC_POINT compressedPoint(EC_POINT_new(group.get()));
  if (!compressedPoint)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_new"));
  }

  internal::OpenSSLUtils::BN_CTX context(BN_CTX_new());
  if (!context)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_CTX_new"));
  }

  // parse the compressed point into an EC_POINT object
  if (EC_POINT_oct2point(group.get(),
                         compressedPoint.get(),
                         internal::Utilities::toTypePtr<unsigned char>(compressedBytes.data()),
                         COMPRESSED_KEY_SIZE,
                         context.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_oct2point"));
  }

  std::vector<std::byte> uncompressedBytes(UNCOMPRESSED_KEY_SIZE);

  // serialize the point to uncompressed form
  if (EC_POINT_point2oct(group.get(),
                         compressedPoint.get(),
                         POINT_CONVERSION_UNCOMPRESSED,
                         internal::Utilities::toTypePtr<unsigned char>(uncompressedBytes.data()),
                         UNCOMPRESSED_KEY_SIZE,
                         context.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_point2oct"));
  }

  return uncompressedBytes;
}

//-----
std::unique_ptr<Key> ECDSAsecp256k1PublicKey::clone() const
{
  return std::make_unique<ECDSAsecp256k1PublicKey>(*this);
}

//----
std::unique_ptr<proto::Key> ECDSAsecp256k1PublicKey::toProtobufKey() const
{
  auto keyProtobuf = std::make_unique<proto::Key>();
  keyProtobuf->set_ecdsa_secp256k1(internal::Utilities::byteVectorToString(toBytesRaw()));
  return keyProtobuf;
}

//-----
bool ECDSAsecp256k1PublicKey::verifySignature(const std::vector<std::byte>& signatureBytes,
                                              const std::vector<std::byte>& signedBytes) const
{
  // incoming signatures are in the raw form (r, s), where r and s are each 32 bytes long
  if (signatureBytes.size() != ECDSAsecp256k1PrivateKey::RAW_SIGNATURE_SIZE)
  {
    return false;
  }

  // First, convert the incoming signature to DER format, so that it can be verified
  internal::OpenSSLUtils::BIGNUM signatureR(BN_bin2bn(
    internal::Utilities::toTypePtr<unsigned char>(signatureBytes.data()), ECDSAsecp256k1PrivateKey::R_SIZE, nullptr));
  if (!signatureR)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_bin2bn"));
  }

  internal::OpenSSLUtils::BIGNUM signatureS(
    BN_bin2bn(internal::Utilities::toTypePtr<unsigned char>(signatureBytes.data()) + ECDSAsecp256k1PrivateKey::R_SIZE,
              ECDSAsecp256k1PrivateKey::S_SIZE,
              nullptr));
  if (!signatureS)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_bin2bn"));
  }

  internal::OpenSSLUtils::ECDSA_SIG signatureObject(ECDSA_SIG_new());
  if (ECDSA_SIG_set0(signatureObject.get(), signatureR.get(), signatureS.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("ECDSA_SIG_set0"));
  }

  // Ownership of signatureR and signatureS has been transferred to signatureObject as a part of the previous
  // ECDSA_SIG_set0 call, so ownership should now be released
  signatureR.release();
  signatureS.release();

  // maximum length of DER encoded signature is 72
  std::vector<std::byte> derEncodedSignature(ECDSAsecp256k1PrivateKey::MAX_SIGNATURE_SIZE);

  // keep track of how long the DER encoding actually is, since we'll need to tell the verification function
  int derEncodedSignatureLength;
  if (auto encodedSignaturePointer = internal::Utilities::toTypePtr<unsigned char>(derEncodedSignature.data());
      (derEncodedSignatureLength = i2d_ECDSA_SIG(signatureObject.get(), &encodedSignaturePointer)) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_ECDSA_SIG"));
  }

  internal::OpenSSLUtils::EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_CTX_new"));
  }

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

  if (EVP_DigestVerifyInit(messageDigestContext.get(), nullptr, messageDigest.get(), nullptr, getInternalKey().get()) <=
      0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestVerifyInit"));
  }

  const int verificationResult =
    EVP_DigestVerify(messageDigestContext.get(),
                     internal::Utilities::toTypePtr<unsigned char>(derEncodedSignature.data()),
                     static_cast<size_t>(derEncodedSignatureLength),
                     internal::Utilities::toTypePtr<unsigned char>(signedBytes.data()),
                     signedBytes.size());

  // any value other than 0 or 1 means an error occurred
  if (verificationResult != 0 && verificationResult != 1)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestVerify"));
  }

  return verificationResult == 1;
}

//-----
std::string ECDSAsecp256k1PublicKey::toStringDer() const
{
  return internal::HexConverter::bytesToHex(toBytesDer());
}

//-----
std::string ECDSAsecp256k1PublicKey::toStringRaw() const
{
  return internal::HexConverter::bytesToHex(toBytesRaw());
}

//-----
std::vector<std::byte> ECDSAsecp256k1PublicKey::toBytes() const
{
  return toBytesDer();
}

//-----
std::vector<std::byte> ECDSAsecp256k1PublicKey::toBytesDer() const
{
  return internal::Utilities::concatenateVectors({ DER_ENCODED_COMPRESSED_PREFIX_BYTES, toBytesRaw() });
}

//-----
std::vector<std::byte> ECDSAsecp256k1PublicKey::toBytesRaw() const
{
  int bytesLength = i2d_PUBKEY(getInternalKey().get(), nullptr);

  std::vector<std::byte> publicKeyBytes(bytesLength);

  if (auto rawPublicKeyBytes = internal::Utilities::toTypePtr<unsigned char>(publicKeyBytes.data());
      i2d_PUBKEY(getInternalKey().get(), &rawPublicKeyBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PUBKEY"));
  }

  // Don't return the algorithm identification bytes, and compress
  return compressBytes({ publicKeyBytes.cbegin() + static_cast<long>(DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size()),
                         publicKeyBytes.cend() });
}

//-----
std::unique_ptr<proto::SignaturePair> ECDSAsecp256k1PublicKey::toSignaturePairProtobuf(
  const std::vector<std::byte>& signature) const
{
  auto signaturePair = std::make_unique<proto::SignaturePair>();
  signaturePair->set_pubkeyprefix(internal::Utilities::byteVectorToString(toBytesRaw()));
  signaturePair->set_ecdsa_secp256k1(internal::Utilities::byteVectorToString(signature));
  return signaturePair;
}

//-----
EvmAddress ECDSAsecp256k1PublicKey::toEvmAddress() const
{
  // Generate hash without "0x04" prefix of uncompressed bytes.
  return EvmAddress::fromBytes(internal::Utilities::removePrefix(
    internal::OpenSSLUtils::computeKECCAK256(internal::Utilities::removePrefix(uncompressBytes(toBytesRaw()), 1)),
    static_cast<long>(internal::OpenSSLUtils::KECCAK256_HASH_SIZE - EvmAddress::NUM_BYTES)));
}

//-----
ECDSAsecp256k1PublicKey::ECDSAsecp256k1PublicKey(internal::OpenSSLUtils::EVP_PKEY&& key)
  : PublicKey(std::move(key))
{
}

//-----
std::shared_ptr<PublicKey> ECDSAsecp256k1PublicKey::getShared() const
{
  return std::const_pointer_cast<ECDSAsecp256k1PublicKey>(
    std::dynamic_pointer_cast<const ECDSAsecp256k1PublicKey>(shared_from_this()));
}

} // namespace Hedera

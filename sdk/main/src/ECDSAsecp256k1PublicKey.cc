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
#include "ECDSAsecp256k1PublicKey.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "exceptions/BadKeyException.h"
#include "exceptions/OpenSSLException.h"
#include "impl/HexConverter.h"
#include "impl/OpenSSLUtils.h"
#include "impl/Utilities.h"

#include <openssl/decoder.h>
#include <openssl/ec.h>
#include <openssl/x509.h>
#include <proto/basic_types.pb.h>
#include <set>

namespace Hedera
{
//-----
ECDSAsecp256k1PublicKey::ECDSAsecp256k1PublicKey(const ECDSAsecp256k1PublicKey& other)
  : mPublicKey(rawBytesToPKEY(other.toBytesRaw()))
{
}

//-----
ECDSAsecp256k1PublicKey& ECDSAsecp256k1PublicKey::operator=(const ECDSAsecp256k1PublicKey& other)
{
  if (this != &other)
  {
    mPublicKey = rawBytesToPKEY(other.toBytesRaw());
  }

  return *this;
}

//-----
ECDSAsecp256k1PublicKey::ECDSAsecp256k1PublicKey(ECDSAsecp256k1PublicKey&& other) noexcept
  : mPublicKey(std::move(other.mPublicKey))
{
}

//-----
ECDSAsecp256k1PublicKey& ECDSAsecp256k1PublicKey::operator=(ECDSAsecp256k1PublicKey&& other) noexcept
{
  mPublicKey = std::move(other.mPublicKey);
  return *this;
}

//-----
std::shared_ptr<ECDSAsecp256k1PublicKey> ECDSAsecp256k1PublicKey::fromString(std::string_view key)
{
  if (key.size() == COMPRESSED_KEY_SIZE * 2 + DER_ENCODED_COMPRESSED_PREFIX_HEX.size() ||
      key.size() == UNCOMPRESSED_KEY_SIZE * 2 + DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size())
  {
    return fromStringDer(key);
  }
  else if (key.size() == COMPRESSED_KEY_SIZE * 2 || key.size() == UNCOMPRESSED_KEY_SIZE * 2)
  {
    return fromStringRaw(key);
  }
  else
  {
    throw BadKeyException("ECDSAsecp256k1PublicKey cannot be realized from input string: input string size should be " +
                          std::to_string(UNCOMPRESSED_KEY_SIZE * 2 + DER_ENCODED_UNCOMPRESSED_PREFIX_HEX.size()) +
                          " or " + std::to_string(UNCOMPRESSED_KEY_SIZE * 2) + " for uncompressed keys, or " +
                          std::to_string(COMPRESSED_KEY_SIZE * 2 + DER_ENCODED_COMPRESSED_PREFIX_HEX.size()) + " or " +
                          std::to_string(COMPRESSED_KEY_SIZE * 2) + " for compressed keys");
  }
}

//-----
std::shared_ptr<ECDSAsecp256k1PublicKey> ECDSAsecp256k1PublicKey::fromStringDer(std::string_view key)
{
  if (key.size() == COMPRESSED_KEY_SIZE * 2 + DER_ENCODED_COMPRESSED_PREFIX_HEX.size())
  {
    // Remove the DER-encoded header
    key.remove_prefix(DER_ENCODED_COMPRESSED_PREFIX_HEX.size());
    return fromStringRaw(key);
  }
  else if (key.size() == UNCOMPRESSED_KEY_SIZE * 2 + DER_ENCODED_UNCOMPRESSED_PREFIX_HEX.size())
  {
    // Remove the DER-encoded header
    key.remove_prefix(DER_ENCODED_UNCOMPRESSED_PREFIX_HEX.size());
    return fromStringRaw(key);
  }
  else
  {
    throw BadKeyException(
      "ECDSAsecp256k1PublicKey cannot be realized from input string: DER encoded "
      "ECDSAsecp256k1PublicKey hex string size should be " +
      std::to_string(COMPRESSED_KEY_SIZE * 2 + DER_ENCODED_COMPRESSED_PREFIX_HEX.size()) + " if compressed, or " +
      std::to_string(UNCOMPRESSED_KEY_SIZE * 2 + DER_ENCODED_UNCOMPRESSED_PREFIX_HEX.size()) + " if uncompressed");
  }
}

//-----
std::shared_ptr<ECDSAsecp256k1PublicKey> ECDSAsecp256k1PublicKey::fromStringRaw(std::string_view key)
{
  if (key.size() != COMPRESSED_KEY_SIZE * 2 && key.size() != UNCOMPRESSED_KEY_SIZE * 2)
  {
    throw BadKeyException("ECDSAsecp256k1PublicKey cannot be realized from input string: raw ECDSAsecp256k1PublicKey "
                          "string size should be " +
                          std::to_string(COMPRESSED_KEY_SIZE * 2) + " if compressed, or " +
                          std::to_string(UNCOMPRESSED_KEY_SIZE * 2) + " if uncompressed");
  }

  try
  {
    return std::make_shared<ECDSAsecp256k1PublicKey>(
      ECDSAsecp256k1PublicKey(rawBytesToPKEY(internal::HexConverter::hexToBytes(key))));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(std::string("ECDSAsecp256k1PublicKey cannot be realized from input string: ") +
                          openSSLException.what());
  }
  catch (const std::invalid_argument& invalidArgument)
  {
    throw BadKeyException(std::string("ECDSAsecp256k1PublicKey cannot be realized from input string: ") +
                          invalidArgument.what());
  }
}

//-----
std::shared_ptr<ECDSAsecp256k1PublicKey> ECDSAsecp256k1PublicKey::fromBytes(const std::vector<unsigned char>& bytes)
{
  if (bytes.size() == COMPRESSED_KEY_SIZE + DER_ENCODED_COMPRESSED_PREFIX_BYTES.size() ||
      bytes.size() == UNCOMPRESSED_KEY_SIZE + DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size())
  {
    return fromBytesDer(bytes);
  }
  else if (bytes.size() == COMPRESSED_KEY_SIZE || bytes.size() == UNCOMPRESSED_KEY_SIZE)
  {
    return fromBytesRaw(bytes);
  }
  else
  {
    throw BadKeyException(
      "ECDSAsecp256k1PublicKey cannot be realized from input bytes: input byte array size should be " +
      std::to_string(UNCOMPRESSED_KEY_SIZE + DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size()) + " or " +
      std::to_string(UNCOMPRESSED_KEY_SIZE) + " for uncompressed keys, or " +
      std::to_string(COMPRESSED_KEY_SIZE + DER_ENCODED_COMPRESSED_PREFIX_BYTES.size()) + " or " +
      std::to_string(COMPRESSED_KEY_SIZE) + " for compressed keys");
  }
}

//-----
std::shared_ptr<ECDSAsecp256k1PublicKey> ECDSAsecp256k1PublicKey::fromBytesDer(const std::vector<unsigned char>& bytes)
{
  if (bytes.size() == COMPRESSED_KEY_SIZE + DER_ENCODED_COMPRESSED_PREFIX_BYTES.size())
  {
    // Remove the DER-encoded header
    return fromBytesRaw(
      { bytes.cbegin() + static_cast<long>(DER_ENCODED_COMPRESSED_PREFIX_BYTES.size()), bytes.cend() });
  }
  else if (bytes.size() == UNCOMPRESSED_KEY_SIZE + DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size())
  {
    // Remove the DER-encoded header
    return fromBytesRaw(
      { bytes.cbegin() + static_cast<long>(DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size()), bytes.cend() });
  }
  else
  {
    throw BadKeyException(
      "ECDSAsecp256k1PublicKey cannot be realized from input bytes: DER encoded "
      "ECDSAsecp256k1PublicKey byte array should contain " +
      std::to_string(COMPRESSED_KEY_SIZE + DER_ENCODED_COMPRESSED_PREFIX_BYTES.size()) + " bytes if compressed, or " +
      std::to_string(UNCOMPRESSED_KEY_SIZE + DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size()) + " bytes if uncompressed");
  }
}

//-----
std::shared_ptr<ECDSAsecp256k1PublicKey> ECDSAsecp256k1PublicKey::fromBytesRaw(const std::vector<unsigned char>& bytes)
{
  if (bytes.size() != COMPRESSED_KEY_SIZE && bytes.size() != UNCOMPRESSED_KEY_SIZE)
  {
    throw BadKeyException("ECDSAsecp256k1PublicKey cannot be realized from input bytes: raw ECDSAsecp256k1PublicKey "
                          "byte array should contain " +
                          std::to_string(COMPRESSED_KEY_SIZE) + " bytes if compressed, or " +
                          std::to_string(UNCOMPRESSED_KEY_SIZE) + " bytes if uncompressed");
  }

  try
  {
    return std::make_shared<ECDSAsecp256k1PublicKey>(ECDSAsecp256k1PublicKey(rawBytesToPKEY(bytes)));
  }
  catch (const OpenSSLException& openSSLException)
  {
    throw BadKeyException(std::string("ECDSAsecp256k1PublicKey cannot be realized from input bytes: ") +
                          openSSLException.what());
  }
  catch (const std::invalid_argument& invalidArgument)
  {
    throw BadKeyException(std::string("ECDSAsecp256k1PublicKey cannot be realized from input bytes: ") +
                          invalidArgument.what());
  }
}

//-----
std::vector<unsigned char> ECDSAsecp256k1PublicKey::compressBytes(const std::vector<unsigned char>& uncompressedBytes)
{
  // a public key is an (x, y) coordinate on the elliptic curve
  // the uncompressed key comes in the form [0x04][32 bytes of x coord][32 bytes of y coord]
  if (uncompressedBytes.size() != UNCOMPRESSED_KEY_SIZE)
  {
    throw std::invalid_argument("Input bytes size [" + std::to_string(uncompressedBytes.size()) +
                                "] is invalid: must be [" + std::to_string(UNCOMPRESSED_KEY_SIZE) + "]");
  }

  if (uncompressedBytes.at(0) != 0x04)
  {
    throw std::invalid_argument("Uncompressed bytes should begin with 0x04");
  }

  const internal::OpenSSLUtils::EC_GROUP group(EC_GROUP_new_by_curve_name(NID_secp256k1));
  if (!group)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_GROUP_new_by_curve_name"));
  }

  const internal::OpenSSLUtils::EC_POINT uncompressedPoint(EC_POINT_new(group.get()));
  if (!uncompressedPoint)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_new"));
  }

  const internal::OpenSSLUtils::BN_CTX context(BN_CTX_new());
  if (!context)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_CTX_new"));
  }

  // parse the uncompressed point into an EC_POINT object
  if (EC_POINT_oct2point(
        group.get(), uncompressedPoint.get(), &uncompressedBytes.front(), UNCOMPRESSED_KEY_SIZE, context.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_oct2point"));
  }

  std::vector<unsigned char> compressedBytes(COMPRESSED_KEY_SIZE);

  // serialize the point to compressed form
  if (EC_POINT_point2oct(group.get(),
                         uncompressedPoint.get(),
                         POINT_CONVERSION_COMPRESSED,
                         &compressedBytes.front(),
                         COMPRESSED_KEY_SIZE,
                         context.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_point2oct"));
  }

  return compressedBytes;
}

//-----
std::vector<unsigned char> ECDSAsecp256k1PublicKey::uncompressBytes(const std::vector<unsigned char>& compressedBytes)
{
  // a public key is an (x, y) coordinate on the elliptic curve
  // the compressed key comes in the form [0x02 or 0x03][32 bytes of x coord]
  if (compressedBytes.size() != COMPRESSED_KEY_SIZE)
  {
    throw std::invalid_argument("Input bytes size [" + std::to_string(compressedBytes.size()) +
                                "] is invalid: must be [" + std::to_string(COMPRESSED_KEY_SIZE) + "]");
  }

  if (compressedBytes.at(0) != 0x02 && compressedBytes.at(0) != 0x03)
  {
    throw std::invalid_argument("Compressed bytes should begin with 0x02 or 0x03");
  }

  const internal::OpenSSLUtils::EC_GROUP group(EC_GROUP_new_by_curve_name(NID_secp256k1));
  if (!group)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_GROUP_new_by_curve_name"));
  }

  const internal::OpenSSLUtils::EC_POINT compressedPoint(EC_POINT_new(group.get()));
  if (!compressedPoint)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_new"));
  }

  const internal::OpenSSLUtils::BN_CTX context(BN_CTX_new());
  if (!context)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_CTX_new"));
  }

  // parse the compressed point into an EC_POINT object
  if (EC_POINT_oct2point(
        group.get(), compressedPoint.get(), &compressedBytes.front(), COMPRESSED_KEY_SIZE, context.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_oct2point"));
  }

  std::vector<unsigned char> uncompressedBytes(UNCOMPRESSED_KEY_SIZE);

  // serialize the point to uncompressed form
  if (EC_POINT_point2oct(group.get(),
                         compressedPoint.get(),
                         POINT_CONVERSION_UNCOMPRESSED,
                         &uncompressedBytes.front(),
                         UNCOMPRESSED_KEY_SIZE,
                         context.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EC_POINT_point2oct"));
  }

  return uncompressedBytes;
}

//-----
std::unique_ptr<PublicKey> ECDSAsecp256k1PublicKey::clone() const
{
  return std::make_unique<ECDSAsecp256k1PublicKey>(*this);
}

//-----
bool ECDSAsecp256k1PublicKey::verifySignature(const std::vector<unsigned char>& signatureBytes,
                                              const std::vector<unsigned char>& signedBytes) const
{
  // incoming signatures are in the raw form (r, s), where r and s are each 32 bytes long
  if (signatureBytes.size() != ECDSAsecp256k1PrivateKey::RAW_SIGNATURE_SIZE)
  {
    return false;
  }

  // First, convert the incoming signature to DER format, so that it can be verified
  internal::OpenSSLUtils::BIGNUM signatureR(
    BN_bin2bn(&signatureBytes.front(), ECDSAsecp256k1PrivateKey::R_SIZE, nullptr));
  if (!signatureR)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_bin2bn"));
  }

  internal::OpenSSLUtils::BIGNUM signatureS(
    BN_bin2bn(&signatureBytes.front() + ECDSAsecp256k1PrivateKey::R_SIZE, ECDSAsecp256k1PrivateKey::S_SIZE, nullptr));
  if (!signatureS)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("BN_bin2bn"));
  }

  const internal::OpenSSLUtils::ECDSA_SIG signatureObject(ECDSA_SIG_new());
  if (ECDSA_SIG_set0(signatureObject.get(), signatureR.get(), signatureS.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("ECDSA_SIG_set0"));
  }

  // Ownership of signatureR and signatureS has been transferred to signatureObject as a part of the previous
  // ECDSA_SIG_set0 call, so ownership should now be released
  signatureR.release();
  signatureS.release();

  // maximum length of DER encoded signature is 72
  std::vector<unsigned char> derEncodedSignature(ECDSAsecp256k1PrivateKey::DER_ENCODED_SIGNATURE_SIZE);

  // keep track of how long the DER encoding actually is, since we'll need to tell the verification function
  int derEncodedSignatureLength;
  if (unsigned char* encodedSignaturePointer = &derEncodedSignature.front();
      (derEncodedSignatureLength = i2d_ECDSA_SIG(signatureObject.get(), &encodedSignaturePointer)) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_ECDSA_SIG"));
  }

  const internal::OpenSSLUtils::EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_CTX_new"));
  }

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

  if (EVP_DigestVerifyInit(messageDigestContext.get(), nullptr, messageDigest.get(), nullptr, mPublicKey.get()) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestVerifyInit"));
  }

  const int verificationResult = EVP_DigestVerify(messageDigestContext.get(),
                                                  &derEncodedSignature.front(),
                                                  static_cast<size_t>(derEncodedSignatureLength),
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
std::vector<unsigned char> ECDSAsecp256k1PublicKey::toBytesDer() const
{
  return internal::Utilities::appendVector(DER_ENCODED_COMPRESSED_PREFIX_BYTES, toBytesRaw());
}

//-----
std::vector<unsigned char> ECDSAsecp256k1PublicKey::toBytesRaw() const
{
  int bytesLength = i2d_PUBKEY(mPublicKey.get(), nullptr);

  std::vector<unsigned char> publicKeyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &publicKeyBytes.front(); i2d_PUBKEY(mPublicKey.get(), &rawPublicKeyBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PUBKEY"));
  }

  if (publicKeyBytes.size() != UNCOMPRESSED_KEY_SIZE + DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size())
  {
    throw OpenSSLException("Expected public key size [" +
                           std::to_string(UNCOMPRESSED_KEY_SIZE + DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size()) +
                           "]. Actual size was [" + std::to_string(publicKeyBytes.size()) + "]");
  }

  // don't return the algorithm identification bytes, and compress
  return compressBytes(
    { publicKeyBytes.begin() + static_cast<long>(DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES.size()), publicKeyBytes.end() });
}

//----
std::unique_ptr<proto::Key> ECDSAsecp256k1PublicKey::toProtobuf() const
{
  auto keyProtobuf = std::make_unique<proto::Key>();
  const std::vector<unsigned char> rawBytes = toBytesRaw();
  keyProtobuf->set_allocated_ecdsa_secp256k1(new std::string({ rawBytes.cbegin(), rawBytes.cend() }));
  return keyProtobuf;
}

//-----
internal::OpenSSLUtils::EVP_PKEY ECDSAsecp256k1PublicKey::rawBytesToPKEY(const std::vector<unsigned char>& bytes)
{
  if (bytes.size() != COMPRESSED_KEY_SIZE && bytes.size() != UNCOMPRESSED_KEY_SIZE)
  {
    throw std::invalid_argument("Input bytes size [" + std::to_string(bytes.size()) + "] is invalid: must be either [" +
                                std::to_string(UNCOMPRESSED_KEY_SIZE) + "] or [" + std::to_string(COMPRESSED_KEY_SIZE) +
                                "]");
  }

  // OpenSSL requires that the bytes are uncompressed and that they contain the appropriate ASN1 prefix
  const std::vector<unsigned char> uncompressedKeyBytes =
    (bytes.size() == COMPRESSED_KEY_SIZE)
      ? internal::Utilities::appendVector(DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES, uncompressBytes(bytes))
      : internal::Utilities::appendVector(DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES, bytes);

  EVP_PKEY* pkey = nullptr;
  const internal::OpenSSLUtils::OSSL_DECODER_CTX context(
    OSSL_DECODER_CTX_new_for_pkey(&pkey, "DER", nullptr, "EC", EVP_PKEY_PUBLIC_KEY, nullptr, nullptr));
  if (!context)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OSSL_DECODER_CTX_new_for_pkey"));
  }

  size_t dataLength = uncompressedKeyBytes.size();
  if (const unsigned char* rawKeyBytes = &uncompressedKeyBytes.front();
      OSSL_DECODER_from_data(context.get(), &rawKeyBytes, &dataLength) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OSSL_DECODER_from_data"));
  }

  return internal::OpenSSLUtils::EVP_PKEY(pkey);
}

//-----
ECDSAsecp256k1PublicKey::ECDSAsecp256k1PublicKey(internal::OpenSSLUtils::EVP_PKEY&& publicKey)
  : mPublicKey(std::move(publicKey))
{
}

} // namespace Hedera

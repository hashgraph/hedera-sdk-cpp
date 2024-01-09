/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file
 * Defines the Hedera C++ SDK's ASN1ECPublicKey class, derived from ASN1ECKey.
 */

#ifndef HEDERA_SDK_CPP_IMPL_ASN1_EC_PUBLIC_KEY_H_
#define HEDERA_SDK_CPP_IMPL_ASN1_EC_PUBLIC_KEY_H_

#include "ASN1ECKey.h"

namespace Hedera::internal::asn1
{
/**
 * The ASN.1 algorithm identifier prefix bytes for a compressed
 * ECDSA secp256k1 public key.
 * @details
 * Byte sequence: 3036301006072A8648CE3D020106052B8104000A0322
 */
const std::vector<std::byte> ASN1_CPUBK_PREFIX_BYTES = {
  std::byte(0x30), std::byte(0x36), std::byte(0x30), std::byte(0x10), std::byte(0x06), std::byte(0x07),
  std::byte(0x2A), std::byte(0x86), std::byte(0x48), std::byte(0xCE), std::byte(0x3D), std::byte(0x02),
  std::byte(0x01), std::byte(0x06), std::byte(0x05), std::byte(0x2B), std::byte(0x81), std::byte(0x04),
  std::byte(0x00), std::byte(0x0A), std::byte(0x03), std::byte(0x22)
};

/**
 * The ASN.1 algorithm identifier prefix bytes for an uncompressed
 * ECDSA secp256k1 public key.
 * @details
 * Byte sequence: 3056301006072A8648CE3D020106052B8104000A0342
 */
const std::vector<std::byte> ASN1_UPUBK_PREFIX_BYTES = {
  std::byte(0x30), std::byte(0x56), std::byte(0x30), std::byte(0x10), std::byte(0x06), std::byte(0x07),
  std::byte(0x2A), std::byte(0x86), std::byte(0x48), std::byte(0xCE), std::byte(0x3D), std::byte(0x02),
  std::byte(0x01), std::byte(0x06), std::byte(0x05), std::byte(0x2B), std::byte(0x81), std::byte(0x04),
  std::byte(0x00), std::byte(0x0A), std::byte(0x03), std::byte(0x42)
};

// PEM Format prefix/suffix string
constexpr std::string_view PEM_ECPUBK_PREFIX_STRING = "-----BEGIN EC PUBLIC KEY-----";
constexpr std::string_view PEM_ECPUBK_SUFFIX_STRING = "-----END EC PUBLIC KEY-----";

/**
 * @class ASN1ECPublicKey
 * ASN.1 elliptic curve public key object.
 */
class ASN1ECPublicKey : public ASN1ECKey
{
public:
  /**
   * Constructor for ASN.1 elliptic curve public key from a vector of bytes.
   * @param bytes The vector of bytes containing the ASN.1 key data.
   */
  ASN1ECPublicKey(const std::vector<std::byte>& bytes);

  /**
   * Get the key value associated with the ASN.1 elliptic curve public key.
   * @return The key as a vector of bytes.
   */
  std::vector<std::byte> getKey() const override;

private:
  /**
   * Constructor for ASN.1 elliptic curve public key.
   */
  ASN1ECPublicKey() = default;

  /**
   * Populate X and Y coordinates from the ASN.1 key bytes.
   */
  void populateXYcoords();

  std::vector<std::byte> ecXcoord; ///< X coordinate of the elliptic curve public key.
  std::vector<std::byte> ecYcoord; ///< Y coordinate of the elliptic curve public key.
};

} // namespace Hedera::internal:asn1

#endif // HEDERA_SDK_CPP_IMPL_ASN1_EC_PUBLIC_KEY_H_

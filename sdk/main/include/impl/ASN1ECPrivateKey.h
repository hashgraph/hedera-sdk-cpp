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
 * @brief This file defines the Hedera C++ SDK's ASN1Key class, derived from ASN1Object.
 */

#ifndef HEDERA_SDK_CPP_IMPL_ASN1_EC_PRIVATE_KEY_H_
#define HEDERA_SDK_CPP_IMPL_ASN1_EC_PRIVATE_KEY_H_

#include "ASN1ECKey.h"

namespace Hedera::internal::asn1
{
// The ASN.1 algorithm identifier prefix bytes for an ECDSAsecp256k1PrKeyKey.
const std::vector<std::byte> ASN1_PRK_PREFIX_BYTES = { std::byte(0x30), std::byte(0x2E), std::byte(0x02), std::byte(0x01),
                                                       std::byte(0x01), std::byte(0x04), std::byte(0x20) };
                                       
// The ASN.1 algorithm identifier suffix bytes for an ECDSAsecp256k1Key.
const std::vector<std::byte> ASN1_PRK_SUFFIX_BYTES = { std::byte(0xA0), std::byte(0x07), std::byte(0x06),
                                                   std::byte(0x05), std::byte(0x2B), std::byte(0x81),
                                                   std::byte(0x04), std::byte(0x00), std::byte(0x0A) };
/**
 * @class ASN1Key
 * @brief ASN.1 key object.
 */
class ASN1ECPrivateKey : public ASN1ECKey
{
public:

  /**
   * @brief Constructor for ASN.1 key from a vector of bytes.
   *
   * @param bytes The vector of bytes containing the ASN.1 key data.
   */
  ASN1ECPrivateKey(const std::vector<std::byte>& bytes);

  /**
   * @brief Get the key value associated with the ASN.1 key.
   *
   * @return The key as a vector of bytes.
   */
  const std::vector<std::byte> getKey() const override;

private:
  /**
   * @brief Constructor for ASN.1 key.
   */
  ASN1ECPrivateKey(){};

  /**
   * @brief Decode ASN.1 data representing an Elliptic Curve Key.
   *
   * This method decodes basic ASN.1 data, extracting key data and storing it in the `asn1KeyData` map.
   * EC Keys in ASN1 format always follow a common structure:
   * 
   *   ECPrivateKey ::= SEQUENCE {
   *   version INTEGER { ecPrivkeyVer1(1) } (ecPrivkeyVer1),
   *   privateKey OCTET STRING,
   *   parameters [0] ECParameters {{ NamedCurve }} OPTIONAL,
   *   publicKey [1] BIT STRING OPTIONAL
   *   }
   *
   * @param bytes The ASN.1-encoded data representing the Elliptic Curve Key.
   */
void decode(const std::vector<std::byte>& bytes) override;

  /**
   * @brief Get the value associated with the given ASN.1 tag.
   *
   * @param tag The ASN.1 tag.
   * @return The value associated with the tag as a vector of bytes.
   */
const std::vector<std::byte> get(const std::byte tag) const override;


};

} // namespace Hedera::internal:asn1

#endif // HEDERA_SDK_CPP_IMPL_ASN1_EC_PRIVATE_KEY_H_

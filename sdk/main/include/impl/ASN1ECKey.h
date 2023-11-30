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

#ifndef HEDERA_SDK_CPP_IMPL_ASN1_EC_KEY_H_
#define HEDERA_SDK_CPP_IMPL_ASN1_EC_KEY_H_

#include "ASN1Object.h"

namespace Hedera::internal::asn1
{
constexpr size_t ECDSA_KEY_LENGTH = 32; // bytes

/**
 * @class ASN1Key
 * @brief ASN.1 key object.
 */
class ASN1ECKey : public ASN1Object
{
public:
  /**
   * @brief Get the key value associated with the ASN.1 key.
   *
   * @return The key as a vector of bytes.
   */
  virtual std::vector<std::byte> getKey() const = 0;

protected:
  /**
   * @brief Decode ASN.1 data representing an Elliptic Curve Key.
   *
   * This method decodes basic ASN.1 data, extracting key data and storing it in the `asn1KeyData` map.
   * EC Keys in ASN1 format always follow a common structure:
   *
   *   ECKey ::= SEQUENCE {
   *   version INTEGER { ecPrivkeyVer1(1) } (ecPrivkeyVer1) OPTIONAL,
   *   key STRING_DATA,
   *   parameters [0] ECParameters {{ NamedCurve }} OPTIONAL,
   *   otherKey [1] STRING_DATA OPTIONAL
   *   }
   *
   * @param bytes The ASN.1-encoded data representing the Elliptic Curve Key.
   */
  virtual void decode(const std::vector<std::byte>& bytes);

  /**
   * @brief Get the value associated with the given ASN.1 tag.
   *
   * @param tag The ASN.1 tag.
   * @return The value associated with the tag as a vector of bytes.
   */
  virtual const std::vector<std::byte> get(const std::byte tag) const;

  /**
   * @brief A map to store ASN.1 key data with their associated tags.
   */
  std::unordered_map<std::byte, std::vector<std::byte>> asn1KeyData;
};

} // namespace Hedera::internal:asn1

#endif // HEDERA_SDK_CPP_IMPL_ASN1_EC_KEY_H_

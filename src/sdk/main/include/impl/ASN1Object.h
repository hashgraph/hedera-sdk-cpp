/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
 * This file defines the Hedera C++ SDK's ASN1Object class.
 */

#ifndef HEDERA_SDK_CPP_IMPL_ASN1_OBJECT_H_
#define HEDERA_SDK_CPP_IMPL_ASN1_OBJECT_H_

#include <iostream>
#include <unordered_map>
#include <vector>

namespace Hedera::internal::asn1
{

/**
 * Constants for ASN.1 standard.
 */
constexpr std::byte INTEGER = std::byte(0x02);
constexpr std::byte BIT_STRING = std::byte(0x03);
constexpr std::byte OCTET_STRING = std::byte(0x04);
constexpr std::byte OBJECT_IDENTIFIER = std::byte(0x06);
constexpr std::byte SEQUENCE = std::byte(0x30);

/**
 * @class ASN1Object
 * Abstract base class for ASN.1 objects.
 */
class ASN1Object
{
protected:
  /**
   * Get the ASN.1 object's value in bytes.
   *
   * @param tag The ASN.1 tag of the object.
   * @return The object's value in bytes.
   */
  virtual const std::vector<std::byte> get(const std::byte tag) const = 0;

  /**
   * Decode the ASN.1 object from a vector of bytes.
   *
   * @param data The vector of bytes containing the ASN.1 object's data.
   */
  virtual void decode(const std::vector<std::byte>& data) = 0;
};

} // namespace Hedera::internal::asn1

#endif // HEDERA_SDK_CPP_IMPL_ASN1_OBJECT_H_
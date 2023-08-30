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
#ifndef HEDERA_SDK_CPP_IMPL_IPV4_ADDRESS_H_
#define HEDERA_SDK_CPP_IMPL_IPV4_ADDRESS_H_

#include <string>
#include <string_view>

namespace Hedera::internal
{
/**
 * An IPv4 address (without port)
 */
class IPv4Address
{
public:
  IPv4Address() = default;

  /**
   * Constructor with four octets.
   *
   * @param octet1 The first octet.
   * @param octet2 The second octet.
   * @param octet3 The third octet.
   * @param octet4 The fourth octet.
   */
  IPv4Address(std::byte octet1, std::byte octet2, std::byte octet3, std::byte octet4);

  /**
   * Creates a new IP address from a string. Supports ascii or byte representation.
   *
   * @param address The IP address from which to construct, in string form.
   * @return The new IP address.
   * @throws std::invalid_argument If the input IPv4Address string is malformed.
   */
  [[nodiscard]] static IPv4Address fromString(std::string_view address);

  /**
   * Get the string representation of this IPv4Address (form is <octet>.<octet>.<octet>.<octet>).
   *
   * @return The string represenation of this IPv4Address.
   */
  [[nodiscard]] std::string toString() const;

private:
  /**
   * The first octet of the address
   */
  std::byte mOctet1;

  /**
   * The second octet of the address
   */
  std::byte mOctet2;

  /**
   * The third octet of the address
   */
  std::byte mOctet3;

  /**
   * The fourth octet of the address
   */
  std::byte mOctet4;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_IPV4_ADDRESS_H_

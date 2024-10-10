/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_IPV4_ADDRESS_H_
#define HEDERA_SDK_CPP_IPV4_ADDRESS_H_

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace Hedera
{
/**
 * An IPv4 address (without port).
 */
class IPv4Address
{
public:
  /**
   * Construct an IPv4Address object from a byte array.
   *
   * @param bytes The byte array from which to construct an IPv4Address.
   * @return The constructed IPv4Address object.
   * @throws std::invalid_argument If an IPv4Address could not be realized from the input bytes.
   */
  [[nodiscard]] static IPv4Address fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Get the byte array representation of this IPv4Address.
   *
   * @return The byte array representation of this IPv4Address.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Get the string representation of this IPv4Address.
   *
   * @return The string representation of this IPv4Address.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Check if the IPV4Address is empty.
   *
   * @return The boolean representation of an IPV4Address empty state.
   */
  [[nodiscard]] bool isEmpty() const;

private:
  /**
   * The four octets of the address.
   */
  std::vector<std::byte> mAddress;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_IPV4_ADDRESS_H_

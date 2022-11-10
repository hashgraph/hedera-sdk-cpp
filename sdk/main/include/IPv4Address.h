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

#ifndef HEDERA_SDK_CPP_IPV4ADDRESS_H
#define HEDERA_SDK_CPP_IPV4ADDRESS_H

#include <string>

namespace Hedera
{

/**
 * An IPv4 address (without port)
 */
class IPv4Address
{
public:
  /**
   * Default constructor
   */
  IPv4Address() = default;

  /**
   * Constructor which initializes address octets
   */
  IPv4Address(unsigned char octet1, unsigned char octet2, unsigned char octet3, unsigned char octet4);

  /**
   * Creates a new IP address from a string. Supports ascii or byte representation
   *
   * @param inputString string representing an IP address
   *
   * @return the new IP address. Currently returns address of 0.0.0.0 if there is an error in creation
   */
  static IPv4Address fromString(const std::string& inputString);

  /**
   * Converts address to form octet1.octet2.octet3.octet4
   *
   * @return string representing the address
   */
  [[nodiscard]] std::string toString() const;

private:
  /**
   * The first octet of the address
   */
  unsigned char mOctet1;

  /**
   * The second octet of the address
   */
  unsigned char mOctet2;

  /**
   * The third octet of the address
   */
  unsigned char mOctet3;

  /**
   * The fourth octet of the address
   */
  unsigned char mOctet4;
};

} // Hedera

#endif // HEDERA_SDK_CPP_IPV4ADDRESS_H

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

#include "IPv4AddressPart.h"
#include <string>

namespace Hedera
{

class IPv4Address
{
public:
  static IPv4Address fromString(const std::string& inputString);

private:
  /**
   * The first 16 bits of the IPV4 address
   */
  IPv4AddressPart network;

  /**
   * The last 16 bits of the IPV4 address
   */
  IPv4AddressPart host;
};

} // Hedera

#endif // HEDERA_SDK_CPP_IPV4ADDRESS_H

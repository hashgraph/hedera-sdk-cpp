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

#ifndef HEDERA_SDK_CPP_IPV4ADDRESSPART_H
#define HEDERA_SDK_CPP_IPV4ADDRESSPART_H

#import "../../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/usr/include/c++/v1/cstddef"

namespace Hedera
{
/**
 * Class representing a 16 bit half of an IPv4Address
 */
class IPv4AddressPart
{
private:
  std::byte leftByte;
  std::byte rightByte;

  IPv4AddressPart& setLeftByte(std::byte byteValue);
  IPv4AddressPart& setRightByte(std::byte byteValue);
  std::byte getLeftByte();
  std::byte getRightByte();
};

} // Hedera

#endif // HEDERA_SDK_CPP_IPV4ADDRESSPART_H

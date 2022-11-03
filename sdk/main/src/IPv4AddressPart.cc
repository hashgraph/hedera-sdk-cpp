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

#include "IPv4AddressPart.h"

namespace Hedera
{

IPv4AddressPart& IPv4AddressPart::setLeftByte(std::byte byteValue)
{
  leftByte = byteValue;
  return *this;
}

IPv4AddressPart& IPv4AddressPart::setRightByte(std::byte byteValue)
{
  rightByte = byteValue;
  return *this;
}

std::byte IPv4AddressPart::getLeftByte()
{
  return leftByte;
}

std::byte IPv4AddressPart::getRightByte()
{
  return rightByte;
}
} // Hedera

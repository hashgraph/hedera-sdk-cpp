/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "helper/HexConverter.h"

#include <iomanip>
#include <sstream>

namespace Hedera
{
std::string HexConverter::bytesToHex(const unsigned char* data, int len)
{
  std::stringstream ss;
  ss << std::hex;

  for (int i(0); i < len; ++i)
  {
    ss << std::setw(2) << std::setfill('0') << (int)data[i];
  }

  return ss.str();
}
}

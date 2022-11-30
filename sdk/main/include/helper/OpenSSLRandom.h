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

#ifndef HEDERA_SDK_CPP_OPENSSLRANDOM_H
#define HEDERA_SDK_CPP_OPENSSLRANDOM_H

#include <vector>

namespace Hedera
{

/**
 * Utility class wrapping around OpenSSL random functions
 */
class OpenSSLRandom
{
public:
  OpenSSLRandom() = delete;

  /**
   * Gets a vector of random bytes
   *
   * @param count number of random bytes to generate
   *
   * @return the vector of random bytes
   */
  static std::vector<unsigned char> getRandomBytes(int count);
};

} // Hedera

#endif // HEDERA_SDK_CPP_OPENSSLRANDOM_H

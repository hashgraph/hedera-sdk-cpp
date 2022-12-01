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

#include "helper/OpenSSLRandom.h"

#include <openssl/rand.h>

namespace Hedera::OpenSSLRandom
{
std::vector<unsigned char> getRandomBytes(int count)
{
  if (count <= 0)
  {
    throw std::invalid_argument("count must be positive");
  }

  std::vector<unsigned char> randomBytes(count);

  if (RAND_priv_bytes(&randomBytes.front(), count) <= 0)
  {
    throw std::runtime_error("RAND_priv_bytes failure");
  }

  return randomBytes;
}
} // namespace Hedera::OpenSSLRandom

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
#include "impl/OpenSSLRandom.h"
#include "exceptions/OpenSSLException.h"
#include "impl/OpenSSLHasher.h"

#include <openssl/rand.h>

namespace Hedera::internal::OpenSSLRandom
{
//-----
std::vector<unsigned char> getRandomBytes(int count)
{
  if (count <= 0)
  {
    throw std::invalid_argument("The number of random bytes to generate must be positive");
  }

  std::vector<unsigned char> randomBytes(count);

  if (RAND_priv_bytes(&randomBytes.front(), count) <= 0)
  {
    throw OpenSSLException(OpenSSLHasher::getOpenSSLErrorMessage("RAND_priv_bytes"));
  }

  return randomBytes;
}

} // namespace Hedera::internal::OpenSSLRandom

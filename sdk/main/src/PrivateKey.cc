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
#include "PrivateKey.h"
#include "PublicKey.h"
#include "exceptions/OpenSSLException.h"

#include <openssl/x509.h>

namespace Hedera
{
//-----
PrivateKey::PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& keypair, std::vector<unsigned char> chainCode)
  : mKeypair(std::move(keypair))
  , mChainCode(std::move(chainCode))
  , mPublicKey(PublicKey::fromBytesDer(getPublicKeyBytes()))
{
}

//-----
std::vector<unsigned char> PrivateKey::getPublicKeyBytes() const
{
  int bytesLength = i2d_PUBKEY(mKeypair.get(), nullptr);

  std::vector<unsigned char> keyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &keyBytes.front(); i2d_PUBKEY(mKeypair.get(), &rawPublicKeyBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PUBKEY"));
  }

  return keyBytes;
}

} // namespace Hedera

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
#include "impl/PrivateKeyImpl.h"

#include <openssl/x509.h>

namespace Hedera
{
//-----
PrivateKey::~PrivateKey() = default;

//-----
std::vector<unsigned char> PrivateKey::getChainCode() const
{
  return mImpl->mChainCode;
}

//-----
std::shared_ptr<PublicKey> PrivateKey::getPublicKey() const
{
  return mImpl->mPublicKey;
}

//-----
PrivateKey::PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& key, std::vector<unsigned char> chainCode)
  : mImpl(PrivateKeyImpl())
{
  mImpl->mKey = std::move(key);
  mImpl->mChainCode = std::move(chainCode);
  mImpl->mPublicKey = PublicKey::fromBytesDer(getPublicKeyBytes());
}

//-----
std::vector<unsigned char> PrivateKey::getPublicKeyBytes() const
{
  int bytesLength = i2d_PUBKEY(mImpl->mKey.get(), nullptr);

  std::vector<unsigned char> keyBytes(bytesLength);

  if (unsigned char* rawPublicKeyBytes = &keyBytes.front(); i2d_PUBKEY(mImpl->mKey.get(), &rawPublicKeyBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PUBKEY"));
  }

  return keyBytes;
}

//-----
internal::OpenSSLUtils::EVP_PKEY PrivateKey::getInternalKey() const
{
  return mImpl->mKey;
}

} // namespace Hedera

/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "exceptions/BadKeyException.h"
#include "exceptions/OpenSSLException.h"
#include "impl/PrivateKeyImpl.h"
#include "impl/Utilities.h"
#include "impl/openssl_utils/OpenSSLUtils.h"

#include <openssl/x509.h>

namespace Hedera
{
//-----
PrivateKey::~PrivateKey() = default;

//-----
std::vector<std::byte> PrivateKey::getChainCode() const
{
  return mImpl->mChainCode;
}

//-----
std::shared_ptr<PublicKey> PrivateKey::getPublicKey() const
{
  return mImpl->mPublicKey;
}

//-----
PrivateKey::PrivateKey(const PrivateKey& other)
  : mImpl(std::make_unique<PrivateKeyImpl>(*other.mImpl))
{
}

//-----
PrivateKey& PrivateKey::operator=(const PrivateKey& other)
{
  if (this != &other)
  {
    mImpl = std::make_unique<PrivateKeyImpl>(*other.mImpl);
  }

  return *this;
}

//-----
PrivateKey::PrivateKey(PrivateKey&& other) noexcept
  : mImpl(std::move(other.mImpl))
{
  // Leave the moved-from PrivateKey in a valid state.
  other.mImpl = std::make_unique<PrivateKeyImpl>();
}

//-----
PrivateKey& PrivateKey::operator=(PrivateKey&& other) noexcept
{
  if (this != &other)
  {
    mImpl = std::move(other.mImpl);

    // Leave the moved-from PrivateKey in a valid state.
    other.mImpl = std::make_unique<PrivateKeyImpl>();
  }

  return *this;
}

//-----
PrivateKey::PrivateKey(internal::OpenSSLUtils::EVP_PKEY&& key, std::vector<std::byte> chainCode)
  : mImpl(std::make_unique<PrivateKeyImpl>())
{
  mImpl->mKey = std::move(key);
  mImpl->mChainCode = std::move(chainCode);

  if (!mImpl->mChainCode.empty() && mImpl->mChainCode.size() != CHAIN_CODE_SIZE)
  {
    throw BadKeyException("Key chain code malformed");
  }

  std::vector<std::byte> keyBytes(i2d_PUBKEY(mImpl->mKey.get(), nullptr));

  if (unsigned char* rawPublicKeyBytes = internal::Utilities::toTypePtr<unsigned char>(keyBytes.data());
      i2d_PUBKEY(mImpl->mKey.get(), &rawPublicKeyBytes) <= 0)
  {
    throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("i2d_PUBKEY"));
  }

  mImpl->mPublicKey = PublicKey::fromBytesDer(keyBytes);
}

//-----
internal::OpenSSLUtils::EVP_PKEY PrivateKey::getInternalKey() const
{
  return mImpl->mKey;
}

} // namespace Hedera

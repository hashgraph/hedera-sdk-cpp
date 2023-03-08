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
#include "PublicKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PublicKey.h"
#include "exceptions/BadKeyException.h"
#include "impl/Utilities.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
std::shared_ptr<PublicKey> PublicKey::fromProtobuf(const proto::Key& key)
{
  if (key.key_case() == proto::Key::KeyCase::kEd25519)
  {
    return ED25519PublicKey::fromBytes({ key.ed25519().cbegin(), key.ed25519().cend() });
  }
  else if (key.key_case() == proto::Key::KeyCase::kECDSASecp256K1)
  {
    return ECDSAsecp256k1PublicKey::fromBytes({ key.ecdsa_secp256k1().cbegin(), key.ecdsa_secp256k1().cend() });
  }
  else
  {
    return nullptr;
  }
}

//-----
std::shared_ptr<PublicKey> PublicKey::fromStringDer(std::string_view key)
{
  if (key.find(ED25519PublicKey::DER_ENCODED_PREFIX_HEX) == 0UL)
  {
    return ED25519PublicKey::fromStringDer(key);
  }

  else if (key.find(ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_HEX) == 0UL)
  {
    return ECDSAsecp256k1PublicKey::fromStringDer(key);
  }

  throw BadKeyException("Key type cannot be determined from input DER-encoded hex string");
}

//-----
std::shared_ptr<PublicKey> PublicKey::fromBytesDer(const std::vector<unsigned char>& bytes)
{
  if (internal::Utilities::isPrefixOf(bytes, ED25519PublicKey::DER_ENCODED_PREFIX_BYTES))
  {
    return ED25519PublicKey::fromBytesDer(bytes);
  }

  else if (internal::Utilities::isPrefixOf(bytes, ECDSAsecp256k1PublicKey::DER_ENCODED_COMPRESSED_PREFIX_BYTES) ||
           internal::Utilities::isPrefixOf(bytes, ECDSAsecp256k1PublicKey::DER_ENCODED_UNCOMPRESSED_PREFIX_BYTES))
  {
    return ECDSAsecp256k1PublicKey::fromBytesDer(bytes);
  }

  throw BadKeyException("Key type cannot be determined from input DER-encoded byte array");
}

//-----
PublicKey::PublicKey(internal::OpenSSLUtils::EVP_PKEY&& keypair)
  : mKeypair(std::move(keypair))
{
}

} // namespace Hedera

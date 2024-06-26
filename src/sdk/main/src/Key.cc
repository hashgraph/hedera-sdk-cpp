/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "Key.h"
#include "ContractId.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PublicKey.h"
#include "KeyList.h"
#include "impl/Utilities.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
std::unique_ptr<Key> Key::fromProtobuf(const proto::Key& key)
{
  switch (key.key_case())
  {
    case proto::Key::KeyCase::kContractID:
      return std::make_unique<ContractId>(ContractId::fromProtobuf(key.contractid()));
    case proto::Key::KeyCase::kEd25519:
      return ED25519PublicKey::fromBytes(internal::Utilities::stringToByteVector(key.ed25519()));
    case proto::Key::KeyCase::kECDSASecp256K1:
      return ECDSAsecp256k1PublicKey::fromBytes(internal::Utilities::stringToByteVector(key.ecdsa_secp256k1()));
    case proto::Key::KeyCase::kKeyList:
      return std::make_unique<KeyList>(KeyList::fromProtobuf(key.keylist()));
    case proto::Key::KeyCase::kThresholdKey:
      return std::make_unique<KeyList>(KeyList::fromProtobuf(key.thresholdkey()));
    default:
      throw std::invalid_argument("Key protobuf case not recognized");
  }
}

//-----
std::vector<std::byte> Key::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobufKey()->SerializeAsString());
}

} // namespace Hedera

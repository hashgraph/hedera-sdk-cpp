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
#include "key/KeyType.h"

#include <string>
#include <unordered_map>

namespace Hedera::TCK::KeyService
{
//-----
const std::unordered_map<std::string, KeyType> gStringToKeyType = {
  {"ed25519PrivateKey",         KeyType::ED25519_PRIVATE_KEY_TYPE        },
  { "ed25519PublicKey",         KeyType::ED25519_PUBLIC_KEY_TYPE         },
  { "ecdsaSecp256k1PrivateKey", KeyType::ECDSA_SECP256k1_PRIVATE_KEY_TYPE},
  { "ecdsaSecp256k1PublicKey",  KeyType::ECDSA_SECP256k1_PUBLIC_KEY_TYPE },
  { "keyList",                  KeyType::LIST_KEY_TYPE                   },
  { "thresholdKey",             KeyType::THRESHOLD_KEY_TYPE              },
  { "evmAddress",               KeyType::EVM_ADDRESS_KEY_TYPE            }
};

//-----
const std::unordered_map<KeyType, std::string> gKeyTypeToString = {
  {KeyType::ED25519_PRIVATE_KEY_TYPE,          "ed25519PrivateKey"       },
  { KeyType::ED25519_PUBLIC_KEY_TYPE,          "ed25519PublicKey"        },
  { KeyType::ECDSA_SECP256k1_PRIVATE_KEY_TYPE, "ecdsaSecp256k1PrivateKey"},
  { KeyType::ECDSA_SECP256k1_PUBLIC_KEY_TYPE,  "ecdsaSecp256k1PublicKey" },
  { KeyType::LIST_KEY_TYPE,                    "keyList"                 },
  { KeyType::THRESHOLD_KEY_TYPE,               "thresholdKey"            },
  { KeyType::EVM_ADDRESS_KEY_TYPE,             "evmAddress"              }
};

} // namespace Hedera::TCK::KeyService
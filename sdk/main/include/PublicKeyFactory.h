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
#ifndef PUBLIC_KEY_FACTORY_H_
#define PUBLIC_KEY_FACTORY_H_

#include <memory.h>

#include "ED25519PublicKey.h"
#include "PublicKey.h"

namespace Hedera
{
class PublicKeyFactory
{
public:
  static std::shared_ptr<PublicKey> fromProtobuf(const proto::Key& key)
  {
    switch (key.key_case())
    {
      case proto::Key::KeyCase::kEd25519:
      {
        std::string keyString = key.ed25519();
        unsigned char keyBytes[20]; // TODO define a key length variable somewhere, 20 isn't correct
        std::copy(keyString.begin(), keyString.end(), keyBytes);

        return std::make_shared<ED25519PublicKey>(ED25519PublicKey(keyBytes));
      }
      default:
      {
        // TODO throw
        break;
      }
    }
  }

  static std::shared_ptr<PublicKey> fromAliasBytes(const std::string& bytes)
  {
    // TODO this implementation is meaningless, only acts as a stub
    unsigned char keyBytes[20];
    return std::make_shared<ED25519PublicKey>(ED25519PublicKey(keyBytes));
  }
};
}
#endif // PUBLIC_KEY_FACTORY_H_
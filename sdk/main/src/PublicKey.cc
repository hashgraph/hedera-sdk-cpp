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
#include "ED25519PublicKey.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
std::shared_ptr<PublicKey> PublicKey::fromProtobuf(const proto::Key& key)
{
  switch (key.key_case())
  {
    case proto::Key::KeyCase::kEd25519:
    {
      return ED25519PublicKey::fromBytes({ key.ed25519().cbegin(), key.ed25519().cend() });
    }
    default:
    {
      return std::shared_ptr<PublicKey>();
    }
  }
}

} // namespace Hedera

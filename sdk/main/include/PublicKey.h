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
#ifndef PUBLIC_KEY_H_
#define PUBLIC_KEY_H_

#include "Key.h"

namespace Hedera
{
class PublicKey : public Key
{
public:
  static PublicKey fromAliasBytes(const std::string& bytes)
  {
    return PublicKey();
  }

  bool operator==(const PublicKey& key) const { return true; }

  std::string toStringDER() const { return std::string(); }
};

} // namespace Hedera

#endif // PUBLIC_KEY_H_
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
#ifndef PUBLIC_KEY_H_
#define PUBLIC_KEY_H_

#include <memory>
#include <string>
#include <vector>

namespace proto
{
class Key;
}

namespace Hedera
{
class PrivateKey;
};

namespace Hedera
{
class PublicKey
{
public:
  virtual ~PublicKey() = default;

  [[nodiscard]] virtual proto::Key* toProtobuf() const = 0;
  [[nodiscard]] virtual bool verifySignature(const std::vector<unsigned char>& signatureBytes,
                                             const std::vector<unsigned char>& signedBytes) const = 0;

  static std::shared_ptr<PublicKey> fromProtobuf(const proto::Key& key);
  static std::shared_ptr<PublicKey> fromAliasBytes(const std::string&);

  [[nodiscard]] virtual std::string toString() const = 0;

  [[nodiscard]] virtual std::shared_ptr<PrivateKey> getPrivateKey() const = 0;
};

} // namespace Hedera

#endif // PUBLIC_KEY_H_
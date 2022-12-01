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
/**
 * A generic class representing a public key
 */
class PublicKey
{
public:
  /**
   * Default destructor
   */
  virtual ~PublicKey() = default;

  /**
   * Convert the public key to a protobuf key
   * @return the protobuf key
   */
  [[nodiscard]] virtual proto::Key* toProtobuf() const = 0;

  /**
   * Verify that a signature was made by the private key which corresponds to this public key
   * @param signatureBytes
   * the byte vector representing the signature
   * @param signedBytes
   * the bytes which were purportedly signed to create the signature
   * @return true if the signature is valid, otherwise false
   */
  [[nodiscard]] virtual bool verifySignature(const std::vector<unsigned char>& signatureBytes,
                                             const std::vector<unsigned char>& signedBytes) const = 0;

  /**
   * Create a new public key from a protobuf key
   * @param key the protobuf key
   * @return the new public key
   */
  static std::shared_ptr<PublicKey> fromProtobuf(const proto::Key& key);

  /**
   * Get the string representation of this key, in DER format
   * @return the DER string
   */
  [[nodiscard]] virtual std::string toString() const = 0;

  /**
   * Get the byte representation (DER format) of the public key
   *
   * @return the byte representation of the key
   */
  [[nodiscard]] virtual std::vector<unsigned char> toBytes() const = 0;
};

} // namespace Hedera

#endif // PUBLIC_KEY_H_
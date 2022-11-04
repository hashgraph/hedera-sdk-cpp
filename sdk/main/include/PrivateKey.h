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
#ifndef PRIVATE_KEY_H_
#define PRIVATE_KEY_H_

#include <memory>
#include <string>
#include <vector>

#include "PublicKey.h"

namespace Hedera
{
/**
 * A generic private key. Contains the public key in addition to the private key
 */
class PrivateKey
{
public:
  /**
   * Get the public key that corresponds to this private key
   * @return the public key
   */
  [[nodiscard]] virtual std::shared_ptr<PublicKey> getPublicKey() const = 0;

  /**
   * Sign an arbitrary byte message. Message is hashed before signing
   * @param bytesToSign the bytes to sign
   * @return the signature
   */
  [[nodiscard]] virtual std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const = 0;

  /**
   * Gets the string representation of the private key, in DER format
   * @return the string representation of the private key
   */
  [[nodiscard]] virtual std::string toString() const = 0;

  /**
   * Default destructor
   */
  virtual ~PrivateKey() = default;
};

} // namespace Hedera

#endif // PRIVATE_KEY_H_

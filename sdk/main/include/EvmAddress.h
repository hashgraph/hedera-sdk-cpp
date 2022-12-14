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
#ifndef EVM_ADDRESS_H_
#define EVM_ADDRESS_H_

#include "PublicKey.h"

#include <memory.h>
#include <string>

namespace Hedera
{
class EvmAddress : public PublicKey
{
public:
  EvmAddress() = default;
  virtual ~EvmAddress() = default;
  EvmAddress(const EvmAddress& other) = default;
  EvmAddress& operator=(const EvmAddress& other) = default;
  EvmAddress& operator=(const EvmAddress&& other) = delete;

  [[nodiscard]] proto::Key* toProtobuf() const override;
  [[nodiscard]] std::string toString() const override;

  bool operator==(const EvmAddress& addr) const
  {
    (void)addr;
    return true;
  }
};

} // namespace Hedera

#endif // EVM_ADDRESS_H_
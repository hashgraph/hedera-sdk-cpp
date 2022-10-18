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

#ifndef HEDERA_SDK_CPP_KEYPAIR_H
#define HEDERA_SDK_CPP_KEYPAIR_H

#include <vector>
#include <memory>

namespace Hedera {
class Keypair {
  [[nodiscard]] virtual std::shared_ptr<PublicKey> getPublicKey() const = 0;
  [[nodiscard]] virtual std::vector<unsigned char> sign(const std::vector<unsigned char>& bytesToSign) const = 0;
};
}

#endif // HEDERA_SDK_CPP_KEYPAIR_H

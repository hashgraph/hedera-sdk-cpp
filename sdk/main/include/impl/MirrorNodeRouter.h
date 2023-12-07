/*-
*
* Hedera C++ SDK
*
* Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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

#ifndef HEDERA_SDK_CPP_MIRRORNODEROUTER_H
#define HEDERA_SDK_CPP_MIRRORNODEROUTER_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Hedera::internal::MirrorNodeGateway
{
class MirrorNodeRouter
{
public:
  [[nodiscard]] static const json getRoutes();
private:
  [[nodiscard]] static const json readRoutesFromFile();
};
}

#endif // HEDERA_SDK_CPP_MIRRORNODEROUTER_H

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
#ifndef HEDERA_SDK_CPP_MIRRORNODEGATEWAY_H
#define HEDERA_SDK_CPP_MIRRORNODEGATEWAY_H

#include "impl/HttpClient.h"

#include <nlohmann/json.hpp>

#include <string_view>

using json = nlohmann::json;

namespace Hedera::internal::MirrorNodeGateway
{
  json AccountInfoQuery(std::string_view accountId);
  json AccountBalanceQuery(std::string_view accountId);
  json ContractInfoQuery(std::string_view contractId);
}
#endif // HEDERA_SDK_CPP_MIRRORNODEGATEWAY_H

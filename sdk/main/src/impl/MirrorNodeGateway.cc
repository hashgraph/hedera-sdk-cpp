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
#include <filesystem>
#include <iostream>

#include "impl/MirrorNodeGateway.h"
#include "impl/Utilities.h"

namespace Hedera::internal::MirrorNodeGateway
{
  json AccountInfoQuery()
  {
    std::cout << std::filesystem::current_path().string() + "/config/mirror_node_routes.json";
    json routes = internal::Utilities::fromConfigFile(std::filesystem::current_path().string() + "/config/mirror_node_routes.json");
  }

  json AccountBalanceQuery()
  {

  }

  json ContractInfoQuery()
  {

  }
}
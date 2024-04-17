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

#include "impl/MirrorNodeRouter.h"
#include "impl/Utilities.h"

#include <filesystem>
#include <iostream>

namespace Hedera::internal::MirrorNodeGateway
{
//-----
std::string MirrorNodeRouter::getRoute(std::string_view queryType) const
{
  std::string queryRoute;
  auto route = routes.find(queryType.data());
  if (route != routes.end())
  {
    queryRoute = route->second;
  }
  else
  {
    throw std::runtime_error("Route not found in the routes map");
  }
  return queryRoute;
}
} // namespace Hedera::internal::MirrorNodeGateway
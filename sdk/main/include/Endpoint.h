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

#ifndef HEDERA_SDK_CPP_ENDPOINT_H
#define HEDERA_SDK_CPP_ENDPOINT_H

#include "IPv4Address.h"

namespace proto
{
 class ServiceEndpoint;
 }

namespace Hedera
{
class Endpoint
{
public:
  static Endpoint fromProtobuf(const proto::ServiceEndpoint& serviceEndpoint);

private:
  Endpoint(IPv4Address address, int port);

  IPv4Address address;
  int port;
};

} // Hedera

#endif // HEDERA_SDK_CPP_ENDPOINT_H

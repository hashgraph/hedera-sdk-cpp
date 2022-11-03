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

#include "Endpoint.h"

#include "proto/basic_types.pb.h"

namespace Hedera
{

Endpoint Endpoint::fromProtobuf(const proto::ServiceEndpoint& serviceEndpoint)
{
  int port = (int) (serviceEndpoint.port() & 0x00000000ffffffffL);

  if (port == 0 || port == 50111) {
    port = 50211;
  }

//  serviceEndpoint.ipaddressv4()
  return Endpoint(IPv4Address::fromString(serviceEndpoint.ipaddressv4()), port);
}

Endpoint::Endpoint(IPv4Address address, int port) {
  this->address = address;
  this->port = port;
}

} // Hedera

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
#include "impl/Endpoint.h"
#include "impl/Utilities.h"

#include <proto/basic_types.pb.h>

namespace Hedera::internal
{
//-----
Endpoint Endpoint::fromProtobuf(const proto::ServiceEndpoint& protoServiceEndpoint)
{
  return Endpoint()
    .setAddress(IPv4Address::fromBytes(Utilities::stringToByteVector(protoServiceEndpoint.ipaddressv4())))
    .setPort(static_cast<unsigned int>(protoServiceEndpoint.port()));
}

//-----
std::unique_ptr<proto::ServiceEndpoint> Endpoint::toProtobuf() const
{
  auto proto = std::make_unique<proto::ServiceEndpoint>();
  proto->set_ipaddressv4(Utilities::byteVectorToString(mAddress.toBytes()));
  proto->set_port(static_cast<int32_t>(mPort));
  return proto;
}

//-----
std::string Endpoint::toString() const
{
  return mAddress.toString() + ':' + std::to_string(mPort);
}

//-----
Endpoint& Endpoint::setAddress(const IPv4Address& address)
{
  mAddress = address;
  return *this;
}

//-----
Endpoint& Endpoint::setPort(unsigned int port)
{
  mPort = port;
  return *this;
}

} // namespace Hedera::internal

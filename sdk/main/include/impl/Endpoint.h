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
#ifndef HEDERA_SDK_CPP_IMPL_ENDPOINT_H_
#define HEDERA_SDK_CPP_IMPL_ENDPOINT_H_

#include "IPv4Address.h"

namespace proto
{
class ServiceEndpoint;
}

namespace Hedera::internal
{
/**
 * A network endpoint, which contains an IPv4 address and a port.
 */
class Endpoint
{
public:
  /**
   * Construct from an address and a port number.
   *
   * @param ipAddressV4 The IPv4 address of the endpoint.
   * @param port        The port of the endpoint.
   */
  Endpoint(const IPv4Address& ipAddressV4, int port);

  /**
   * Create an Endpoint object from a ServiceEndpoint protobuf object.
   *
   * @param proto The ServiceEndpoint protobuf object from which to create an Endpoint object.
   * @return The created Endpoint object.
   */
  static Endpoint fromProtobuf(const proto::ServiceEndpoint& serviceEndpoint);

  /**
   * Get a string representation of the endpoint with the form <ip.add.re.ss>:<port>.
   *
   * @return A string representation of the Endpoint.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Get the IP address of the node.
   *
   * @return The IP address (v4) of the node.
   */
  [[nodiscard]] inline IPv4Address getAddress() const { return mAddress; }

  /**
   * Get the port of the Endpoint.
   *
   * @return The port of the Endpoint.
   */
  [[nodiscard]] inline int getPort() const { return mPort; }

private:
  /**
   * The IP address of the Endpoint.
   */
  IPv4Address mAddress;

  /**
   * The port of the Endpoint.
   */
  int mPort;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_ENDPOINT_H_

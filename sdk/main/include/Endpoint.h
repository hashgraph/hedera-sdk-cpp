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

/**
 * A network endpoint, which contains an IPv4 address and a port
 */
class Endpoint
{
public:
  /**
   * Constructor
   *
   * @param address the IPv4 address of the endpoint
   * @param port the port of the endpoint
   */
  Endpoint(IPv4Address address, int port);

  /**
   * Creates a new endpoint object from a protobuf endpoint
   *
   * @param serviceEndpoint the protobuf endpoint
   *
   * @return the new Endpoint object
   */
  static Endpoint fromProtobuf(const proto::ServiceEndpoint& serviceEndpoint);

  /**
   * Gets a string representation of the endpoint, of form <ip.add.re.ss>:<port>
   *
   * @return the string representation of the endpoint
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Gets the port of the endpoint
   *
   * @return the port
   */
  [[nodiscard]] int getPort() const;

private:
  /**
   * The IP address of the endpoint
   */
  IPv4Address mAddress{};

  /**
   * The port of the endpoint
   */
  int mPort;
};

} // Hedera

#endif // HEDERA_SDK_CPP_ENDPOINT_H

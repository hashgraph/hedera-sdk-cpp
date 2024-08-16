/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_ENDPOINT_H_
#define HEDERA_SDK_CPP_ENDPOINT_H_

#include "IPv4Address.h"

#include <memory>
#include <string>

namespace proto
{
class ServiceEndpoint;
}

namespace Hedera
{
/**
 * A network endpoint, which contains an IPv4 address or domain name and a port.
 */
class Endpoint
{
public:
  /**
   * Construct an Endpoint object from a ServiceEndpoint protobuf object.
   *
   * @param proto The ServiceEndpoint protobuf object from which to construct an Endpoint object.
   * @return The constructed Endpoint object.
   */
  [[nodiscard]] static Endpoint fromProtobuf(const proto::ServiceEndpoint& protoServiceEndpoint);

  /**
   * Construct a ServiceEndpoint protobuf object from this Endpoint object.
   *
   * @return A pointer to the created ServiceEndpoint protobuf object filled with this Endpoint object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::ServiceEndpoint> toProtobuf() const;

  /**
   * Get a string representation of this Endpoint.
   *
   * @return A string representation of this Endpoint.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Set the IP address of this Endpoint.
   *
   * @param address The IP address to set.
   * @return A reference to this Endpoint with the newly-set IP address.
   */
  Endpoint& setAddress(const IPv4Address& address);

  /**
   * Set the port of this Endpoint.
   *
   * @param port The port to set.
   * @return A reference to this Endpoint with the newly-set port.
   */
  Endpoint& setPort(unsigned int port);

  /**
   * Set the domain name of this Endpoint.
   *
   * @param domainName The domain name to set.
   * @return A reference to this Endpoint with the newly-set domain name.
   */
  Endpoint& setDomainName(const std::string& domainName);

  /**
   * Get the IP address of this Endpoint.
   *
   * @return The IP address of this Endpoint.
   */
  [[nodiscard]] inline IPv4Address getAddress() const { return mAddress; }

  /**
   * Get the port of this Endpoint.
   *
   * @return The port of this Endpoint.
   */
  [[nodiscard]] inline unsigned int getPort() const { return mPort; }

  /**
   * Get the domain name of this Endpoint.
   *
   * @return The domain name of this Endpoint.
   */
  [[nodiscard]] inline const std::string& getDomainName() const { return mDomainName; }

private:
  /**
   * The IP address of the Endpoint.
   */
  IPv4Address mAddress;

  /**
   * The port of the Endpoint.
   */
  unsigned int mPort = 0U;

  /**
   * A node domain name.
   * This MUST be the fully qualified domain(DNS) name of the node.
   * This value MUST NOT be more than 253 characters.
   * domain_name and ipAddressV4 are mutually exclusive.
   * When the `domain_name` field is set, the `ipAddressV4` field MUST NOT be set.
   * When the `ipAddressV4` field is set, the `domain_name` field MUST NOT be set.
   */
  std::string mDomainName;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ENDPOINT_H_

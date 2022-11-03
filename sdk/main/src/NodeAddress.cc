/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "NodeAddress.h"

#include "proto/basic_types.pb.h"

#include <iomanip>
#include <sstream>

namespace Hedera
{
NodeAddress::NodeAddress()
{
  mEndpoints = {};
}

//-----
NodeAddress NodeAddress::fromString(std::string_view address)
{
  NodeAddress nodeAddress;

  // TODO: add string checks
  size_t colonIndex = address.find(':');
  nodeAddress.mIPAddress = address.substr(0, address.size() - colonIndex + 1);
  nodeAddress.mPort =
    atoi(static_cast<std::string>(address.substr(colonIndex + 1, address.size() - nodeAddress.mIPAddress.size() - 1))
           .c_str());

  return nodeAddress;
}

NodeAddress NodeAddress::fromProtobuf(const proto::NodeAddress& protoNodeAddress)
{
  NodeAddress outputNodeAddress = NodeAddress();

  for (int i = 0; i < protoNodeAddress.serviceendpoint_size(); ++i)
  {
    outputNodeAddress.mEndpoints.push_back(Endpoint::fromProtobuf(protoNodeAddress.serviceendpoint(i)));
  }

  if (protoNodeAddress.ipaddress().length() != 0)
  {
    outputNodeAddress.mEndpoints.emplace_back(IPv4Address::fromString(protoNodeAddress.ipaddress()),
                                              protoNodeAddress.portno());
  }

  outputNodeAddress.mRSAPublicKey = protoNodeAddress.rsa_pubkey();
  outputNodeAddress.mNodeId = protoNodeAddress.nodeid();
  outputNodeAddress.mCertificateHash = protoNodeAddress.nodecerthash();
  outputNodeAddress.mDescription = protoNodeAddress.description();

  return outputNodeAddress;
}

std::string NodeAddress::toString() const
{
  std::stringstream outputStream;

  outputStream << std::setw(20) << std::right << "NodeId: " << std::left << mNodeId << std::endl;
  outputStream << std::setw(20) << std::right << "IP Address: " << std::left << mIPAddress << std::endl;
  outputStream << std::setw(20) << std::right << "Description: " << std::left << mDescription << std::endl;
  outputStream << std::setw(20) << std::right << "RSA Public Key: " << std::left << mRSAPublicKey << std::endl;
  outputStream << std::setw(20) << std::right << "Certificate Hash: " << std::left << mCertificateHash << std::endl;
  outputStream << std::setw(20) << std::right << "Endpoints: " << std::endl;

  for (auto endpoint : mEndpoints)
  {
    outputStream << endpoint.toString() << std::endl;
  }

  return outputStream.str();
}

} // namespace Hedera

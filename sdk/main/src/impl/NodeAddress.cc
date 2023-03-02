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
#include "impl/NodeAddress.h"

#include <iomanip>
#include <memory>
#include <proto/basic_types.pb.h>
#include <sstream>

namespace Hedera::internal
{
//-----
NodeAddress NodeAddress::fromProtobuf(const proto::NodeAddress& protoNodeAddress)
{
  NodeAddress outputNodeAddress;

  for (int i = 0; i < protoNodeAddress.serviceendpoint_size(); ++i)
  {
    outputNodeAddress.mEndpoints.push_back(internal::Endpoint::fromProtobuf(protoNodeAddress.serviceendpoint(i)));
  }

  if (protoNodeAddress.ipaddress().length() != 0)
  {
    outputNodeAddress.mEndpoints.emplace_back(internal::IPv4Address::fromString(protoNodeAddress.ipaddress()),
                                              protoNodeAddress.portno());
  }

  outputNodeAddress.mRSAPublicKey = protoNodeAddress.rsa_pubkey();
  outputNodeAddress.mNodeId = protoNodeAddress.nodeid();
  outputNodeAddress.mNodeAccountId = AccountId::fromProtobuf(protoNodeAddress.nodeaccountid());
  outputNodeAddress.mNodeCertHash = protoNodeAddress.nodecerthash();
  outputNodeAddress.mDescription = protoNodeAddress.description();
  outputNodeAddress.mStake = protoNodeAddress.stake();

  return outputNodeAddress;
}

//-----
static NodeAddress fromString(const std::string addressString)
{
  NodeAddress outputNodeAddress;
  return outputNodeAddress;
}

//-----
NodeAddress& NodeAddress::setNodeId(const int64_t nodeId)
{
  mNodeId = nodeId;
  return *this;
}

//-----
NodeAddress& NodeAddress::setNodeAccountId(const AccountId& accountId)
{
  mNodeAccountId = accountId;
  return *this;
}

//-----
NodeAddress& NodeAddress::setNodeCertHash(const std::string& certHash)
{
  mNodeCertHash = certHash;
  return *this;
}

//-----
std::string NodeAddress::toString() const
{
  std::stringstream outputStream;

  int columnWidth = 20;
  outputStream << std::setw(columnWidth) << std::right << "NodeId: " << std::left << mNodeId << std::endl;
  outputStream << std::setw(columnWidth) << std::right << "AccountId: " << std::left << mNodeAccountId.toString()
               << std::endl;
  outputStream << std::setw(columnWidth) << std::right << "Description: " << std::left << mDescription << std::endl;
  outputStream << std::setw(columnWidth) << std::right << "RSA Public Key: " << std::left << mRSAPublicKey << std::endl;
  outputStream << std::setw(columnWidth) << std::right << "Certificate Hash: " << std::left << mNodeCertHash
               << std::endl;
  outputStream << std::setw(columnWidth) << std::right << "Stake: " << std::left << mStake << std::endl;
  outputStream << std::setw(columnWidth) << std::right << "Endpoints: ";

  if (size_t endpointCount = mEndpoints.size(); !endpointCount)
  {
    outputStream << "<None>";
  }
  else
  {
    int counter = 0;
    for (auto endpoint : mEndpoints)
    {
      if (counter == 0)
      {
        outputStream << endpoint.toString();
      }
      else
      {
        outputStream << std::setw(columnWidth) << "" << endpoint.toString();
      }

      ++counter;

      if (counter != endpointCount)
      {
        outputStream << std::endl;
      }
    }
  }

  return outputStream.str();
}

} // namespace Hedera::internal

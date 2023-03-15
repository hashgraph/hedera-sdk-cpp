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
#include "exceptions/IllegalStateException.h"
#include "impl/HexConverter.h"
#include "impl/IPv4Address.h"

#include <iomanip>
#include <memory>
#include <proto/basic_types.pb.h>
#include <sstream>

namespace Hedera::internal
{
//-----
NodeAddress::NodeAddress(std::string_view ipAddressV4, int port)
{
  std::vector<unsigned char> octets;
  std::stringstream strStream({ ipAddressV4.begin(), ipAddressV4.end() });
  std::string temp;

  try
  {
    while (getline(strStream, temp, '.'))
    {
      int octet = std::stoi(temp);

      if (octet > 0)
      {
        octets.push_back(static_cast<unsigned char>(octet));
      }
    }
  }
  catch (const std::exception&)
  {
    throw IllegalStateException("Failed to parse the IP address.");
  }

  if (octets.size() != 4)
  {
    throw IllegalStateException("The IP address is missing or has wrong format.");
  }

  mEndpoints.push_back(std::make_shared<Endpoint>(IPv4Address(octets[0], octets[1], octets[2], octets[3]), port));
}

//-----
NodeAddress NodeAddress::fromProtobuf(const proto::NodeAddress& protoNodeAddress)
{
  NodeAddress outputNodeAddress;

  for (int i = 0; i < protoNodeAddress.serviceendpoint_size(); ++i)
  {
    Endpoint endpoint = Endpoint::fromProtobuf(protoNodeAddress.serviceendpoint(i));
    outputNodeAddress.mEndpoints.push_back(std::make_shared<Endpoint>(endpoint));
  }

  if (protoNodeAddress.ipaddress().length() != 0)
  {
    outputNodeAddress.mEndpoints.emplace_back(
      std::make_shared<Endpoint>(IPv4Address::fromString(protoNodeAddress.ipaddress()), protoNodeAddress.portno()));
  }

  outputNodeAddress.mRSAPublicKey = protoNodeAddress.rsa_pubkey();
  outputNodeAddress.mNodeId = protoNodeAddress.nodeid();
  outputNodeAddress.mNodeAccountId = AccountId::fromProtobuf(protoNodeAddress.nodeaccountid());
  outputNodeAddress.mNodeCertHash = { protoNodeAddress.nodecerthash().cbegin(),
                                      protoNodeAddress.nodecerthash().cend() };
  outputNodeAddress.mDescription = protoNodeAddress.description();
  outputNodeAddress.mStake = protoNodeAddress.stake();

  return outputNodeAddress;
}

//-----
NodeAddress NodeAddress::fromString(std::string_view nodeAddress)
{
  std::vector<std::string> parts;
  std::stringstream strStream({ nodeAddress.begin(), nodeAddress.end() });
  std::string ipAddressV4;
  int port;

  try
  {
    std::string temp;

    while (getline(strStream, temp, ':'))
    {
      parts.emplace_back(temp.c_str());
    }

    ipAddressV4 = parts[0];
    port = std::stoi(parts[1]);
  }
  catch (const std::exception&)
  {
    throw IllegalStateException("Failed to parse the node address.");
  }

  return { ipAddressV4, port };
}

//-----
NodeAddress& NodeAddress::setRSAPublicKey(std::string_view publicKey)
{
  mRSAPublicKey = publicKey;
  return *this;
}

//-----
NodeAddress& NodeAddress::setNodeId(const int64_t& nodeId)
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
NodeAddress& NodeAddress::setNodeCertHash(std::string_view certHash)
{
  mNodeCertHash = { certHash.cbegin(), certHash.cend() };
  return *this;
}

//-----
NodeAddress& NodeAddress::setEndpoints(const std::vector<std::shared_ptr<Endpoint>>& endpoints)
{
  mEndpoints = endpoints;
  return *this;
}

//-----
NodeAddress& NodeAddress::setDescription(std::string_view description)
{
  mDescription = description;
  return *this;
}

//-----
NodeAddress& NodeAddress::setStake(const uint64_t& stake)
{
  mStake = stake;
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
  outputStream << std::setw(columnWidth) << std::right << "Certificate Hash: " << std::left
               << HexConverter::bytesToHex(mNodeCertHash) << std::endl;
  outputStream << std::setw(columnWidth) << std::right << "Stake: " << std::left << mStake << std::endl;
  outputStream << std::setw(columnWidth) << std::right << "Endpoints: ";

  if (size_t endpointCount = mEndpoints.size(); !endpointCount)
  {
    outputStream << "<None>";
  }
  else
  {
    int counter = 0;
    for (const auto& endpoint : mEndpoints)
    {
      if (counter == 0)
      {
        outputStream << endpoint->toString();
      }
      else
      {
        outputStream << std::setw(columnWidth) << endpoint->toString();
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

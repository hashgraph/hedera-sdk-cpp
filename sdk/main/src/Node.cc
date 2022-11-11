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
#include "Node.h"

#include <proto/crypto_service.grpc.pb.h>

namespace Hedera
{
//-----
Node::Node(const std::shared_ptr<NodeAddress>& address)
{
  mAddress = address;
}

//-----
std::function<grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
Node::getGrpcTransactionMethod(int transactionBodyDataCase)
{
  if (!checkChannelInitialized())
  {
    return nullptr;
  }

  return mChannel.getGrpcTransactionMethod(transactionBodyDataCase);
}

//-----
std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> Node::getGrpcQueryMethod(
  int queryBodyDataCase)
{
  if (!checkChannelInitialized())
  {
    return nullptr;
  }

  return mChannel.getGrpcQueryMethod(queryBodyDataCase);
}

//-----
void Node::shutdown()
{
  mChannel.shutdown();
}

bool Node::checkChannelInitialized()
{
  return mChannel.getInitialized() || tryInitializeChannel();
}

bool Node::tryInitializeChannel()
{
  const std::vector<Endpoint> endpoints = mAddress->getEndpoints();

  return std::any_of(endpoints.begin(),
                     endpoints.end(),
                     [this](const Endpoint& endpoint)
                     {
                       switch (mTLSBehavior)
                       {
                         case TLSBehavior::REQUIRE:
                           return (endpoint.getPort() == NodeAddress::PORT_NODE_TLS ||
                                   endpoint.getPort() == NodeAddress::PORT_MIRROR_TLS) &&
                                  !mAddress->getCertificateHash().empty() &&
                                  mChannel.initializeEncryptedChannel(endpoint.toString(),
                                                                      mAddress->getCertificateHash());

                         case TLSBehavior::DISABLE:
                           return (endpoint.getPort() == NodeAddress::PORT_NODE_PLAIN ||
                                   endpoint.getPort() == NodeAddress::PORT_MIRROR_PLAIN) &&
                                  mChannel.initializeUnencryptedChannel(endpoint.toString());
                         default:
                           return false;
                       }
                     });
}

void Node::setTLSBehavior(TLSBehavior desiredBehavior)
{
  if (desiredBehavior == mTLSBehavior)
  {
    return;
  }

  mTLSBehavior = desiredBehavior;
  mChannel.shutdown();
}

} // namespace Hedera

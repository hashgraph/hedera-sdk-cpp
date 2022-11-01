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
Node::Node(const std::string& url, const AccountId& accountId)
  : mAccountId(accountId)
  , mAddress(NodeAddress::fromString(url))
  , mChannel(url)
{
}

//-----
std::function<grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
Node::getGrpcTransactionMethod(int transactionBodyDataCase) const
{
  std::cout << __FUNCTION__ << std::endl;
  return mChannel.getGrpcTransactionMethod(transactionBodyDataCase);
}

//-----
std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> Node::getGrpcQueryMethod(
  int queryBodyDataCase) const
{
  return mChannel.getGrpcQueryMethod(queryBodyDataCase);
}

//-----
void Node::shutdown()
{
  mChannel.shutdown();
}

//-----
/*template<typename ProtoRequestType, typename ProtoResponseType>
std::pair<ProtoResponseType, grpc::Status> submitRequest(int funcCase,
                                                         grpc::ClientContext* context,
                                                         const ProtoRequestType& request)
{
}

template std::pair<proto::Response, grpc::Status> Node::submitRequest(int funcCase,
                                                                      grpc::ClientContext* context,
                                                                      const proto::Query& request);
template std::pair<proto::TransactionResponse, grpc::Status> Node::submitRequest(int funcCase,
                                                                                 grpc::ClientContext* context,
                                                                                 const proto::Transaction& request);*/

} // namespace Hedera

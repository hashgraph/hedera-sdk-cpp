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
#ifndef NODE_H_
#define NODE_H_

#include "AccountId.h"
#include "Channel.h"
#include "NodeAddress.h"

#include <string>
#include <utility>

namespace grpc
{
class ClientContext;
class Status;
}

namespace proto
{
class Query;
class Response;
class Transaction;
class TransactionResponse;
}

namespace Hedera
{
class Node
{
public:
  /**
   * Construct this node with the URL and the account ID.
   *
   * @param url       The URL of the node.
   * @param accountId The account ID of the node.
   */
  explicit Node(const std::string& url, const AccountId& accountId);

  /**
   * Shutdown connections with the node.
   */
  void shutdown();

  /**
   * Extract the account ID.
   *
   * @return The account ID.
   */
  inline AccountId getAccountId() const { return mAccountId; }

  /**
   * Get a gRPC transaction method for an associated protobuf Transaction data case from this Node's channel.
   *
   * @param transactionBodyDataCase The case describing the function to get.
   * @return The function described by the case, bound to this channel's proper stub.
   */
  std::function<grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
  getGrpcTransactionMethod(int transactionBodyDataCase) const;

  /**
   * Get a gRPC query method for an associated protobuf Query data case from this Node's channel.
   *
   * @param queryBodyDataCase The case describing the function to get.
   * @return The function described by the case, bound to this channel's proper stub.
   */
  std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> getGrpcQueryMethod(
    int queryBodyDataCase) const;

private:
  /**
   * The account ID associated with this node.
   */
  AccountId mAccountId;

  /**
   * The address of this node.
   */
  NodeAddress mAddress;

  /**
   * The channels this node is using to communicate.
   */
  Channel mChannel;
};

} // namespace Hedera

#endif // NODE_H_
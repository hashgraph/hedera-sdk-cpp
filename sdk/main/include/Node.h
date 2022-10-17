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

namespace grpc
{
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
   * Submit a query request to this node.
   *
   * @param query   The query to send.
   * @param timeout The timeout duration.
   * @return The protobuf response object and the gRPC status.
   */
  std::pair<proto::Response, grpc::Status> submitRequest(const proto::Query& request,
                                                         const std::chrono::duration<double>& timeout);

  /**
   * Submit a transaction request to this node.
   *
   * @param transaction The transaction to send.
   * @param timeout     The timeout duration.
   * @return The protobuf transaction response object and the gRPC status.
   */
  std::pair<proto::TransactionResponse, grpc::Status> submitRequest(const proto::Transaction& request,
                                                                    const std::chrono::duration<double>& timeout);

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
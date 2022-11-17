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
#ifndef HEDERA_SDK_CPP_IMPL_NODE_H_
#define HEDERA_SDK_CPP_IMPL_NODE_H_

#include "AccountId.h"
#include "NodeAddress.h"
#include "TLSBehavior.h"
#include "impl/Channel.h"

#include <memory>
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

namespace Hedera::internal
{
class Node
{
public:
  /**
   * Construct this node with the URL and the account ID.
   *
   * @param address The address of the node
   */
  explicit Node(const std::shared_ptr<NodeAddress>& address);

  /**
   * Shutdown connections with the node.
   */
  void shutdown();

  /**
   * Extract the account ID.
   *
   * @return The account ID.
   */
  [[nodiscard]] inline std::shared_ptr<AccountId> getAccountId() const { return mAddress->getAccountId(); }

  /**
   * Sets the TLS behavior of the node
   *
   * @param desiredBehavior the desired behavior
   */
  void setTLSBehavior(TLSBehavior desiredBehavior);

  /**
   * Get a gRPC transaction method for an associated protobuf Transaction data case from this Node's channel.
   *
   * @param transactionBodyDataCase The case describing the function to get.
   * @return The function described by the case, bound to this channel's proper stub.
   */
  std::function<grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
  getGrpcTransactionMethod(int transactionBodyDataCase);

  /**
   * Get a gRPC query method for an associated protobuf Query data case from this Node's channel.
   *
   * @param queryBodyDataCase The case describing the function to get.
   * @return The function described by the case, bound to this channel's proper stub.
   */
  std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> getGrpcQueryMethod(
    int queryBodyDataCase);

private:
  /**
   * The address of this node.
   */
  std::shared_ptr<NodeAddress> mAddress;

  /**
   * The channels this node is using to communicate.
   */
  Channel mChannel;

  /**
   * The TLS behavior of the node
   */
  TLSBehavior mTLSBehavior = TLSBehavior::REQUIRE;

  /**
   * Checks if the channel is initialized. If it isn't, try to initialize it
   *
   * @return true if the channel is already initialized, or if it is successfully initialized in the course of this
   * call. false if channel could not be initialized
   */
  bool checkChannelInitialized();

  /**
   * Tries to initialize the channel
   *
   * @return true if initialization is successful, otherwise false
   */
  bool tryInitializeChannel();
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_NODE_H_

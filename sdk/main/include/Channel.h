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
#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <chrono>
#include <memory>
#include <string>

namespace grpc
{
class ClientContext;
class Status;
}

namespace proto
{
class CryptoService;
class Query;
class Response;
class Transaction;
class TransactionResponse;
}

namespace Hedera
{
class Channel
{
public:
  /**
   * Default constructor.
   */
  Channel();

  /**
   * Construct this channel to communicate with a node URL.
   *
   * @param url The URL and port of the gRPC service with which this channel
   *            should communicate.
   */
  explicit Channel(const std::string& url);

  /**
   * Default destructor.
   */
  ~Channel();

  /**
   * Initialize this channel to communicate with a node URL.
   *
   * @param url The URL and port of the gRPC service with which this channel
   *            should communicate.
   */
  void initChannel(const std::string& url);

  /**
   * Get a gRPC transaction method for an associated protobuf Transaction data case.
   *
   * @param transactionBodyDataCase The case describing the function to get.
   * @return The function described by the case, bound to this channel's proper stub.
   */
  std::function<grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
  getGrpcTransactionMethod(int transactionBodyDataCase) const;

  /**
   * Get a gRPC query method for an associated protobuf Query data case.
   *
   * @param queryBodyDataCase The case describing the function to get.
   * @return The function described by the case, bound to this channel's proper stub.
   */
  std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> getGrpcQueryMethod(
    int queryBodyDataCase) const;

  /**
   * Shutdown the channel.
   */
  void shutdown();

private:
  /**
   * Helper function to close/shutdown/delete channels.
   */
  void shutdownChannel();

  /**
   * Implementation object used to hide implementation details and gRPC headers.
   */
  struct ChannelImpl;
  std::unique_ptr<ChannelImpl> mImpl;
};

} // namespace Hedera

#endif // CHANNEL_H_

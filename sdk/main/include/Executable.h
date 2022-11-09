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
#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include "AccountId.h"

#include <chrono>
#include <vector>
#include <memory>

namespace Hedera
{
class Client;
class Node;
}

namespace grpc
{
class ClientContext;
class Status;
}

namespace Hedera
{
/**
 * Base class for queries and transactions that contains all the behavior for executing and submitting requests to a
 * Hedera network.
 *
 * @tparam SdkRequestType    The SDK request type.
 * @tparam ProtoRequestType  The protobuf request message type.
 * @tparam ProtoResponseType The protobuf response message type.
 * @tparam SdkResponseType   The SDK response type.
 */
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
class Executable
{
public:
  /**
   * Execute this executable.
   *
   * @param client The client with which this will be executed.
   * @return The result of execution.
   * @throws std::runtime_error If unable to communicate with client network or if operator is needed and not set.
   */
  SdkResponseType execute(const Client& client);

  /**
   * Execute this executable with a specified timeout.
   *
   * @param client  The client with which this will be executed.
   * @param timeout The timeout for this execution.
   * @return The result of execution.
   * @throws std::runtime_error If unable to communicate with client network or if operator is needed and not set.
   */
  SdkResponseType execute(const Client& client, const std::chrono::duration<double>& duration);

  /**
   * Set the account IDs of the nodes to which this transaction will be submitted.
   *
   * @param nodeAccountIds The list of node account IDs to be set.
   * @return Reference to this Executable derived class.
   */
  SdkRequestType& setNodeAccountIds(const std::vector<std::shared_ptr<AccountId>>& nodeAccountIds);

  /**
   * Get the list of account IDs for nodes with which execution will be attempted.
   * @return The list of account IDs.
   */
  inline std::vector<std::shared_ptr<AccountId>> getNodeAccountIds() const { return mNodeAccountIds; }

protected:
  /**
   * Default destructor.
   */
  virtual ~Executable() = default;

  /**
   * Construct a ProtoRequestType from this request.
   *
   * @param client The Client submitting this request.
   * @param node   The Node on which this request is being submitted.
   * @return A ProtoRequestType that contains this request's data.
   */
  virtual ProtoRequestType makeRequest([[maybe_unused]] const Client& client,
                                       [[maybe_unused]] const std::shared_ptr<Node>& node) const = 0;

  /**
   * Construct the response from a protobuf response object.
   *
   * @param response The protobuf response object.
   * @return The response object with the response data.
   */
  virtual SdkResponseType mapResponse(const ProtoResponseType& response) const = 0;

  /**
   * Get the gRPC method to call for this Executable.
   *
   * @param node The Node from which to retrieve the function.
   * @return The gRPC method to call to execute this request.
   */
  virtual std::function<grpc::Status(grpc::ClientContext*, const ProtoRequestType&, ProtoResponseType*)> getGrpcMethod(
    const std::shared_ptr<Node>& node) const = 0;

  /**
   * Perform any needed actions for this Executable when it is being executed.
   *
   * @param client The Client executing this Executable.
   */
  virtual void onExecute([[maybe_unused]] const Client& client) = 0;

  /**
   * Perform any needed actions for this Executable when a Node has been selected to which to send this Executable.
   *
   * @param node The Node to which this Executable is being sent.
   */
  virtual void onSelectNode([[maybe_unused]] const std::shared_ptr<Node>& node) = 0;

private:
  /**
   * The list of account IDs of nodes on which to attempt execution.
   */
  std::vector<std::shared_ptr<AccountId>> mNodeAccountIds;

  /**
   * The maximum number of attempts to send a request.
   */
  const int mMaxNumOfAttempts = 10;
};

} // namespace Hedera

#endif // EXECUTABLE_H

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
#ifndef HEDERA_SDK_CPP_EXECUTABLE_H_
#define HEDERA_SDK_CPP_EXECUTABLE_H_

#include "AccountId.h"

#include <chrono>
#include <functional>
#include <memory>
#include <vector>
#include <functional>

namespace Hedera
{
namespace internal
{
class Node;
}
class Client;
}

namespace grpc
{
class ClientContext;
class Status;
}

namespace Hedera
{
/**
 * Base class for queries and transactions that contains the behavior for executing and submitting requests.
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
  virtual ~Executable() = default;

  /**
   * Create a clone of this Executable object.
   *
   * @return A pointer to the created clone of this Executable.
   */
  virtual std::unique_ptr<Executable> clone() const = 0;

  /**
   * Submit this Executable to a Hedera network.
   *
   * @param client The Client to use to submit this Executable.
   * @return The SdkResponseType object sent from the Hedera network that contains the result of the request.
   * @throws std::runtime_error If unable to communicate with client network or if operator is needed and not set.
   */
  SdkResponseType execute(const Client& client);

  /**
   * Submit this Executable to a Hedera network with a specific timeout.
   *
   * @param client The Client to use to submit this Executable.
   * @param timeout The desired timeout for the execution of this Executable.
   * @return The SdkResponseType object sent from the Hedera network that contains the result of the request.
   * @throws std::runtime_error If unable to communicate with client network or if operator is needed and not set.
   */
  SdkResponseType execute(const Client& client, const std::chrono::duration<int64_t>& duration);

  /**
   * Set the desired account IDs of nodes to which this transaction will be submitted.
   *
   * @param nodeAccountIds The desired list of account IDs of nodes to submit this request.
   * @return A reference to this Executable derived class with the newly-set node account IDs.
   */
  SdkRequestType& setNodeAccountIds(const std::vector<std::shared_ptr<AccountId>>& nodeAccountIds);

  /**
   * Get the list of account IDs for nodes with which execution will be attempted.
   *
   * @return The list of account IDs of nodes this Executable would attempt request submission.
   */
  [[nodiscard]] inline std::vector<std::shared_ptr<AccountId>> getNodeAccountIds() const { return mNodeAccountIds; }

protected:
  /**
   * Prevent public copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  Executable() = default;
  Executable(const Executable&) = default;
  Executable& operator=(const Executable&) = default;
  Executable(Executable&&) noexcept = default;
  Executable& operator=(Executable&&) noexcept = default;

  /**
   * Construct a ProtoRequestType object from this Executable object.
   *
   * @param client The Client being used to submit this Executable.
   * @param node   The Node on which this Executable will be sent.
   * @return A ProtoRequestType object filled with this Executable object's data.
   */
  virtual ProtoRequestType makeRequest([[maybe_unused]] const Client& client,
                                       [[maybe_unused]] const std::shared_ptr<internal::Node>& node) const = 0;

  /**
   * Construct an SdkResponseType from a ProtoResponseType object.
   *
   * @param response The ProtoResponseType object from which to construct an SdkResponseType object.
   * @return An SdkResponseType object filled with the ProtoResponseType object's data.
   */
  virtual SdkResponseType mapResponse(const ProtoResponseType& response) const = 0;

  /**
   * Get the gRPC method to call to send this Executable.
   *
   * @param node The Node to which this Executable is being sent and from which the gRPC method should be retrieved.
   * @return The gRPC method to call to submit this Executable.
   */
  virtual std::function<grpc::Status(grpc::ClientContext*, const ProtoRequestType&, ProtoResponseType*)> getGrpcMethod(
    const std::shared_ptr<internal::Node>& node) const = 0;

  /**
   * Perform any needed actions for this Executable when it is being submitted.
   *
   * @param client The Client being used to submit this Executable.
   */
  virtual void onExecute([[maybe_unused]] const Client& client) = 0;

  /**
   * Perform any needed actions for this Executable when a Node has been selected to which to submit this Executable.
   *
   * @param node The Node to which this Executable is being submitted.
   */
  virtual void onSelectNode([[maybe_unused]] const std::shared_ptr<internal::Node>& node) = 0;

private:
  /**
   * The list of account IDs of the nodes with which execution should be attempted.
   */
  std::vector<std::shared_ptr<AccountId>> mNodeAccountIds;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_EXECUTABLE_H_

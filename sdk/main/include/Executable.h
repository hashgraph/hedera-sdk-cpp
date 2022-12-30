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
#include "Defaults.h"

#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace Hedera
{
namespace internal
{
class Node;
}
class Client;
enum class Status;
}

namespace grpc
{
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
   * @throws std::invalid_argument If the there was a network issue processing this Executable.
   * @throws std::runtime_error If unable to communicate with client network or if operator is needed and not set.
   */
  SdkResponseType execute(const Client& client);

  /**
   * Submit this Executable to a Hedera network with a specific timeout.
   *
   * @param client  The Client to use to submit this Executable.
   * @param timeout The desired timeout for the execution of this Executable.
   * @return The SdkResponseType object sent from the Hedera network that contains the result of the request.
   * @throws std::invalid_argument If the there was a network issue processing this Executable.
   * @throws std::runtime_error If unable to communicate with client network or if operator is needed and not set.
   */
  SdkResponseType execute(const Client& client, const std::chrono::duration<double>& timeout);

  /**
   * Set the desired account IDs of nodes to which this transaction will be submitted.
   *
   * @param nodeAccountIds The desired list of account IDs of nodes to submit this request.
   * @return A reference to this Executable derived class with the newly-set node account IDs.
   */
  SdkRequestType& setNodeAccountIds(const std::vector<AccountId>& nodeAccountIds);

  /**
   * Set the maximum number of times this Executable should try to resubmit itself after a failed attempt before it
   * considers itself a failure. This will override the maximum number of attempts of the Client used to submit this
   * Executable.
   *
   * @param attempts The desired maximum number of execution attempts.
   * @return A reference to this Executable derived class with the newly-set maximum number of execution attempts.
   */
  SdkRequestType& setMaxAttempts(uint32_t attempts);

  /**
   * Set the minimum amount of time a Node should wait after this Executable failed to execute before being willing to
   * submit this Executable again. This will override the minimum backoff time of the Client used to submit this
   * Executable.
   *
   * @param backoff The desired minimum amount of time this Executable should wait between retries.
   * @return A reference to this Executable derived class with the newly-set minimum backoff time.
   * @throws std::invalid_argument If the desired minimum backoff duration is longer than the set maximum backoff time
   *                               (DEFAULT_MAX_BACKOFF if the maximum backoff time has not been set).
   */
  SdkRequestType& setMinBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Set the maximum amount of time a Node should wait after this Executable failed to execute before being willing to
   * submit this Executable again. This will override the maximum backoff time of the Client used to submit this
   * Executable.
   *
   * @param backoff The desired maximum amount of time this Executable should wait between retries.
   * @return A reference to this Executable derived class with the newly-set maximum backoff time.
   */
  SdkRequestType& setMaxBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Get the list of account IDs for nodes with which execution will be attempted.
   *
   * @return The list of account IDs of nodes this Executable would attempt request submission.
   */
  [[nodiscard]] inline std::vector<AccountId> getNodeAccountIds() const { return mNodeAccountIds; }

  /**
   * Get the maximum number of times this Executable should try to resubmit itself after a failed attempt before it
   * considers itself a failure.
   *
   * @return The maximum number of execution attempts. Uninitialized value if not previously set.
   */
  [[nodiscard]] inline std::optional<uint32_t> getMaxAttempts() const { return mMaxAttempts; }

  /**
   * Get the minimum amount of time a Node should wait after this Executable failed to execute before being willing to
   * submit this Executable again.
   *
   * @return The minimum backoff time for Nodes for execution attempts of this Executable. Uninitialized value if not
   * previously set.
   */
  [[nodiscard]] inline std::optional<std::chrono::duration<double>> getMinBackoff() const { return mMinBackoff; }

  /**
   * Get the maximum amount of time a Node should wait after this Executable failed to execute before being willing to
   * submit this Executable again.
   *
   * @return The maximum backoff time for Nodes for execution attempts of this Executable. Uninitialized value if not
   * previously set.
   */
  [[nodiscard]] inline std::optional<std::chrono::duration<double>> getMaxBackoff() const { return mMaxBackoff; }

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
   * Enumeration describing the status of a submitted Executable.
   */
  enum class ExecutionStatus
  {
    /**
     * The request was successfully executed.
     */
    SUCCESS,
    /**
     * There was an error on the server side that shouldn't be retried.
     */
    SERVER_ERROR,
    /**
     * There was a user error with the request.
     */
    REQUEST_ERROR,
    /**
     * The call was successful but an operation did not complete.
     */
    RETRY,
    /**
     * The status of the request could not be determined, requires further processing.
     */
    UNKNOWN
  };

  /**
   * Perform any needed actions for this Executable when a Node has been selected to which to submit this Executable.
   *
   * @param node The Node to which this Executable is being submitted.
   */
  virtual void onSelectNode([[maybe_unused]] const std::shared_ptr<internal::Node>& node);

  /**
   * Determine the ExecutionStatus of this Executable after being submitted.
   *
   * @param status   The response status from the network.
   * @param client   The Client that submitted this Executable.
   * @param response The ProtoResponseType received from the network in response to submitting this Executable.
   * @return The status of the submitted Executable.
   */
  [[nodiscard]] virtual ExecutionStatus determineStatus(Status status,
                                                        [[maybe_unused]] const Client& client,
                                                        [[maybe_unused]] const ProtoResponseType& response);

private:
  /**
   * Construct a ProtoRequestType object from this Executable object.
   *
   * @param client The Client being used to submit this Executable.
   * @param node   The Node to which this Executable will be sent.
   * @return A ProtoRequestType object filled with this Executable object's data.
   */
  [[nodiscard]] virtual ProtoRequestType makeRequest(
    [[maybe_unused]] const Client& client,
    [[maybe_unused]] const std::shared_ptr<internal::Node>& node) const = 0;

  /**
   * Construct an SdkResponseType from a ProtoResponseType object.
   *
   * @param response The ProtoResponseType object from which to construct an SdkResponseType object.
   * @return An SdkResponseType object filled with the ProtoResponseType object's data.
   */
  [[nodiscard]] virtual SdkResponseType mapResponse(const ProtoResponseType& response) const = 0;

  /**
   * Get the status response code from a ProtoResponseType object.
   *
   * @param response The ProtoResponseType object from which to grab the status response code.
   * @return The status response code of the input ProtoResponseType object.
   */
  [[nodiscard]] virtual Status mapResponseStatus(const ProtoResponseType& response) const = 0;

  /**
   * Submit this Executable to a Node.
   *
   * @param client   The Client submitting this Executable.
   * @param deadline The deadline for submitting this Executable.
   * @param node     Pointer to the Node to which this Executable should be submitted.
   * @param response Pointer to the ProtoResponseType object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] virtual grpc::Status submitRequest(const Client& client,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   ProtoResponseType* response) const = 0;

  /**
   * Perform any needed actions for this Executable when it is being submitted.
   *
   * @param client The Client being used to submit this Executable.
   */
  virtual void onExecute([[maybe_unused]] const Client& client) = 0;

  /**
   * Set the execution parameters to be used to submit this Executable. If any of mMaxAttempts, mMinBackoff, or
   * mMaxBackoff have been set with setMaxAttempts(), setMinBackoff(), or setMaxBackoff() respectively, these values
   * will be placed into mCurrentMaxAttempts, mCurrentMinBackoff, and mCurrentMaxBackoff respectively. If these values
   * have not been set for this Executable, the respective values for the Client will be grabbed and set instead. If
   * these values have not been set in the Client, the defaults are set.
   *
   * @param client The Client submitting this Executable.
   */
  void setExecutionParameters(const Client& client);

  /**
   * Get a Node from a list of Nodes to which to try and send this Executable. This will prioritize getting "healthy"
   * Nodes first in order to ensure as little wait time to submit as possible.
   *
   * @param nodes The list of Nodes from which to select a Node.
   * @return A pointer to a Node to which to try and send this Executable.
   */
  [[nodiscard]] std::shared_ptr<internal::Node> getNodeForExecute(
    const std::vector<std::shared_ptr<internal::Node>>& nodes) const;

  /**
   * The list of account IDs of the nodes with which execution should be attempted.
   */
  std::vector<AccountId> mNodeAccountIds;

  /**
   * The maximum number of attempts that will be made to submit this Executable. If not set, a submission will use the
   * Client's set maximum number of attempts. If that's not set, DEFAULT_MAX_ATTEMPTS will be used.
   */
  std::optional<uint32_t> mMaxAttempts;

  /**
   * The minimum amount of time to wait between submission attempts. If not set, a submission will use the Client's set
   * minimum backoff. If that's not set, DEFAULT_MIN_BACKOFF will be used.
   */
  std::optional<std::chrono::duration<double>> mMinBackoff;

  /**
   * The maximum amount of time to wait between submission attempts. If not set, a submission will use the Client's set
   * maximum backoff. If that's not set, DEFAULT_MAX_BACKOFF will be used.
   */
  std::optional<std::chrono::duration<double>> mMaxBackoff;

  /**
   * The maximum number of attempts to be used for an execution. This may be this Executable's mMaxAttempts, the
   * Client's max attempts, or DEFAULT_MAX_ATTEMPTS.
   */
  uint32_t mCurrentMaxAttempts;

  /**
   * The minimum backoff to be used for an execution. This may be this Executable's mMinBackoff, the Client's set
   * minimum backoff, or DEFAULT_MIN_BACKOFF.
   */
  std::chrono::duration<double> mCurrentMinBackoff;

  /**
   * The maximum backoff to be used for an execution. This may be this Executable's mMaxBackoff, the Client's set
   * maximum backoff, or DEFAULT_MAX_BACKOFF.
   */
  std::chrono::duration<double> mCurrentMaxBackoff;

  /**
   * The current backoff time being used during the current execution. Every failed submission attempt waits a certain
   * amount of time that is double the previous amount of time this Executable waited for its previous submission
   * attempt, up to the specified maximum backoff time, at which point the execution is considered a failure.
   */
  std::chrono::duration<double> mCurrentBackoff;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_EXECUTABLE_H_

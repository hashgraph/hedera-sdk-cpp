/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_IMPL_BASE_NODE_H_
#define HEDERA_SDK_CPP_IMPL_BASE_NODE_H_

#include "BaseNodeAddress.h"
#include "Defaults.h"

#include <chrono>
#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>

namespace Hedera::internal
{
template<typename NodeType, typename KeyType>
class BaseNode
{
public:
  /**
   * Get this BaseNode's key.
   *
   * @return This BaseNode's key.
   */
  [[nodiscard]] virtual KeyType getKey() const = 0;

  /**
   * Close this BaseNode's connection to its remote node.
   */
  void close();

  /**
   * Increase the backoff of this BaseNode.
   */
  void increaseBackoff();

  /**
   * Decrease the backoff of this BaseNode.
   */
  void decreaseBackoff();

  /**
   * Is this BaseNode currently considered healthy?
   *
   * @return \c TRUE if this BaseNode is healthy, otherwise \c FALSE.
   */
  [[nodiscard]] bool isHealthy() const;

  /**
   * Has this BaseNode failed to connect to its remote node?
   *
   * @return \c TRUE if this BaseNode has failed to connect to its remote node, otherwise \c FALSE.
   */
  [[nodiscard]] bool channelFailedToConnect();

  /**
   * Get the remaining amount of time this BaseNode has in its backoff.
   *
   * @return The remaining amount of time this BaseNode has in its backoff.
   */
  [[nodiscard]] std::chrono::duration<double> getRemainingTimeForBackoff() const;

  /**
   * Set the minimum amount of time for this BaseNode to backoff after a bad gRPC status is received.
   *
   * @param backoff The minimum backoff time for this BaseNode after a bad gRPC status is received.
   * @return A reference to this derived BaseNode object with the newly-set minimum backoff.
   */
  NodeType& setMinNodeBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Set the maximum amount of time for this BaseNode to backoff after a bad gRPC status is received.
   *
   * @param backoff The maximum backoff time for this BaseNode after a bad gRPC status is received.
   * @return A reference to this derived BaseNode object with the newly-set minimum backoff.
   */
  NodeType& setMaxNodeBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Get this BaseNode's BaseNodeAddress.
   *
   * @return address The BaseNodeAddress of this BaseNode.
   */
  [[nodiscard]] inline BaseNodeAddress getAddress() const { return mAddress; }

  /**
   * Get the minimum amount of time for this BaseNode to backoff after a bad gRPC status is received.
   *
   * @return The minimum amount of time for this BaseNode to backoff after a bad gRPC status is received.
   */
  [[nodiscard]] inline std::chrono::duration<double> getMinNodeBackoff() const { return mMinNodeBackoff; }

  /**
   * Get the maximum amount of time for this BaseNode to backoff after a bad gRPC status is received.
   *
   * @return The maximum amount of time for this BaseNode to backoff after a bad gRPC status is received.
   */
  [[nodiscard]] inline std::chrono::duration<double> getMaxNodeBackoff() const { return mMaxNodeBackoff; }

  /**
   * Get the number of times this BaseNode has received a bad gRPC status when attempting to submit a request.
   *
   * @return The number of times this BaseNode has received a bad gRPC status.
   */
  [[nodiscard]] inline unsigned int getBadGrpcStatusCount() const { return mBadGrpcStatusCount; }

  /**
   * Get the time at which this BaseNode will be considered "healthy".
   *
   * @return The time at which this BaseNode will be considered "healthy".
   */
  [[nodiscard]] inline std::chrono::system_clock::time_point getReadmitTime() const { return mReadmitTime; }

protected:
  ~BaseNode() = default;

  /**
   * Construct with a BaseNodeAddress.
   *
   * @param address The BaseNodeAddress of this BaseNode.
   */
  explicit BaseNode(BaseNodeAddress address);

  /**
   * Get this BaseNode's gRPC channel. Creates and initializes a new channel if one isn't already created.
   *
   * @return A pointer to this BaseNode's gRPC channel.
   */
  [[nodiscard]] std::shared_ptr<grpc::Channel> getChannel();

private:
  /**
   * How often to query for the state of the channel when determining if this BaseNode has connected to its remote node.
   */
  static constexpr auto GET_STATE_INTERVAL = std::chrono::milliseconds(50);

  /**
   * How long to try and let the channel connect before calling the connection a failure.
   */
  static constexpr auto GET_STATE_TIMEOUT = std::chrono::seconds(10);

  /**
   * Get the TLS credentials for this BaseNode's gRPC channel.
   *
   * @return A pointer to the TLS credentials for this BaseNode's gRPC channel.
   */
  [[nodiscard]] virtual std::shared_ptr<grpc::ChannelCredentials> getTlsChannelCredentials() const;

  /**
   * Initialize the stubs in this derived BaseNode with a gRPC channel.
   *
   * @param channel The gRPC channel with which to initialize the stubs.
   */
  virtual void initializeStubs([[maybe_unused]] const std::shared_ptr<grpc::Channel>& channel)
  { // Intentionally unimplemented, derived BaseNodes that don't use stubs require no functionality.
  }

  /**
   * Close the stubs in this derived BaseNode.
   */
  virtual void closeStubs()
  { // Intentionally unimplemented, derived BaseNodes that don't use stubs require no functionality.
  }

  /**
   * Get the authority of this BaseNode.
   *
   * @return The authority of this BaseNode.
   */
  [[nodiscard]] virtual inline std::string getAuthority() const { return "127.0.0.1"; }

  /**
   * The address of this BaseNode.
   */
  BaseNodeAddress mAddress;

  /**
   * Pointer to the gRPC channel used to communicate with the gRPC server living on the remote node.
   */
  std::shared_ptr<grpc::Channel> mChannel = nullptr;

  /**
   * The minimum amount of time to wait to use this BaseNode after it has received a bad gRPC status.
   */
  std::chrono::duration<double> mMinNodeBackoff = DEFAULT_MIN_NODE_BACKOFF;

  /**
   * The maximum amount of time to wait to use this BaseNode after it has received a bad gRPC status.
   */
  std::chrono::duration<double> mMaxNodeBackoff = DEFAULT_MAX_NODE_BACKOFF;

  /**
   * The current amount of time to wait to use this BaseNode after it has received a bad gRPC status. This will increase
   * exponentially until mMaxNodeBackoff is reached.
   */
  std::chrono::duration<double> mCurrentBackoff = DEFAULT_MIN_NODE_BACKOFF;

  /**
   * The time at which this BaseNode will be considered "healthy".
   */
  std::chrono::system_clock::time_point mReadmitTime = std::chrono::system_clock::now();

  /**
   * The number of times this BaseNode has received a bad gRPC status.
   */
  unsigned int mBadGrpcStatusCount = 0U;

  /**
   * Is the gRPC channel being utilized by this BaseNode to communicate with its remote node initialized?
   */
  bool mIsConnected = false;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_BASE_NODE_H_

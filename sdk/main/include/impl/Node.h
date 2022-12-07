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

#include "Defaults.h"
#include "impl/NodeAddress.h"
#include "impl/TLSBehavior.h"

#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>
#include <memory>
#include <proto/crypto_service.grpc.pb.h>

namespace Hedera::internal
{
class Node
{
public:
  /**
   * Construct this Node with an address.
   *
   * @param address The desired NodeAddress for this Node.
   * @param tls     The TLS behavior this Node should initially use. Defaults to requiring TLS.
   */
  explicit Node(std::shared_ptr<NodeAddress> address, TLSBehavior tls = TLSBehavior::REQUIRE);

  /**
   * Attempt to connect this Node to a Hedera network.
   *
   * @param timeout The point in time that the attempted connection will cease and be considered a failure.
   * @return \c TRUE if the Node is already connected or successfully connected, otherwise \c FALSE.
   */
  bool connect(const std::chrono::system_clock::time_point& timeout);

  /**
   * Shutdown this Node.
   */
  void shutdown();

  /**
   * Submit a Query to this Node.
   *
   * @param funcEnum The enumeration specifying which gRPC function to call for this specific Query.
   * @param query    The Query protobuf object to send.
   * @param deadline The deadline for submitting this Query.
   * @param response Pointer to the Response protobuf object to fill with the gRPC server's response.
   * @return The gRPC status response of the function call from the gRPC server.
   */
  grpc::Status submitQuery(proto::Query::QueryCase funcEnum,
                           const proto::Query& query,
                           const std::chrono::system_clock::time_point& deadline,
                           proto::Response* response);

  /**
   * Submit a Transaction to this Node.
   *
   * @param funcEnum    The enumeration specifying which gRPC function to call for this specific Transaction.
   * @param transaction The Transaction protobuf object to send.
   * @param deadline    The deadline for submitting this Transaction.
   * @param response    Pointer to the TransactionResponse protobuf object to fill with the gRPC server's response.
   * @return The gRPC status response of the function call from the gRPC server.
   */
  grpc::Status submitTransaction(proto::TransactionBody::DataCase funcEnum,
                                 const proto::Transaction& transaction,
                                 const std::chrono::system_clock::time_point& deadline,
                                 proto::TransactionResponse* response);

  /**
   * Set the TLS behavior this Node should utilize.
   *
   * @param desiredBehavior The desired behavior.
   */
  void setTLSBehavior(TLSBehavior desiredBehavior);

  /**
   * Set the minimum backoff time this Node should use.
   *
   * @param backoff The minimum backoff time.
   */
  void setMinBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Set the maximum backoff time this Node should use.
   *
   * @param backoff The maximum backoff time.
   */
  void setMaxBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Determine if this Node is "healthy", meaning this Node has not received any bad gRPC statuses, or if it has then it
   * has fully backed off for its full period of backoff time.
   *
   * @return \c TRUE if this Node is healthy, otherwise \c FALSE.
   */
  [[nodiscard]] bool isHealthy() const;

  /**
   * Increase the backoff of this Node. This should be called when the Node either fails to connect to the network or
   * returns a bad gRPC status.
   */
  void increaseBackoff();

  /**
   * Decrease the backoff of this Node. This should be called when the Node either fails to connect to the network or
   * returns a bad gRPC status.
   */
  void decreaseBackoff();

  /**
   * Get the amount of time remaining in this Node's current backoff.
   *
   * @return The amount of time that should pass before this Node is good to submit another request.
   */
  [[nodiscard]] std::chrono::duration<double> getRemainingTimeForBackoff() const;

  /**
   * Get the ID of the account associated with this Node.
   *
   * @return The account ID.
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAddress->getAccountId(); }

private:
  /**
   * Initialize the gRPC channel based on the TLS behavior.
   *
   * @param deadline The deadline to connect. Processing will wait until this point to establish a connection and will
   *                 return \c FALSE if not established by then.
   * @return \c TRUE if initialization was successful, otherwise \c FALSE.
   */
  bool initializeChannel(const std::chrono::system_clock::time_point& deadline);

  /**
   * Pointer to the NodeAddress of this Node.
   */
  std::shared_ptr<NodeAddress> mAddress = nullptr;

  /**
   * The TLS channel credentials. This only depends on the NodeAddress so this will be updated whenever mAddress is.
   */
  std::shared_ptr<grpc::ChannelCredentials> mTlsChannelCredentials = nullptr;

  /**
   * Pointer to the gRPC channel used to communicate with the Hedera network.
   */
  std::shared_ptr<grpc::Channel> mChannel = nullptr;

  /**
   * Pointer to the gRPC stub used to communicate with the cryptography service.
   */
  std::unique_ptr<proto::CryptoService::Stub> mCryptoStub = nullptr;

  /**
   * The TLS behavior of this Node.
   */
  TLSBehavior mTLSBehavior = TLSBehavior::REQUIRE;

  /**
   * The point in time that this Node would be considered healthy again.
   */
  std::chrono::system_clock::time_point mReadmitTime = std::chrono::system_clock::now();

  /**
   * The minimum amount of time to wait between submission attempts.
   */
  std::chrono::duration<double> mMinBackoff = DEFAULT_MIN_BACKOFF;

  /**
   * The maximum amount of time to wait between submission attempts.
   */
  std::chrono::duration<double> mMaxBackoff = DEFAULT_MAX_BACKOFF;

  /**
   * The current backoff time. Every failed submission attempt waits a certain amount of time that is double the
   * previous amount of time this Executable waited for its previous submission attempt, up to the specified maximum
   * backoff time, at which point the execution is considered a failure.
   */
  std::chrono::duration<double> mCurrentBackoff = mMinBackoff;

  /**
   * Is the gRPC channel being utilized by this Node initialized?
   */
  bool mIsInitialized = false;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_NODE_H_

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
#ifndef HEDERA_SDK_CPP_CLIENT_H_
#define HEDERA_SDK_CPP_CLIENT_H_

#include <chrono>
#include <memory>
#include <optional>
#include <vector>

namespace Hedera
{
namespace internal
{
class Node;
}
class AccountId;
class Hbar;
class PrivateKey;
class PublicKey;
}

namespace Hedera
{
/**
 * Managed client for use on the Hedera Hashgraph network.
 */
class Client
{
public:
  /**
   * Constructor creates implementation object.
   */
  Client();

  /**
   * Destructor closes all network connections.
   */
  ~Client();

  /**
   * No copying allowed. Underlying connections should be moved instead of copied.
   */
  Client(Client&& other) noexcept;
  Client& operator=(Client&& other) noexcept;

  /**
   * Construct a Hedera client pre-configured for Hedera testnet access.
   *
   * @return A Client object that can communicate with the Hedera testnet.
   */
  static Client forTestnet();

  /**
   * Set the account that will, by default, be paying for requests submitted by this Client. The operator account ID is
   * used to generate the default transaction ID for all transactions executed with this Client. The operator private
   * key is used to sign all transactions executed by this Client.
   *
   * @param accountId  The account ID of the operator.
   * @param privateKey Pointer to the private key of the operator. This transfers ownership of the pointed-to PrivateKey
   *                   to this Client.
   * @return A reference to this Client object with the newly-set operator account ID and private key.
   */
  Client& setOperator(const AccountId& accountId, std::unique_ptr<PrivateKey>& privateKey);
  Client& setOperator(const AccountId& accountId, std::unique_ptr<PrivateKey>&& privateKey);

  /**
   * Sign a serialized request with this Client's operator.
   *
   * @param bytes The bytes for this Client's operator to sign.
   * @return The bytes, signed with this Client's operator.
   * @throws std::runtime_error If no client operator has been set.
   */
  [[nodiscard]] std::vector<unsigned char> sign(const std::vector<unsigned char>& bytes) const;

  /**
   * Set the maximum fee to be paid for requests executed by this Client. Every request submitted with this Client will
   * have its maximum transaction fee overwritten by this Client's maximum transaction fee if it has not been set
   * manually in the request itself.
   *
   * @param defaultMaxTransactionFee The desired maximum transaction fee for requests made by this Client.
   * @return A reference to this Client object with the newly-set default maximum transaction fee.
   * @throws std::invalid_argument If the transaction fee is negative.
   */
  Client& setDefaultMaxTransactionFee(const Hbar& defaultMaxTransactionFee);

  /**
   * Set the transaction ID regeneration policy for requests executed by this Client. Every request submitted with this
   * Client will have its transaction ID regeneration policy overwritten by this Client's transaction ID regeneration
   * policy if it has not been set manually in the request itself.
   */
  Client& setTransactionIdRegenerationPolicy(bool regenerate);

  /**
   * Set the maximum number of times a request being submitted by this Client should attempt to be submitted to this
   * Client's network before the execution is considered a failure.
   *
   * @param attempts The desired maximum number of execution attempts.
   * @return A reference to this Client with the newly-set maximum number of execution attempts.
   */
  Client& setMaxAttempts(uint32_t attempts);

  /**
   * Set the minimum amount of time requests being submitted by this Client should wait after a failed submission to a
   * node before attempting another to that same node.
   *
   * @param backoff The desired minimum amount of time to wait between retries to a specific node.
   * @return A reference to this Client with the newly-set minimum backoff time.
   * @throws std::invalid_argument If the desired minimum backoff duration is longer than the set maximum backoff time
   *                               (DEFAULT_MAX_BACKOFF if the maximum backoff time has not been set).
   */
  Client& setMinBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Set the maximum amount of time requests being submitted by this Client should wait after a failed submission to a
   * node before attempting another to that same node.
   *
   * @param backoff The desired maximum amount of time to wait between retries to a specific node.
   * @return A reference to this Client with the newly-set maximum backoff time.
   * @throws std::invalid_argument If the desired maximum backoff duration is shorter than the set minimum backoff time
   *                               (DEFAULT_MIN_BACKOFF if the minimum backoff time has not been set).
   */
  Client& setMaxBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Get a list of pointers to Nodes on this Client's network that are associated with the input account IDs. If no
   * account IDs are specified, this returns the list a list of pointers to all nodes in this Client's network.
   *
   * @param accountIds The account IDs of the requested nodes.
   * @return A list of nodes that are associated with the requested account IDs.
   * @throws std::runtime_error If no client network has been initialized.
   */
  [[nodiscard]] std::vector<std::shared_ptr<internal::Node>> getNodesWithAccountIds(
    const std::vector<AccountId>& accountIds) const;

  /**
   * Initiates an orderly shutdown of communications with the network with which this Client was configured to
   * communicate. Preexisting transactions or queries continue but more would be immediately cancelled.
   *
   * After this method returns, this client can be re-used. All network communication can be re-established as needed.
   */
  void close() const;

  /**
   * Get the account ID of this Client's operator.
   *
   * @return A pointer to the account ID of this Client's operator. Nullptr if the operator has not been set yet.
   */
  [[nodiscard]] std::optional<AccountId> getOperatorAccountId() const;

  /**
   * Get the public key of this Client's operator.
   *
   * @return A pointer to the public key of this Client's operator. Nullptr if the operator has not been set yet.
   */
  [[nodiscard]] std::unique_ptr<PublicKey> getOperatorPublicKey() const;

  /**
   * Get the default maximum fee used for transactions.
   *
   * @return A pointer to the max transaction fee. Nullptr if the default max transaction fee has not been set.
   */
  [[nodiscard]] std::optional<Hbar> getDefaultMaxTransactionFee() const;

  /**
   * Get the transaction ID regeneration policy for transactions.
   *
   * @return The default transaction ID regeneration policy for this Client.
   */
  [[nodiscard]] std::optional<bool> getTransactionIdRegenerationPolicy() const;

  /**
   * Get the request timeout, which defaults to 2 minutes.
   *
   * @return The request timeout.
   */
  [[nodiscard]] std::chrono::duration<double> getRequestTimeout() const;

  /**
   * Get the maximum number of execution attempts this Executable is currently configured to try.
   *
   * @return The maximum number of execution attempts.
   */
  [[nodiscard]] std::optional<uint32_t> getMaxAttempts() const;

  /**
   * Get the minimum amount of time this Executable is currently configured to wait between submission attempts to a
   * specific node.
   *
   * @return The minimum amount of time between submission attempts to a specific node.
   */
  [[nodiscard]] std::optional<std::chrono::duration<double>> getMinBackoff() const;

  /**
   * Get the maximum amount of time this Executable is currently configured to wait between submission attempts to a
   * specific node.
   *
   * @return The maximum amount of time between submission attempts to a specific node.
   */
  [[nodiscard]] std::optional<std::chrono::duration<double>> getMaxBackoff() const;

private:
  /**
   * Implementation object used to hide implementation details and internal headers.
   */
  struct ClientImpl;
  std::unique_ptr<ClientImpl> mImpl;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CLIENT_H_

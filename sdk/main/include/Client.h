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
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace Hedera
{
namespace internal
{
class MirrorNetwork;
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
   * Construct a Client pre-configured for a specific Hedera network.
   *
   * @param networkMap The map with string representation of node addresses with their corresponding accountId.
   * @return A reference to this Client object with the newly-set operator account ID from the map.
   */
  [[nodiscard]] static Client forNetwork(const std::unordered_map<std::string, AccountId>& networkMap);

  /**
   * Construct a Client pre-configured for Hedera Mainnet access.
   *
   * @return A Client object that is set-up to communicate with the Hedera Mainnet.
   */
  [[nodiscard]] static Client forMainnet();

  /**
   * Construct a Client pre-configured for Hedera Testnet access.
   *
   * @return A Client object that is set-up to communicate with the Hedera Testnet.
   */
  [[nodiscard]] static Client forTestnet();

  /**
   * Construct a Client pre-configured for Hedera Previewnet access.
   *
   * @return A Client object that is set-up to communicate with the Hedera Previewnet.
   */
  [[nodiscard]] static Client forPreviewnet();

  /**
   * Set the mirror network with which this Client should communicate.
   *
   * @param network The list of IPs and ports of the mirror nodes with which this Client should communicate.
   * @return A reference to this Client object with the newly-set mirror network.
   */
  Client& setMirrorNetwork(const std::vector<std::string>& network);

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
  Client& setOperator(const AccountId& accountId, const PrivateKey* privateKey);

  /**
   * Sign an arbitrary array of bytes with this Client's operator.
   *
   * @param bytes The bytes for this Client's operator to sign.
   * @return The generated signature of this Client's operator.
   * @throws UninitializedException If this Client's operator has not yet been set.
   */
  [[nodiscard]] std::vector<std::byte> sign(const std::vector<std::byte>& bytes) const;

  /**
   * Get a list of pointers to Nodes on this Client's network that are associated with the input account IDs. If no
   * account IDs are specified, this returns the list a list of pointers to all nodes in this Client's network.
   *
   * @param accountIds The account IDs of the requested nodes. This can be empty to get all Nodes.
   * @return A list of pointers to Nodes that are associated with the requested account IDs.
   * @throws UninitializedException If this Client's network has not yet been initialized.
   */
  [[nodiscard]] std::vector<std::shared_ptr<internal::Node>> getNodesWithAccountIds(
    const std::vector<AccountId>& accountIds) const;

  /**
   * Initiate an orderly shutdown of communications with the network with which this Client was configured to
   * communicate. Preexisting transactions or queries continue but subsequent calls would be immediately cancelled.
   *
   * After this method returns, this Client can be re-used. All network communication can be re-established as needed.
   */
  void close() const;

  /**
   * Set the length of time a request sent by this Client can be processed before it times out.
   *
   * @param timeout The desired timeout for requests submitted by this Client.
   * @return A reference to this Client object with the newly-set request timeout.
   */
  Client& setRequestTimeout(const std::chrono::duration<double>& timeout);

  /**
   * Set the maximum transaction fee willing to be paid for transactions executed by this Client. Every request
   * submitted with this Client will have its maximum transaction fee overwritten by this Client's maximum transaction
   * fee if and only if it has not been set manually in the request itself.
   *
   * @param fee The desired maximum transaction fee willing to be paid for transactions submitted by this Client.
   * @return A reference to this Client object with the newly-set maximum transaction fee.
   * @throws std::invalid_argument If the transaction fee is negative.
   */
  Client& setMaxTransactionFee(const Hbar& fee);

  /**
   * Set the transaction ID regeneration policy for transactions executed by this Client. Every transaction submitted
   * with this Client will have its transaction ID regeneration policy overwritten by this Client's transaction ID
   * regeneration policy if and only if it has not been set manually in the request itself.
   *
   * @param regenerate The desired transaction ID regeneration policy for transactions submitted by this Client.
   * @return A reference to this Client object with the newly-set transaction ID regeneration policy.
   */
  Client& setTransactionIdRegenerationPolicy(bool regenerate);

  /**
   * Set the maximum number of times this Client is willing to resubmit failed requests before considering the execution
   * of the request a failure. Every request submitted with this Client will have its maximum number of attempts
   * overwritten by this Client's maximum number of attempts if and only if it has not been set manually in the request
   * itself.
   *
   * @param attempts The desired maximum number of execution attempts for requests submitted by this Client.
   * @return A reference to this Client with the newly-set maximum number of execution attempts.
   */
  Client& setMaxAttempts(uint32_t attempts);

  /**
   * Set the minimum amount of time this Client should wait before attempting to resubmit a previously failed request to
   * the same node. Every request submitted with this Client will have its minimum backoff time overwritten by this
   * Client's minimum backoff time if and only if it has not been set manually in the request itself.
   *
   * @param backoff The desired minimum amount of time this Client should wait before attempting to resubmit a
   *                previously failed to a particular node.
   * @return A reference to this Client with the newly-set minimum backoff time.
   * @throws std::invalid_argument If the desired minimum backoff duration is longer than the set maximum backoff time
   *                               (DEFAULT_MAX_BACKOFF if the maximum backoff time has not been set).
   */
  Client& setMinBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Set the maximum amount of time this Client should wait before attempting to resubmit a previously failed request to
   * the same node. Every request submitted with this Client will have its maximum backoff time overwritten by this
   * Client's maximum backoff time if and only if it has not been set manually in the request itself.
   *
   * @param backoff The desired maximum amount of time requests submitted by this Client should wait before attempting
   *                to resubmit themselves to a particular node.
   * @return A reference to this Client with the newly-set maximum backoff time.
   * @throws std::invalid_argument If the desired maximum backoff duration is shorter than the set minimum backoff time
   *                               (DEFAULT_MIN_BACKOFF if the minimum backoff time has not been set).
   */
  Client& setMaxBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Get the account ID of this Client's operator.
   *
   * @return The account ID of this Client's operator. Uninitialized if the operator has not yet been set.
   */
  [[nodiscard]] std::optional<AccountId> getOperatorAccountId() const;

  /**
   * Get the public key of this Client's operator.
   *
   * @return A pointer to the public key of this Client's operator. Nullptr if the operator has not yet been set.
   */
  [[nodiscard]] std::shared_ptr<PublicKey> getOperatorPublicKey() const;

  /**
   * Get the signer function of this Client's operator.
   *
   * @return The signer function of this Client's operator.
   */
  [[nodiscard]] std::function<std::vector<std::byte>(const std::vector<std::byte>&)> getOperatorSigner() const;

  /**
   * Get the length of time a request sent by this Client can be processed before it times out.
   *
   * @return The request timeout duration. Defaults to 2 minutes if this value has not been set with
   *         setRequestTimeout().
   */
  [[nodiscard]] std::chrono::duration<double> getRequestTimeout() const;

  /**
   * Get the maximum transaction fee willing to be paid for transactions submitted by this Client.
   *
   * @return The maximum transaction fee willing to be paid by this Client. Uninitialized if the fee has not yet been
   *         set.
   */
  [[nodiscard]] std::optional<Hbar> getMaxTransactionFee() const;

  /**
   * Get the transaction ID regeneration policy for transactions submitted by this Client.
   *
   * @return The default transaction ID regeneration policy for this Client. Uninitialized if the policy has not yet
   *         been set.
   */
  [[nodiscard]] std::optional<bool> getTransactionIdRegenerationPolicy() const;

  /**
   * Get the maximum number of times this Client is willing to resubmit failed requests before considering the execution
   * of the request a failure.
   *
   * @return The maximum number of execution attempts. Uninitialized value if not previously set.
   */
  [[nodiscard]] std::optional<uint32_t> getMaxAttempts() const;

  /**
   * Get the minimum amount of time this Client should wait before attempting to resubmit a previously failed request to
   * the same node.
   *
   * @return The minimum backoff time for Nodes for unsuccessful requests sent by this Client. Uninitialized value if
   *         not previously set.
   */
  [[nodiscard]] std::optional<std::chrono::duration<double>> getMinBackoff() const;

  /**
   * Get the maximum amount of time this Client should wait before attempting to resubmit a previously failed request to
   * the same node.
   *
   * @return The maximum backoff time for Nodes for unsuccessful requests sent by this Client. Uninitialized value if
   *         not previously set.
   */
  [[nodiscard]] std::optional<std::chrono::duration<double>> getMaxBackoff() const;

  /**
   * Get a pointer to the MirrorNetwork being used by this client.
   *
   * @return A pointer to the MirrorNetwork being used by this client.
   */
  [[nodiscard]] std::shared_ptr<internal::MirrorNetwork> getMirrorNetwork() const;

private:
  /**
   * Implementation object used to hide implementation details and internal headers.
   */
  struct ClientImpl;
  std::unique_ptr<ClientImpl> mImpl;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CLIENT_H_

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
#ifndef HEDERA_SDK_CPP_CLIENT_H_
#define HEDERA_SDK_CPP_CLIENT_H_

#include <chrono>
#include <filesystem>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Hedera
{
namespace internal
{
class MirrorNetwork;
class Network;
}
class AccountId;
class Hbar;
class LedgerId;
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
   * Construct a Client by a name. The name must be one of "mainnet", "testnet", or "previewnet", otherwise this will
   * throw std::invalid_argument.
   *
   * @param name The name of the Client to construct.
   * @return A Client object that is set-up to communicate with the input network name.
   */
  [[nodiscard]] static Client forName(std::string_view name);

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
   * Construct a Client from a JSON configuration string.
   *
   * @param json The JSON configuration string.
   * @return A Client object initialized with the properties specified in the JSON configuration string.
   */
  [[nodiscard]] static Client fromConfig(std::string_view json);

  /**
   * Construct a Client from a JSON configuration object.
   *
   * @param json The JSON configuration object.
   * @return A Client object initialized with the properties specified in the JSON configuration object.
   */
  [[nodiscard]] static Client fromConfig(const nlohmann::json& json);

  /**
   * Construct a Client from a JSON configuration file.
   *
   * @param path The filepath to the JSON configuration file.
   * @return A Client object initialized with the properties specified in the JSON configuration file.
   */
  [[nodiscard]] static Client fromConfigFile(std::string_view path);

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
   * @param privateKey Pointer to the private key of the operator.
   * @return A reference to this Client object with the newly-set operator account ID and private key.
   */
  Client& setOperator(const AccountId& accountId, const std::shared_ptr<PrivateKey>& privateKey);

  /**
   * Initiate an orderly close of communications with the network with which this Client was configured to
   * communicate. Preexisting transactions or queries continue but subsequent calls would be immediately cancelled.
   *
   * After this method returns, this Client can be re-used. All network communication can be re-established as needed.
   */
  void close();

  /**
   * Replace all nodes in this Client with a new set of nodes from the Hedera network.
   *
   * @param networkMap The map with string representation of node addresses with their corresponding accountId.
   * @return A reference to this Client object with the newly-set operator account ID from the map.
   */
  Client& setNetwork(const std::unordered_map<std::string, AccountId>& networkMap);

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
   * Set the maximum query payment willing to be paid for transactions executed by this Client. Every request submitted
   * with this Client will have its maximum query payment overwritten by this Client's maximum query payment if and only
   * if it has not been set manually in the request itself.
   *
   * @param payment The desired maximum query payment willing to be paid for queries submitted by this Client.
   * @return A reference to this Client object with the newly-set maximum query payment.
   * @throws std::invalid_argument If the query payment is negative.
   */
  Client& setMaxQueryPayment(const Hbar& payment);

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
   * Set the period of time this Client wait between updating its network. This will immediately cancel any scheduled
   * network updates and start a new waiting period.
   *
   * @param update The period of time this Client wait between updating its network.
   * @return A reference to this Client with the newly-set network update period.
   */
  Client& setNetworkUpdatePeriod(const std::chrono::duration<double>& update);

  /**
   * Set the automatic entity checksum validation policy.
   *
   * @param validate \c TRUE if this Client should validate entity checksums when submitting Executables, otherwise \c
   *                 FALSE.
   * @return A reference to this Client with the newly-set automatic entity checksum validation policy.
   */
  Client& setAutoValidateChecksums(bool validate);

  /**
   * Set the ID of the ledger of this Client's Network. Useful when constructing a Network which is a subset of an
   * existing known network. Does nothing if the Client's Network is not yet initialized.
   *
   * @param ledgerId The LedgerId to set.
   * @return A reference to this Client with the newly-set ledger ID.
   */
  Client& setLedgerId(const LedgerId& ledgerId);

  /**
   * Get a pointer to the Network this Client is using to communicate with consensus nodes.
   *
   * @return A pointer to the Network this Client is using to communicate with consensus nodes.
   */
  [[nodiscard]] std::shared_ptr<internal::Network> getNetwork() const;

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
   * Get the maximum payment willing to be paid for queries submitted by this Client.
   *
   * @return The maximum query payment willing to be paid by this Client. Uninitialized if the fee has not yet been set.
   */
  [[nodiscard]] std::optional<Hbar> getMaxQueryPayment() const;

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
   * Get the period of time this Client wait between updating its network.
   *
   * @return The period of time this Client wait between updating its network.
   */
  [[nodiscard]] std::chrono::duration<double> getNetworkUpdatePeriod() const;

  /**
   * Is automatic entity checksum validation turned on for this Client?
   *
   * @return \c TRUE if automatic entity checksum validation is turned on, otherwise \c FALSE.
   */
  [[nodiscard]] bool isAutoValidateChecksumsEnabled() const;

  /**
   * Get the ID of the ledger on which this Client's Network is running.
   *
   * @returns The ID of the ledger on which this Client's Network is running.
   * @throws UninitializedException If this Client does not have a Network initialized.
   */
  [[nodiscard]] LedgerId getLedgerId() const;

  /**
   * Get a pointer to the MirrorNetwork being used by this client.
   *
   * @return A pointer to the MirrorNetwork being used by this client.
   */
  [[nodiscard]] std::shared_ptr<internal::MirrorNetwork> getMirrorNetwork() const;

private:
  /**
   * Start the network update thread.
   *
   * @param period The period of time to wait before a network update is performed.
   */
  void startNetworkUpdateThread(const std::chrono::duration<double>& period);

  /**
   * Schedule a network update.
   */
  void scheduleNetworkUpdate();

  /**
   * Cancel any scheduled network updates.
   */
  void cancelScheduledNetworkUpdate();

  /**
   * Helper function used for moving a Client implementation into this Client, as well as doing network update thread
   * handling.
   *
   * @param other The Client to move into this Client.
   */
  void moveClient(Client&& other);

  /**
   * Implementation object used to hide implementation details and internal headers.
   */
  struct ClientImpl;
  std::unique_ptr<ClientImpl> mImpl;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CLIENT_H_

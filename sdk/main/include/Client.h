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
   * Default destructor, but must be "defined" after ChannelImpl implementation object is defined.
   */
  ~Client();

  /**
   * No copying allowed. Underlying connections should be moved instead of copied.
   */
  Client(const Client& other) = delete;
  Client& operator=(const Client& other) = delete;
  Client(Client&& other) noexcept = default;
  Client& operator=(Client&& other) noexcept = default;

  /**
   * Construct a Hedera client pre-configured for Hedera testnet access.
   *
   * @return A Client object that can communicate with the Hedera testnet.
   */
  static Client forTestnet();

  /**
   * Set the account that will, by default, be paying for requests submitted by this Client. The operator account ID is
   * used to generate the default transaction ID for all transactions executed with this Client. The operator private
   * key is used to sign all transactions executed by this client.
   *
   * @param accountId  The account ID of the operator.
   * @param privateKey Pointer to the private key of the operator. This transfers ownership of the pointed-to PrivateKey
   *                   to this Client.
   * @return A reference to this Client object with the newly-set operator account ID and private key.
   */
  Client& setOperator(const std::shared_ptr<AccountId>& accountId, std::unique_ptr<PrivateKey>& privateKey);
  Client& setOperator(const std::shared_ptr<AccountId>& accountId, std::unique_ptr<PrivateKey>&& privateKey);

  /**
   * Set the maximum fee to be paid for requests executed by this Client.
   *
   * Because transaction fees are always maximums, this will simply add a call to setMaxTransactionFee() on every new
   * transaction. The actual fee assessed for a given transaction may be less than this value, but never greater.
   *
   * @param defaultMaxTransactionFee The desired maximum transaction fee for requests made by this Client.
   * @return A reference to this Client object with the newly-set default maximum transaction fee.
   * @throws std::invalid_argument If the transaction fee is negative.
   */
  Client& setDefaultMaxTransactionFee(const Hbar& defaultMaxTransactionFee);

  /**
   * Sign a serialized request with this Client's operator.
   *
   * @param bytes The bytes for this Client's operator to sign.
   * @return The bytes, signed with this Client's operator.
   */
  [[nodiscard]] std::vector<unsigned char> sign(const std::vector<unsigned char>& bytes) const;

  /**
   * Get a list of nodes on this Client's network that are associated with the input account IDs. If no account IDs are
   * specified, returns all nodes.
   *
   * @param accountIds The account IDs of the requested nodes.
   * @return A list of nodes that are associated with the requested account IDs.
   */
  [[nodiscard]] std::vector<std::shared_ptr<internal::Node>> getNodesWithAccountIds(
    const std::vector<std::shared_ptr<AccountId>>& accountIds) const;

  /**
   * Initiates an orderly shutdown of all channels (to the Hedera network) in which preexisting transactions or queries
   * continue but more would be immediately cancelled.
   *
   * After this method returns, this client can be re-used. Channels will be re-established as needed.
   */
  void close() const;

  /**
   * Get the account ID of this Client's operator.
   *
   * @return A pointer to the account ID of this Client's operator. Nullptr if the operator has not been set yet.
   */
  [[nodiscard]] std::unique_ptr<AccountId> getOperatorAccountId() const;

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
  [[nodiscard]] std::unique_ptr<Hbar> getDefaultMaxTransactionFee() const;

  /**
   * Get the request timeout, which defaults to 2 minutes.
   *
   * @return The request timeout.
   */
  [[nodiscard]] std::chrono::duration<int64_t> getRequestTimeout() const;

private:
  /**
   * Only allow Clients to be constructed from within static functions.
   */
  Client();

  /**
   * Implementation object used to hide implementation details and internal headers.
   */
  struct ClientImpl;
  std::unique_ptr<ClientImpl> mImpl;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CLIENT_H_

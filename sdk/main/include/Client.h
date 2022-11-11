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
#ifndef CLIENT_H_
#define CLIENT_H_

#include "AccountId.h"
#include "Hbar.h"
#include "Network.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <memory>
#include <stdexcept>

namespace Hedera
{
/**
 * Managed client for use on the Hedera Hashgraph network.
 */
class Client
{
private:
  /**
   * Information about the account that will pay for transactions and queries made with this client.
   */
  struct Operator
  {
    /**
     * The account ID of the account.
     */
    std::shared_ptr<AccountId> mAccountId;

    /**
     * The private key of the account.
     */
    std::unique_ptr<PrivateKey> mPrivateKey;
  };

public:
  /**
   * Construct a Hedera client pre-configured for Hedera testnet access.
   *
   * @return Client object that can communicate with the Hedera testnet.
   */
  static Client forTestnet();

  /**
   * Set the account that will, by default, be paying for transactions and queries built with this client. The operator
   * account ID is used to generate the default transaction ID for all transactions executed with this client. The
   * operator private key is used to sign all transactions executed by this client.
   *
   * @param accountId  The account ID of the operator.
   * @param privateKey Pointer to the private key of the operator. This transfers ownership of the pointed-to PrivateKey
   *                   to this Client.
   * @return Reference to this Client object.
   */
  Client& setOperator(const std::shared_ptr<AccountId>& accountId, std::unique_ptr<PrivateKey>& privateKey);
  Client& setOperator(const std::shared_ptr<AccountId>& accountId, std::unique_ptr<PrivateKey>&& privateKey);

  /**
   * Set the maximum fee to be paid for transactions executed by this client.
   *
   * Because transaction fees are always maximums, this will simply add a call to setMaxTransactionFee() on every new
   * transaction. The actual fee assessed for a given transaction may be less than this value, but never greater.
   *
   * @param defaultMaxTransactionFee The Hbar to be set.
   * @return Reference to this Client object.
   * @throws std::invalid_argument If the transaction fee is negative.
   */
  Client& setDefaultMaxTransactionFee(const Hbar& defaultMaxTransactionFee);

  /**
   * Initiates an orderly shutdown of all channels (to the Hedera network) in which preexisting transactions or queries
   * continue but more would be immediately cancelled.
   *
   * After this method returns, this client can be re-used. Channels will be re-established as needed.
   */
  void close();

  /**
   * Get the network of this client.
   *
   * @return Pointer to the network of this client.
   */
  [[nodiscard]] inline std::shared_ptr<Network> getNetwork() const { return mNetwork; }

  /**
   * Extract the operator of this client.
   *
   * @return Pointer to the operator of this client. Nullptr if the operator has not been set yet.
   */
  [[nodiscard]] inline std::shared_ptr<Operator> getOperator() const { return mOperator; }

  /**
   * Get the account ID of the operator.
   *
   * @return The account ID of this client's operator.
   * @throws std::runtime_error If the operator has not yet been set.
   */
  [[nodiscard]] std::shared_ptr<AccountId> getOperatorAccountId() const;

  /**
   * Get the public key of the operator.
   *
   * @return The public key of this client's operator, if valid.
   * @throws std::runtime_error If the operator has not yet been set.
   */
  [[nodiscard]] std::shared_ptr<PublicKey> getOperatorPublicKey() const;

  /**
   * The default maximum fee used for transactions.
   *
   * @return Pointer to the max transaction fee. Nullptr if the default max transaction fee has not been set.
   */
  [[nodiscard]] inline std::unique_ptr<Hbar> getDefaultMaxTransactionFee() const
  {
    return mDefaultMaxTransactionFee ? std::make_unique<Hbar>(*mDefaultMaxTransactionFee) : nullptr;
  }

  /**
   * Extract the request timeout.
   *
   * @return The request timeout.
   */
  [[nodiscard]] inline std::chrono::duration<double> getRequestTimeout() const { return mRequestTimeout; }

private:
  /**
   * The network object encompassing the network setup this client is using. Defaults to uninitialized.
   */
  std::shared_ptr<Network> mNetwork;

  /**
   * The operator for this client. Defaults to uninitialized.
   */
  std::shared_ptr<Operator> mOperator;

  /**
   * The default max transaction fee for transactions processed by this client. Defaults to uninitialized.
   */
  std::unique_ptr<Hbar> mDefaultMaxTransactionFee;

  /**
   * The request timeout. Defaults to 2 minutes.
   */
  std::chrono::duration<double> mRequestTimeout = std::chrono::minutes(2);
};

} // namespace Hedera

#endif // CLIENT_H_

/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "PublicKey.h"

#include <functional>

namespace Hedera
{
class PrivateKey;
} // namespace Hedera

namespace Hedera
{
class Client
{
public:
  /**
   * Construct a Hedera client pre-configured for Testnet access.
   *
   * @return Client object that can communicate with Testnet.
   */
  static Client forTestnet();

  /**
   * Set the account that will, by default, be paying for transactions and queries built with this client. The operator
   * account ID is used to generate the default transaction ID for all transactions executed with this client. The
   * operator private key is used to sign all transactions executed by this client.
   *
   * @param accountId  The account ID of the operator.
   * @param privateKey The private key of the operator.
   * @return Reference to this Client object.
   */
  Client& setOperator(const AccountId& accountId, const PrivateKey& privateKey);

  /**
   * Set the maximum fee to be paid for transactions executed by this client.
   *
   * Because transaction fees are always maximums, this will simply add a call to setMaxTransactionFee() on every new
   * transaction. The actual fee assessed for a given transaction may be less than this value, but never greater.
   *
   * @param defaultMaxTransactionFee The Hbar to be set.
   * @return Reference to this Client object.
   */
  Client& setDefaultMaxTransactionFee(const Hbar& defaultMaxTransactionFee);

  /**
   * Initiates an orderly shutdown of all channels (to the Hedera network) in which preexisting transactions or queries
   * continue but more would be immediately cancelled.
   *
   * After this method returns, this client can be re-used. Channels will be
   * re-established as needed.
   */
  void close();

  /**
   * Get the network of this client.
   *
   * @return The network of this client.
   */
  inline Network getNetwork() const { return mNetwork; }

  /**
   * Get the ID of the operator. Useful when the client was constructed from file.
   *
   * @return The account ID of this client's operator, if valid.
   */
  inline std::optional<AccountId> getOperatorAccountId() const { return mOperator.mAccountId; }

  /**
   * Get the key of the operator. Useful when the client was constructed from file.
   *
   * @return The public key of this client's operator, if valid.
   */
  inline std::shared_ptr<PublicKey> getOperatorPublicKey() const { return mOperator.mPublicKey; }

  /**
   * The default maximum fee used for transactions.
   *
   * @return The max transaction fee.
   */
  inline std::optional<Hbar> getDefaultMaxTransactionFee() const { return mDefaultMaxTransactionFee; }

private:
  /**
   * Information about the account that will pay for transactions and queries made with this client.
   */
  struct Operator
  {
    /**
     * The account ID of the account.
     */
    std::optional<AccountId> mAccountId;

    /**
     * The public key of the account.
     */
    std::shared_ptr<PublicKey> mPublicKey;
  };

  /**
   * The network object encompassing the network setup this client is using.
   */
  Network mNetwork;

  /**
   * The operator for this client.
   */
  Operator mOperator;

  /**
   * The default max transaction fee for transactions processed by this client.
   */
  std::optional<Hbar> mDefaultMaxTransactionFee;
};

} // namespace Hedera

#endif // CLIENT_H_

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
#include "Client.h"
#include "AccountId.h"
#include "Defaults.h"
#include "Hbar.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "exceptions/UninitializedException.h"
#include "impl/Network.h"

#include <stdexcept>

namespace Hedera
{
//-----
struct Client::ClientImpl
{
  // Pointer to the network object that contains all processing for sending/receiving information to/from a Hedera
  // network.
  std::optional<internal::Network> mNetwork;

  // The ID of the account that will pay for requests using this Client.
  std::optional<AccountId> mOperatorAccountId;

  // Pointer to the private key this Client should use to sign transactions.
  std::unique_ptr<PrivateKey> mOperatorPrivateKey = nullptr;

  // The maximum fee this Client is willing to pay for transactions.
  std::optional<Hbar> mMaxTransactionFee;

  // The transaction ID regeneration policy to utilize when transactions submitted by this Client receive a
  // TRANSACTION_EXPIRED response from the network.
  std::optional<bool> mTransactionIdRegenerationPolicy;

  // The maximum length of time this Client should wait to get a response after sending a request to the network.
  std::chrono::duration<double> mRequestTimeout = std::chrono::minutes(2);

  // The maximum number of attempts a request sent by this Client will attempt to send a request. A manually-set
  // maximum number of attempts in the request will override this.
  std::optional<uint32_t> mMaxAttempts;

  // The minimum length of time a request sent to a Node by this Client will wait before attempting to send a request
  // again after a failure to the same Node. A manually-set minimum backoff in the request will override this.
  std::optional<std::chrono::duration<double>> mMinBackoff;

  // The maximum length of time a request sent to a Node by this Client will wait before attempting to send a request
  // again after a failure to the same Node. A manually-set maximum backoff in the request will override this.
  std::optional<std::chrono::duration<double>> mMaxBackoff;
};

//-----
Client::Client()
  : mImpl(std::make_unique<ClientImpl>())
{
}

//-----
Client::~Client()
{
  close();
}

//-----
Client::Client(Client&& other) noexcept
  : mImpl(std::move(other.mImpl))
{
  // Leave moved-from object in a valid state
  other.mImpl = std::make_unique<ClientImpl>();
}

//-----
Client& Client::operator=(Client&& other) noexcept
{
  mImpl = std::move(other.mImpl);

  // Leave moved-from object in a valid state
  other.mImpl = std::make_unique<ClientImpl>();
  return *this;
}

//-----
Client Client::forTestnet()
{
  Client client;
  client.mImpl->mNetwork = internal::Network::forTestnet();
  return client;
}

//-----
Client& Client::setOperator(const AccountId& accountId, std::unique_ptr<PrivateKey>& privateKey)
{
  return setOperator(accountId, std::move(privateKey));
}

//-----
Client& Client::setOperator(const AccountId& accountId, std::unique_ptr<PrivateKey>&& privateKey)
{
  mImpl->mOperatorAccountId = accountId;
  mImpl->mOperatorPrivateKey = std::move(privateKey);

  return *this;
}

//-----
std::vector<unsigned char> Client::sign(const std::vector<unsigned char>& bytes) const
{
  if (mImpl->mOperatorPrivateKey)
  {
    return mImpl->mOperatorPrivateKey->sign(bytes);
  }

  throw UninitializedException("No client operator private key with which to sign");
}

//-----
std::vector<std::shared_ptr<internal::Node>> Client::getNodesWithAccountIds(
  const std::vector<AccountId>& accountIds) const
{
  if (mImpl->mNetwork)
  {
    return mImpl->mNetwork->getNodesWithAccountIds(accountIds);
  }

  throw UninitializedException("Client network uninitialized");
}

void Client::close() const
{
  if (mImpl->mNetwork)
  {
    mImpl->mNetwork->close();
  }
}

//-----
Client& Client::setRequestTimeout(const std::chrono::duration<double>& timeout)
{
  mImpl->mRequestTimeout = timeout;
  return *this;
}

//-----
Client& Client::setMaxTransactionFee(const Hbar& fee)
{
  if (fee.toTinybars() < 0)
  {
    throw std::invalid_argument("Transaction fee cannot be negative");
  }

  mImpl->mMaxTransactionFee = fee;
  return *this;
}

//-----
Client& Client::setTransactionIdRegenerationPolicy(bool regenerate)
{
  mImpl->mTransactionIdRegenerationPolicy = regenerate;
  return *this;
}

//-----
Client& Client::setMaxAttempts(uint32_t attempts)
{
  mImpl->mMaxAttempts = attempts;
  return *this;
}

//-----
Client& Client::setMinBackoff(const std::chrono::duration<double>& backoff)
{
  if ((mImpl->mMaxBackoff && backoff > *mImpl->mMaxBackoff) || (!mImpl->mMaxBackoff && backoff > DEFAULT_MAX_BACKOFF))
  {
    throw std::invalid_argument("Minimum backoff would be larger than maximum backoff");
  }

  mImpl->mMinBackoff = backoff;
  return *this;
}

//-----
Client& Client::setMaxBackoff(const std::chrono::duration<double>& backoff)
{
  if ((mImpl->mMinBackoff && backoff < *mImpl->mMinBackoff) || (!mImpl->mMinBackoff && backoff < DEFAULT_MIN_BACKOFF))
  {
    throw std::invalid_argument("Maximum backoff would be smaller than minimum backoff");
  }

  mImpl->mMaxBackoff = backoff;
  return *this;
}

//-----
std::optional<AccountId> Client::getOperatorAccountId() const
{
  return mImpl->mOperatorAccountId;
}

//-----
std::shared_ptr<PublicKey> Client::getOperatorPublicKey() const
{
  return mImpl->mOperatorPrivateKey ? mImpl->mOperatorPrivateKey->getPublicKey() : nullptr;
}

//-----
std::chrono::duration<double> Client::getRequestTimeout() const
{
  return mImpl->mRequestTimeout;
}

//-----
std::optional<Hbar> Client::getMaxTransactionFee() const
{
  return mImpl->mMaxTransactionFee;
}

//-----
std::optional<bool> Client::getTransactionIdRegenerationPolicy() const
{
  return mImpl->mTransactionIdRegenerationPolicy;
}

//-----
std::optional<uint32_t> Client::getMaxAttempts() const
{
  return mImpl->mMaxAttempts;
}

//-----
std::optional<std::chrono::duration<double>> Client::getMinBackoff() const
{
  return mImpl->mMinBackoff;
}

//-----
std::optional<std::chrono::duration<double>> Client::getMaxBackoff() const
{
  return mImpl->mMaxBackoff;
}

} // namespace Hedera

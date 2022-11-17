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
#include "Hbar.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "impl/Network.h"

#include <stdexcept>

namespace Hedera
{
//-----
struct Client::ClientImpl
{
  ClientImpl() = default;
  ClientImpl(const ClientImpl& other) = delete;
  ClientImpl& operator=(const ClientImpl& other) = delete;
  ClientImpl(ClientImpl&& other) noexcept = default;
  ClientImpl& operator=(ClientImpl&& other) noexcept = default;

  std::unique_ptr<internal::Network> mNetwork;
  std::unique_ptr<AccountId> mOperatorAccountId;
  std::unique_ptr<PrivateKey> mOperatorPrivateKey;
  std::unique_ptr<Hbar> mDefaultMaxTransactionFee;
  std::chrono::duration<int64_t> mRequestTimeout = std::chrono::minutes(2);
};

//-----
Client::~Client() = default;

//-----
Client Client::forTestnet()
{
  Client client;
  client.mImpl->mNetwork = std::make_unique<internal::Network>(internal::Network::forTestnet());
  return client;
}

//-----
Client& Client::setOperator(const std::shared_ptr<AccountId>& accountId, std::unique_ptr<PrivateKey>& privateKey)
{
  return setOperator(accountId, std::move(privateKey));
}

//-----
Client& Client::setOperator(const std::shared_ptr<AccountId>& accountId, std::unique_ptr<PrivateKey>&& privateKey)
{
  mImpl->mOperatorAccountId = std::make_unique<AccountId>(*accountId);
  mImpl->mOperatorPrivateKey = std::move(privateKey);

  return *this;
}

//-----
Client& Client::setDefaultMaxTransactionFee(const Hbar& defaultMaxTransactionFee)
{
  if (defaultMaxTransactionFee.toTinybars() < 0)
  {
    throw std::invalid_argument("Transaction fee cannot be negative");
  }

  mImpl->mDefaultMaxTransactionFee = std::make_unique<Hbar>(defaultMaxTransactionFee);
  return *this;
}

//-----
std::vector<unsigned char> Client::sign(const std::vector<unsigned char>& bytes) const
{
  return mImpl->mOperatorPrivateKey->sign(bytes);
}

//-----
std::vector<std::shared_ptr<internal::Node>> Client::getNodesWithAccountIds(
  const std::vector<std::shared_ptr<AccountId>>& accountIds) const
{
  return mImpl->mNetwork->getNodesWithAccountIds(accountIds);
}

void Client::close() const
{
  mImpl->mNetwork->close();
}

//-----
std::unique_ptr<AccountId> Client::getOperatorAccountId() const
{
  return mImpl->mOperatorAccountId ? std::make_unique<AccountId>(*mImpl->mOperatorAccountId) : nullptr;
}

//-----
std::unique_ptr<PublicKey> Client::getOperatorPublicKey() const
{
  return mImpl->mOperatorPrivateKey ? mImpl->mOperatorPrivateKey->getPublicKey()->clone() : nullptr;
}

//-----
std::unique_ptr<Hbar> Client::getDefaultMaxTransactionFee() const
{
  return mImpl->mDefaultMaxTransactionFee ? std::make_unique<Hbar>(*mImpl->mDefaultMaxTransactionFee) : nullptr;
}

//-----
std::chrono::duration<int64_t> Client::getRequestTimeout() const
{
  return mImpl->mRequestTimeout;
}

//-----
Client::Client()
  : mImpl(std::make_unique<ClientImpl>())
{
}

} // namespace Hedera

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
#include "Client.h"

#include <stdexcept>

namespace Hedera
{
//-----
Client Client::forTestnet()
{
  Client client;
  client.mNetwork = std::make_shared<Network>(Network::forTestnet());
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
  mOperator = std::make_shared<Operator>();
  mOperator->mAccountId = accountId;
  mOperator->mPrivateKey = std::move(privateKey);

  return *this;
}

//-----
Client& Client::setDefaultMaxTransactionFee(const Hbar& defaultMaxTransactionFee)
{
  if (defaultMaxTransactionFee.toTinybars() < 0)
  {
    throw std::invalid_argument("Transaction fee cannot be negative");
  }

  mDefaultMaxTransactionFee = std::make_unique<Hbar>(defaultMaxTransactionFee);
  return *this;
}

void Client::close()
{
  mNetwork->close();
}

//-----
[[nodiscard]] std::shared_ptr<AccountId> Client::getOperatorAccountId() const
{
  if (!mOperator)
  {
    throw std::runtime_error("Operator has not yet been set");
  }

  return mOperator->mAccountId;
}

//-----
[[nodiscard]] std::shared_ptr<PublicKey> Client::getOperatorPublicKey() const
{
  if (!mOperator)
  {
    throw std::runtime_error("Operator has not yet been set");
  }

  return mOperator->mPrivateKey->getPublicKey();
}

} // namespace Hedera

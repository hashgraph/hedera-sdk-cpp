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
#include "AccountDeleteTransaction.h"

/**
 * Protobuf includes
 */
#include "crypto_delete.pb.h"

namespace Hedera
{
//-----
AccountDeleteTransaction::AccountDeleteTransaction()
  : Transaction()
  , mAccountId()
  , mTransferAccountId()
{
}

//-----
AccountDeleteTransaction::AccountDeleteTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//-----
AccountDeleteTransaction::AccountDeleteTransaction(
  const proto::TransactionBody& transaction)
  : Transaction(transaction)
{
  initFromTransactionBody();
}

//-----
void
AccountDeleteTransaction::validateChecksums(const Client& client) const
{
  if (mAccountId.isValid())
  {
    mAccountId.getValue().validateChecksum(client);
  }

  if (mTransferAccountId.isValid())
  {
    mTransferAccountId.getValue().validateChecksum(client);
  }
}

//-----
AccountDeleteTransaction&
AccountDeleteTransaction::setAccountId(const AccountId& accountId)
{
  requireNotFrozen();

  mAccountId.setValue(accountId);
  return *this;
}

//-----
AccountDeleteTransaction&
AccountDeleteTransaction::setTransferAccountId(const AccountId& accountId)
{
  requireNotFrozen();

  mTransferAccountId.setValue(accountId);
  return *this;
}

//-----
void
AccountDeleteTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_cryptodelete())
  {
    const proto::CryptoDeleteTransactionBody& body =
      mSourceTransactionBody.cryptodelete();

    if (body.has_deleteaccountid())
    {
      mAccountId.setValue(AccountId::fromProtobuf(body.deleteaccountid()));
    }

    if (body.has_transferaccountid())
    {
      mTransferAccountId.setValue(
        AccountId::fromProtobuf(body.transferaccountid()));
    }
  }
}

} // namespace Hedera
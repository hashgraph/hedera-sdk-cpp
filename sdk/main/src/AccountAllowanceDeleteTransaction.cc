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
#include "AccountAllowanceDeleteTransaction.h"

#include "AccountId.h"
#include "NftAllowance.h"
#include "NftId.h"
#include "TokenId.h"
#include "helper/InitType.h"

#include "crypto_delete_allowance.pb.h"
#include "schedulable_transaction_body.pb.h"
#include "transaction_body.pb.h"

namespace Hedera
{
//-----
AccountAllowanceDeleteTransaction::AccountAllowanceDeleteTransaction()
  : mNftAllowances()
  , mNftMap()
{
}

//-----
AccountAllowanceDeleteTransaction::AccountAllowanceDeleteTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//----
AccountAllowanceDeleteTransaction::AccountAllowanceDeleteTransaction(
  const proto::TransactionBody& transaction)
  : Transaction(transaction)
{
  initFromTransactionBody();
}

//-----
void
AccountAllowanceDeleteTransaction::validateChecksums(const Client& client) const
{
  for (size_t i = 0; i < mNftAllowances.size(); ++i)
  {
    mNftAllowances.at(i).validateChecksums(client);
  }
}

//-----
void
AccountAllowanceDeleteTransaction::onFreeze(proto::TransactionBody* body) const
{
  body->set_allocated_cryptodeleteallowance(build());
}

//-----
void
AccountAllowanceDeleteTransaction::onScheduled(
  proto::SchedulableTransactionBody* body) const
{
  body->set_allocated_cryptodeleteallowance(build());
}

//-----
proto::CryptoDeleteAllowanceTransactionBody*
AccountAllowanceDeleteTransaction::build() const
{
  proto::CryptoDeleteAllowanceTransactionBody* body =
    new proto::CryptoDeleteAllowanceTransactionBody;

  for (size_t i = 0; i < mNftAllowances.size(); ++i)
  {
    proto::NftRemoveAllowance* allow = body->add_nftallowances();
    (void)allow;
    // TODO: fill out nft removal allowance
  }

  return body;
}

//-----
AccountAllowanceDeleteTransaction&
AccountAllowanceDeleteTransaction::deleteAllNftAllowances(
  const NftId& nftId,
  const AccountId& ownerAccountId)
{
  requireNotFrozen();

  saveNftSerial(nftId.mSerial, nftId.mTokenId, ownerAccountId);

  return *this;
}

//-----
void
AccountAllowanceDeleteTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_cryptodeleteallowance())
  {
    const proto::CryptoDeleteAllowanceTransactionBody& body =
      mSourceTransactionBody.cryptodeleteallowance();

    for (int i = 0; i < body.nftallowances_size(); ++i)
    {
      const proto::NftRemoveAllowance& nft = body.nftallowances(i);
      if (!nft.has_token_id() || !nft.has_owner())
      {
        // TODO: throw
      }

      const TokenId tokenId = TokenId::fromProtobuf(nft.token_id());
      const AccountId owner = AccountId::fromProtobuf(nft.owner());

      for (int i = 0; i < nft.serial_numbers_size(); ++i)
      {
        saveNftSerial(nft.serial_numbers(i), tokenId, owner);
      }
    }
  }
}

//-----
void
AccountAllowanceDeleteTransaction::saveNftSerial(
  const int64_t& serial,
  const TokenId& tokenId,
  const AccountId& ownerAccountId)
{
  const std::string key = ownerAccountId.toString() + ':' + tokenId.toString();

  if (mNftMap.find(key) != mNftMap.end())
  {
    mNftAllowances.at(mNftMap.at(key)).mSerialNumbers.push_back(serial);
  }
  else
  {
    mNftMap.insert({ key, mNftAllowances.size() });
    mNftAllowances.push_back(NftAllowance(InitType<TokenId>(tokenId),
                                          InitType<AccountId>(ownerAccountId),
                                          InitType<AccountId>(),
                                          std::vector<int64_t>(),
                                          false));
  }
}

} // namespace Hedera
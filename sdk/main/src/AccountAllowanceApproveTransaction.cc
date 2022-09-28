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
#include "AccountAllowanceApproveTransaction.h"

#include "AccountId.h"
#include "Hbar.h"
#include "NftId.h"

#include "helper/InitType.h"

#include "basic_types.pb.h"
#include "crypto_approve_allowance.pb.h"
#include "schedulable_transaction_body.pb.h"
#include "transaction_body.pb.h"

namespace Hedera
{
//-----
AccountAllowanceApproveTransaction::AccountAllowanceApproveTransaction()
  : mHbarAllowances()
  , mTokenAllowances()
  , mNftAllowances()
  , mNftMap()
{
}

//-----
AccountAllowanceApproveTransaction::AccountAllowanceApproveTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//----
AccountAllowanceApproveTransaction::AccountAllowanceApproveTransaction(
  const proto::TransactionBody& transaction)
  : Transaction(transaction)
{
  initFromTransactionBody();
}

//-----
void
AccountAllowanceApproveTransaction::validateChecksums(
  const Client& client) const
{
  for (size_t i = 0; i < mHbarAllowances.size(); ++i)
  {
    mHbarAllowances.at(i).validateChecksums(client);
  }

  for (size_t i = 0; i < mNftAllowances.size(); ++i)
  {
    mNftAllowances.at(i).validateChecksums(client);
  }

  for (size_t i = 0; i < mTokenAllowances.size(); ++i)
  {
    mTokenAllowances.at(i).validateChecksums(client);
  }
}

//-----
void
AccountAllowanceApproveTransaction::onFreeze(proto::TransactionBody* body) const
{
  body->set_allocated_cryptoapproveallowance(build());
}

//-----
void
AccountAllowanceApproveTransaction::onScheduled(
  proto::SchedulableTransactionBody* body) const
{
  body->set_allocated_cryptoapproveallowance(build());
}

//----
proto::CryptoApproveAllowanceTransactionBody*
AccountAllowanceApproveTransaction::build() const
{
  proto::CryptoApproveAllowanceTransactionBody* body =
    new proto::CryptoApproveAllowanceTransactionBody;

  for (size_t i = 0; i < mHbarAllowances.size(); ++i)
  {
    proto::CryptoAllowance* allow = body->add_cryptoallowances();
    (void)allow;
    //  TODO: fill out crypto fields
  }

  for (size_t i = 0; i < mNftAllowances.size(); ++i)
  {
    proto::NftAllowance* allow = body->add_nftallowances();
    (void)allow;
    //  TODO: fill out nft fields
  }

  for (size_t i = 0; i < mTokenAllowances.size(); ++i)
  {
    proto::TokenAllowance* allow = body->add_tokenallowances();
    (void)allow;
    //  TODO: fill out token fields
  }

  return body;
}

//-----
AccountAllowanceApproveTransaction&
AccountAllowanceApproveTransaction::approveHbarAllowance(
  const InitType<AccountId>& ownerAccountId,
  const AccountId& spenderAccountId,
  const Hbar& amount)
{
  requireNotFrozen();

  if (amount.toTinybars() < 0)
  {
    // TODO: throw
  }

  mHbarAllowances.push_back(
    HbarAllowance(ownerAccountId, spenderAccountId, amount));

  return *this;
}

//-----
AccountAllowanceApproveTransaction&
AccountAllowanceApproveTransaction::approveNftAllowance(
  const NftId& nftId,
  const InitType<AccountId>& ownerAccountId,
  const AccountId& spenderAccountId)
{
  requireNotFrozen();

  saveNftSerial(
    nftId.mSerial, nftId.mTokenId, ownerAccountId, spenderAccountId);

  return *this;
}

//-----
AccountAllowanceApproveTransaction&
AccountAllowanceApproveTransaction::approveNftAllowanceAllSerials(
  const TokenId& tokenId,
  const InitType<AccountId>& ownerAccountId,
  const AccountId& spenderAccountId)
{
  requireNotFrozen();

  saveAllNftSerials(tokenId, ownerAccountId, spenderAccountId);

  return *this;
}

//-----
AccountAllowanceApproveTransaction&
AccountAllowanceApproveTransaction::approveTokenAllowance(
  const TokenId& tokenId,
  const InitType<AccountId>& ownerAccountId,
  const AccountId& spenderAccountId,
  const int64_t& amount)
{
  requireNotFrozen();

  if (amount < 0)
  {
    // TODO: throw
  }

  mTokenAllowances.push_back(
    TokenAllowance(tokenId, ownerAccountId, spenderAccountId, amount));

  return *this;
}

//-----
void
AccountAllowanceApproveTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_cryptoapproveallowance())
  {
    const proto::CryptoApproveAllowanceTransactionBody& body =
      mSourceTransactionBody.cryptoapproveallowance();

    for (int i = 0; i < body.cryptoallowances_size(); ++i)
    {
      mHbarAllowances.push_back(
        HbarAllowance::fromProtobuf(body.cryptoallowances(i)));
    }

    for (int i = 0; i < body.nftallowances_size(); ++i)
    {
      const proto::NftAllowance& nft = body.nftallowances(i);
      if (!nft.has_tokenid() || !nft.has_spender())
      {
        // TODO: throw
      }

      const TokenId tokenId = TokenId::fromProtobuf(nft.tokenid());
      const InitType<AccountId> owner =
        (nft.has_owner())
          ? InitType<AccountId>(AccountId::fromProtobuf(nft.owner()))
          : InitType<AccountId>();
      const AccountId spender = AccountId::fromProtobuf(nft.spender());

      if (nft.has_approved_for_all() && nft.approved_for_all().value())
      {
        saveAllNftSerials(tokenId, owner, spender);
      }
      else
      {
        for (int i = 0; i < nft.serial_numbers_size(); ++i)
        {
          saveNftSerial(nft.serial_numbers(i), tokenId, owner, spender);
        }
      }
    }

    for (int i = 0; i < body.tokenallowances_size(); ++i)
    {
      mTokenAllowances.push_back(
        TokenAllowance::fromProtobuf(body.tokenallowances(i)));
    }
  }
}

//-----
std::vector<int64_t>
AccountAllowanceApproveTransaction::getNftSerials(
  const TokenId& tokenId,
  const InitType<AccountId>& ownerAccountId,
  const AccountId& spenderAccountId)
{
  const std::string key =
    getNftMapKey(ownerAccountId, spenderAccountId, tokenId);

  if (mNftMap.find(key) != mNftMap.end())
  {
    return mNftAllowances.at(mNftMap.at(key)).mSerialNumbers;
  }

  return std::vector<int64_t>();
}

//-----
void
AccountAllowanceApproveTransaction::saveNftSerial(
  const int64_t& serial,
  const TokenId& tokenId,
  const InitType<AccountId>& ownerAccountId,
  const AccountId& spenderAccountId)
{
  const std::string key =
    getNftMapKey(ownerAccountId, spenderAccountId, tokenId);

  if (mNftMap.find(key) != mNftMap.end())
  {
    mNftAllowances.at(mNftMap.at(key)).mSerialNumbers.push_back(serial);
  }
  else
  {
    mNftMap.insert({ key, mNftAllowances.size() });
    mNftAllowances.push_back(NftAllowance(InitType<TokenId>(tokenId),
                                          ownerAccountId,
                                          InitType<AccountId>(spenderAccountId),
                                          std::vector<int64_t>({ serial }),
                                          false));
  }
}

//-----
void
AccountAllowanceApproveTransaction::saveAllNftSerials(
  const TokenId& tokenId,
  const InitType<AccountId>& ownerAccountId,
  const AccountId& spenderAccountId)
{
  const std::string key =
    getNftMapKey(ownerAccountId, spenderAccountId, tokenId);

  if (mNftMap.find(key) != mNftMap.end())
  {
    mNftAllowances.at(mNftMap.at(key)).allSerials = true;
  }
  else
  {
    mNftMap.insert({ key, mNftAllowances.size() });
    mNftAllowances.push_back(NftAllowance(InitType<TokenId>(tokenId),
                                          ownerAccountId,
                                          InitType<AccountId>(spenderAccountId),
                                          std::vector<int64_t>(),
                                          true));
  }
}

//----
std::string
AccountAllowanceApproveTransaction::getNftMapKey(
  const InitType<AccountId>& ownerAccountId,
  const AccountId& spenderAccountId,
  const TokenId& tokenId) const
{
  return ((ownerAccountId.isValid()) ? ownerAccountId.getValue().toString()
                                     : "FEE_PAYER") +
         ':' + spenderAccountId.toString() + ':' + tokenId.toString();
}

} // namespace Hedera
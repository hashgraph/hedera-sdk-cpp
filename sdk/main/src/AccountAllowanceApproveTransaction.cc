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
#include "AccountAllowanceApproveTransaction.h"
#include "NftId.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/crypto_approve_allowance.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_response.pb.h>
#include <stdexcept>

namespace Hedera
{

//-----
AccountAllowanceApproveTransaction::AccountAllowanceApproveTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<AccountAllowanceApproveTransaction>(transactionBody)
{
  if (!transactionBody.has_cryptoapproveallowance())
  {
    throw std::invalid_argument("Transaction body doesn't contain CryptoApproveAllowance data");
  }

  const proto::CryptoApproveAllowanceTransactionBody& body = transactionBody.cryptoapproveallowance();

  for (int i = 0; i < body.cryptoallowances_size(); ++i)
  {
    mHbarAllowances.push_back(HbarAllowance::fromProtobuf(body.cryptoallowances(i)));
  }

  for (int i = 0; i < body.tokenallowances_size(); ++i)
  {
    mTokenAllowances.push_back(TokenAllowance::fromProtobuf(body.tokenallowances(i)));
  }

  for (int i = 0; i < body.nftallowances_size(); ++i)
  {
    mNftAllowances.push_back(TokenNftAllowance::fromProtobuf(body.nftallowances(i)));
  }
}

//-----
AccountAllowanceApproveTransaction& AccountAllowanceApproveTransaction::approveHbarAllowance(
  const AccountId& ownerAccountId,
  const AccountId& spenderAccountId,
  const Hbar& amount)
{
  requireNotFrozen();

  if (amount.toTinybars() < 0LL)
  {
    throw std::invalid_argument("Can't approve a negative allowance");
  }

  mHbarAllowances.emplace_back(ownerAccountId, spenderAccountId, amount);
  return *this;
}

//-----
AccountAllowanceApproveTransaction& AccountAllowanceApproveTransaction::approveTokenAllowance(
  const TokenId& tokenId,
  const AccountId& ownerAccountId,
  const AccountId& spenderAccountId,
  const uint64_t& amount)
{
  requireNotFrozen();
  mTokenAllowances.emplace_back(tokenId, ownerAccountId, spenderAccountId, amount);
  return *this;
}

//-----
AccountAllowanceApproveTransaction& AccountAllowanceApproveTransaction::approveTokenNftAllowance(
  const NftId& nftId,
  const AccountId& ownerAccountId,
  const AccountId& spenderAccountId)
{
  requireNotFrozen();

  // Add the serial number to the token allowance if there's already an allowance for this token ID, owner, and spender.
  for (TokenNftAllowance& allowance : mNftAllowances)
  {
    if (allowance.getTokenId() == nftId.getTokenId() && allowance.getOwnerAccountId() == ownerAccountId &&
        allowance.getSpenderAccountId() == spenderAccountId)
    {
      allowance.addSerialNumber(nftId.getSerialNum());
      return *this;
    }
  }

  mNftAllowances.emplace_back(
    nftId.getTokenId(), ownerAccountId, spenderAccountId, std::vector<uint64_t>{ nftId.getSerialNum() });
  return *this;
}

//-----
AccountAllowanceApproveTransaction& AccountAllowanceApproveTransaction::approveNftAllowanceAllSerials(
  const TokenId& tokenId,
  const AccountId& ownerAccountId,
  const AccountId& spenderAccountId)
{
  requireNotFrozen();

  for (TokenNftAllowance& allowance : mNftAllowances)
  {
    if (allowance.getTokenId() == tokenId && allowance.getOwnerAccountId() == ownerAccountId &&
        allowance.getSpenderAccountId() == spenderAccountId)
    {
      allowance.approveForAll(true);
      return *this;
    }
  }

  mNftAllowances.emplace_back(tokenId, ownerAccountId, spenderAccountId, std::vector<uint64_t>{}, true);
  return *this;
}

//-----
proto::Transaction AccountAllowanceApproveTransaction::makeRequest(const Client& client,
                                                                   const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody transactionBody = generateTransactionBody(client);
  transactionBody.set_allocated_cryptoapproveallowance(build());

  return signTransaction(transactionBody, client);
}

//-----
grpc::Status AccountAllowanceApproveTransaction::submitRequest(const Client& client,
                                                               const std::chrono::system_clock::time_point& deadline,
                                                               const std::shared_ptr<internal::Node>& node,
                                                               proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kCryptoApproveAllowance, makeRequest(client, node), deadline, response);
}

//-----
proto::CryptoApproveAllowanceTransactionBody* AccountAllowanceApproveTransaction::build() const
{
  auto body = std::make_unique<proto::CryptoApproveAllowanceTransactionBody>();

  for (const HbarAllowance& allowance : mHbarAllowances)
  {
    proto::CryptoAllowance* cryptoAllowance = body->add_cryptoallowances();
    cryptoAllowance->set_allocated_owner(allowance.getOwnerAccountId().toProtobuf().release());
    cryptoAllowance->set_allocated_spender(allowance.getSpenderAccountId().toProtobuf().release());
    cryptoAllowance->set_amount(allowance.getAmount().toTinybars());
  }

  for (const TokenAllowance& allowance : mTokenAllowances)
  {
    proto::TokenAllowance* tokenAllowance = body->add_tokenallowances();
    tokenAllowance->set_allocated_tokenid(allowance.getTokenId().toProtobuf().release());
    tokenAllowance->set_allocated_owner(allowance.getOwnerAccountId().toProtobuf().release());
    tokenAllowance->set_allocated_spender(allowance.getSpenderAccountId().toProtobuf().release());
    tokenAllowance->set_amount(static_cast<int64_t>(allowance.getAmount()));
  }

  for (const TokenNftAllowance& allowance : mNftAllowances)
  {
    proto::NftAllowance* nftAllowance = body->add_nftallowances();
    nftAllowance->set_allocated_tokenid(allowance.getTokenId().toProtobuf().release());
    nftAllowance->set_allocated_owner(allowance.getOwnerAccountId().toProtobuf().release());
    nftAllowance->set_allocated_spender(allowance.getSpenderAccountId().toProtobuf().release());

    for (const uint64_t& serialNumber : allowance.getSerialNumbers())
    {
      nftAllowance->add_serial_numbers(static_cast<int64_t>(serialNumber));
    }

    if (allowance.getApprovedForAll().has_value())
    {
      auto value = std::make_unique<google::protobuf::BoolValue>();
      value->set_value(*allowance.getApprovedForAll());
      nftAllowance->set_allocated_approved_for_all(value.release());
    }

    if (allowance.getDelegateSpender().has_value())
    {
      nftAllowance->set_allocated_delegating_spender(allowance.getDelegateSpender()->toProtobuf().release());
    }
  }

  return body.release();
}

} // namespace Hedera

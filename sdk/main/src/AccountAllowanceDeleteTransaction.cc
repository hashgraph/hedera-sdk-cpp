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
#include "AccountAllowanceDeleteTransaction.h"
#include "NftId.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/crypto_delete_allowance.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_response.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
AccountAllowanceDeleteTransaction::AccountAllowanceDeleteTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<AccountAllowanceDeleteTransaction>(transactionBody)
{
  if (!transactionBody.has_cryptodeleteallowance())
  {
    throw std::invalid_argument("Transaction body doesn't contain CryptoDeleteAllowance data");
  }

  const proto::CryptoDeleteAllowanceTransactionBody& body = transactionBody.cryptodeleteallowance();

  for (int i = 0; i < body.nftallowances_size(); ++i)
  {
    std::vector<uint64_t> serialNumbers;
    serialNumbers.reserve(body.nftallowances(i).serial_numbers_size());

    for (int j = 0; j < body.nftallowances(i).serial_numbers_size(); ++j)
    {
      serialNumbers.push_back(static_cast<uint64_t>(body.nftallowances(i).serial_numbers(j)));
    }

    mNftAllowanceDeletions.emplace_back(TokenId::fromProtobuf(body.nftallowances(i).token_id()),
                                        AccountId::fromProtobuf(body.nftallowances(i).owner()),
                                        std::optional<AccountId>(),
                                        serialNumbers);
  }
}

//-----
AccountAllowanceDeleteTransaction& AccountAllowanceDeleteTransaction::deleteAllTokenNftAllowances(
  const NftId& nftId,
  const AccountId& owner)
{
  requireNotFrozen();

  // Add the serial number to the token allowance if there's already an allowance for this token ID, owner, and spender.
  for (TokenNftAllowance& allowance : mNftAllowanceDeletions)
  {
    if (allowance.mTokenId == nftId.getTokenId() && allowance.mOwnerAccountId == owner)
    {
      allowance.mSerialNumbers.push_back(nftId.getSerialNum());
      return *this;
    }
  }

  mNftAllowanceDeletions.emplace_back(
    nftId.getTokenId(), owner, std::optional<AccountId>(), std::vector<uint64_t>{ nftId.getSerialNum() });
  return *this;
}

//-----
proto::Transaction AccountAllowanceDeleteTransaction::makeRequest(const Client& client,
                                                                  const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody txBody = generateTransactionBody(&client);
  addToBody(txBody);
  return signTransaction(txBody, client);
}

//-----
grpc::Status AccountAllowanceDeleteTransaction::submitRequest(const Client& client,
                                                              const std::chrono::system_clock::time_point& deadline,
                                                              const std::shared_ptr<internal::Node>& node,
                                                              proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kCryptoDeleteAllowance, makeRequest(client, node), deadline, response);
}

//-----
void AccountAllowanceDeleteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_cryptodeleteallowance(build());
}

//-----
proto::CryptoDeleteAllowanceTransactionBody* AccountAllowanceDeleteTransaction::build() const
{
  auto body = std::make_unique<proto::CryptoDeleteAllowanceTransactionBody>();

  for (const TokenNftAllowance& allowance : mNftAllowanceDeletions)
  {
    proto::NftRemoveAllowance* nftRemoveAllowance = body->add_nftallowances();

    if (allowance.mTokenId.has_value())
    {
      nftRemoveAllowance->set_allocated_token_id(allowance.mTokenId->toProtobuf().release());
    }

    if (allowance.mOwnerAccountId.has_value())
    {
      nftRemoveAllowance->set_allocated_owner(allowance.mOwnerAccountId->toProtobuf().release());
    }

    for (const uint64_t& num : allowance.mSerialNumbers)
    {
      nftRemoveAllowance->add_serial_numbers(static_cast<int64_t>(num));
    }
  }

  return body.release();
}

} // namespace Hedera

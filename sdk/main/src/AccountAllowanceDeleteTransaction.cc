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
    mNftAllowanceDeletions.push_back(TokenNftRemoveAllowance::fromProtobuf(body.nftallowances(i)));
  }
}

//-----
AccountAllowanceDeleteTransaction& AccountAllowanceDeleteTransaction::deleteAllTokenNftAllowances(
  const NftId& nftId,
  const AccountId& owner)
{
  requireNotFrozen();

  // Add the serial number to the token allowance if there's already an allowance for this token ID, owner, and spender.
  for (TokenNftRemoveAllowance& allowance : mNftAllowanceDeletions)
  {
    if (allowance.getTokenId() == nftId.getTokenId() && allowance.getOwnerAccountId() == owner)
    {
      allowance.addSerialNumber(nftId.getSerialNum());
      return *this;
    }
  }

  mNftAllowanceDeletions.emplace_back(nftId.getTokenId(), owner, std::vector<uint64_t>{ nftId.getSerialNum() });
  return *this;
}

//-----
proto::Transaction AccountAllowanceDeleteTransaction::makeRequest(const Client& client,
                                                                  const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody transactionBody = generateTransactionBody(client);
  transactionBody.set_allocated_cryptodeleteallowance(build());
  return signTransaction(transactionBody, client);
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
proto::CryptoDeleteAllowanceTransactionBody* AccountAllowanceDeleteTransaction::build() const
{
  auto body = std::make_unique<proto::CryptoDeleteAllowanceTransactionBody>();

  for (const TokenNftRemoveAllowance& allowance : mNftAllowanceDeletions)
  {
    proto::NftRemoveAllowance* nftRemoveAllowance = body->add_nftallowances();
    nftRemoveAllowance->set_allocated_token_id(allowance.getTokenId().toProtobuf().release());
    nftRemoveAllowance->set_allocated_owner(allowance.getOwnerAccountId().toProtobuf().release());

    for (const uint64_t& num : allowance.getSerialNumbers())
    {
      nftRemoveAllowance->add_serial_numbers(static_cast<int64_t>(num));
    }
  }

  return body.release();
}

} // namespace Hedera

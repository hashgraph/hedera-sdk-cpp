/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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

#include <crypto_approve_allowance.pb.h>
#include <grpcpp/client_context.h>
#include <stdexcept>
#include <transaction.pb.h>
#include <transaction_response.pb.h>

namespace Hiero
{
//-----
AccountAllowanceApproveTransaction::AccountAllowanceApproveTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<AccountAllowanceApproveTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
AccountAllowanceApproveTransaction::AccountAllowanceApproveTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<AccountAllowanceApproveTransaction>(transactions)
{
  initFromSourceTransactionBody();
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
  const AccountId& spenderAccountId,
  const AccountId& delegatingAccountId)
{
  requireNotFrozen();

  // Add the serial number to the token allowance if there's already an allowance for this token ID, owner, and spender.
  for (TokenNftAllowance& allowance : mNftAllowances)
  {
    if (allowance.mTokenId == nftId.mTokenId && allowance.mOwnerAccountId == ownerAccountId &&
        allowance.mSpenderAccountId == spenderAccountId)
    {
      allowance.mSerialNumbers.push_back(nftId.mSerialNum);
      return *this;
    }
  }

  mNftAllowances.emplace_back(nftId.mTokenId,
                              ownerAccountId,
                              spenderAccountId,
                              std::vector<uint64_t>{ nftId.mSerialNum },
                              std::optional<bool>(),
                              (delegatingAccountId == AccountId()) ? std::optional<AccountId>() : delegatingAccountId);
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
    if (allowance.mTokenId == tokenId && allowance.mOwnerAccountId == ownerAccountId &&
        allowance.mSpenderAccountId == spenderAccountId)
    {
      allowance.mApprovedForAll = true;
      return *this;
    }
  }

  mNftAllowances.emplace_back(tokenId, ownerAccountId, spenderAccountId, std::vector<uint64_t>{}, true);
  return *this;
}

//-----
AccountAllowanceApproveTransaction& AccountAllowanceApproveTransaction::deleteNftAllowanceAllSerials(
  const TokenId& tokenId,
  const AccountId& ownerAccountId,
  const AccountId& spenderAccountId)
{
  requireNotFrozen();
  mNftAllowances.emplace_back(tokenId, ownerAccountId, spenderAccountId, std::vector<uint64_t>{}, false);
  return *this;
}

//-----
grpc::Status AccountAllowanceApproveTransaction::submitRequest(const proto::Transaction& request,
                                                               const std::shared_ptr<internal::Node>& node,
                                                               const std::chrono::system_clock::time_point& deadline,
                                                               proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kCryptoApproveAllowance, request, deadline, response);
}

//-----
void AccountAllowanceApproveTransaction::validateChecksums(const Client& client) const
{
  std::for_each(mHbarAllowances.cbegin(),
                mHbarAllowances.cend(),
                [&client](const HbarAllowance& allowance) { allowance.validateChecksums(client); });
  std::for_each(mTokenAllowances.cbegin(),
                mTokenAllowances.cend(),
                [&client](const TokenAllowance& allowance) { allowance.validateChecksums(client); });
  std::for_each(mNftAllowances.cbegin(),
                mNftAllowances.cend(),
                [&client](const TokenNftAllowance& allowance) { allowance.validateChecksums(client); });
}

//-----
void AccountAllowanceApproveTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_cryptoapproveallowance(build());
}

//-----
void AccountAllowanceApproveTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

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
proto::CryptoApproveAllowanceTransactionBody* AccountAllowanceApproveTransaction::build() const
{
  auto body = std::make_unique<proto::CryptoApproveAllowanceTransactionBody>();

  for (const HbarAllowance& allowance : mHbarAllowances)
  {
    *body->add_cryptoallowances() = *allowance.toProtobuf();
  }

  for (const TokenAllowance& allowance : mTokenAllowances)
  {
    *body->add_tokenallowances() = *allowance.toProtobuf();
  }

  for (const TokenNftAllowance& allowance : mNftAllowances)
  {
    *body->add_nftallowances() = *allowance.toProtobuf();
  }

  return body.release();
}

} // namespace Hiero

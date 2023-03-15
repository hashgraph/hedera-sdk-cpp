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
#include "AccountCreateTransaction.h"
#include "TransactionResponse.h"
#include "impl/DurationConverter.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/crypto_create.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_response.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
AccountCreateTransaction::AccountCreateTransaction()
  : Transaction<AccountCreateTransaction>()
{
  setMaxTransactionFee(Hbar(5LL));
}

//-----
AccountCreateTransaction::AccountCreateTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<AccountCreateTransaction>(transactionBody)
{
  if (!transactionBody.has_cryptocreateaccount())
  {
    throw std::invalid_argument("Transaction body doesn't contain CryptoCreateAccount data");
  }

  const proto::CryptoCreateTransactionBody& body = transactionBody.cryptocreateaccount();

  if (body.has_key())
  {
    mKey = PublicKey::fromProtobuf(body.key());
  }

  mInitialBalance = Hbar(static_cast<int64_t>(body.initialbalance()), HbarUnit::TINYBAR());
  mReceiverSignatureRequired = body.receiversigrequired();

  if (body.has_autorenewperiod())
  {
    mAutoRenewPeriod = internal::DurationConverter::fromProtobuf(body.autorenewperiod());
  }

  mAccountMemo = body.memo();
  mMaxAutomaticTokenAssociations = static_cast<uint32_t>(body.max_automatic_token_associations());

  if (body.has_staked_account_id())
  {
    mStakedAccountId = AccountId::fromProtobuf(body.staked_account_id());
  }

  if (body.has_staked_node_id())
  {
    mStakedNodeId = static_cast<uint64_t>(body.staked_node_id());
  }

  mDeclineStakingReward = body.decline_reward();

  if (!body.alias().empty())
  {
    mAlias = PublicKey::fromBytesDer({ body.alias().cbegin(), body.alias().cend() });
  }

  if (!body.evm_address().empty())
  {
    mEvmAddress = EvmAddress::fromBytes({ body.evm_address().cbegin(), body.evm_address().cend() });
  }
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setKey(const std::shared_ptr<PublicKey>& key)
{
  requireNotFrozen();

  mKey = key;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setInitialBalance(const Hbar& initialBalance)
{
  requireNotFrozen();

  mInitialBalance = initialBalance;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setReceiverSignatureRequired(bool receiveSignatureRequired)
{
  requireNotFrozen();

  mReceiverSignatureRequired = receiveSignatureRequired;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setAutoRenewPeriod(
  const std::chrono::duration<double>& autoRenewPeriod)
{
  requireNotFrozen();

  mAutoRenewPeriod = autoRenewPeriod;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setAccountMemo(std::string_view memo)
{
  requireNotFrozen();

  if (memo.size() > 100)
  {
    throw std::length_error("Account memo is too large. Must be smaller than 100 bytes");
  }

  mAccountMemo = memo;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setMaxAutomaticTokenAssociations(uint32_t associations)
{
  requireNotFrozen();

  if (associations > 5000U)
  {
    throw std::invalid_argument("Too many maximum number of token associations. Maximum can't be over 5000");
  }

  mMaxAutomaticTokenAssociations = associations;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setStakedAccountId(const AccountId& stakedAccountId)
{
  requireNotFrozen();

  mStakedAccountId = stakedAccountId;
  mStakedNodeId.reset();
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setStakedNodeId(const uint64_t& stakedNodeId)
{
  requireNotFrozen();

  mStakedNodeId = stakedNodeId;
  mStakedAccountId.reset();
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setDeclineStakingReward(bool declineReward)
{
  requireNotFrozen();

  mDeclineStakingReward = declineReward;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setAlias(const std::shared_ptr<PublicKey>& alias)
{
  requireNotFrozen();

  mAlias = alias;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setEvmAddress(const EvmAddress& address)
{
  requireNotFrozen();

  mEvmAddress = address;
  return *this;
}

//-----
proto::Transaction AccountCreateTransaction::makeRequest(const Client& client,
                                                         const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody transactionBody = generateTransactionBody(client);
  transactionBody.set_allocated_cryptocreateaccount(build());

  return signTransaction(transactionBody, client);
}

//-----
grpc::Status AccountCreateTransaction::submitRequest(const Client& client,
                                                     const std::chrono::system_clock::time_point& deadline,
                                                     const std::shared_ptr<internal::Node>& node,
                                                     proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kCryptoCreateAccount, makeRequest(client, node), deadline, response);
}

//-----
proto::CryptoCreateTransactionBody* AccountCreateTransaction::build() const
{
  auto body = std::make_unique<proto::CryptoCreateTransactionBody>();

  if (mKey)
  {
    body->set_allocated_key(mKey->toProtobuf().release());
  }

  body->set_initialbalance(mInitialBalance.toTinybars());
  body->set_receiversigrequired(mReceiverSignatureRequired);
  body->set_allocated_autorenewperiod(
    internal::DurationConverter::toProtobuf(std::chrono::duration_cast<std::chrono::seconds>(mAutoRenewPeriod)));
  body->set_memo(mAccountMemo);
  body->set_max_automatic_token_associations(static_cast<int32_t>(mMaxAutomaticTokenAssociations));

  if (mStakedAccountId)
  {
    body->set_allocated_staked_account_id(mStakedAccountId->toProtobuf().release());
  }

  if (mStakedNodeId)
  {
    body->set_staked_node_id(static_cast<int64_t>(*mStakedNodeId));
  }

  body->set_decline_reward(mDeclineStakingReward);

  if (mAlias)
  {
    const std::vector<unsigned char> aliasBytes = mAlias->toBytesDer();
    body->set_allocated_alias(new std::string{ aliasBytes.cbegin(), aliasBytes.cend() });
  }

  if (mEvmAddress)
  {
    const std::vector<unsigned char> addressBytes = mEvmAddress->toBytes();
    body->set_allocated_evm_address(new std::string{ addressBytes.cbegin(), addressBytes.cend() });
  }

  return body.release();
}

} // namespace Hedera

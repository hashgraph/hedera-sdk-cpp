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
#include "ContractCreateTransaction.h"
#include "Client.h"
#include "impl/DurationConverter.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <grpcpp/client_context.h>
#include <proto/contract_create.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_response.pb.h>

namespace Hedera
{
//-----
ContractCreateTransaction::ContractCreateTransaction()
  : Transaction<ContractCreateTransaction>()
{
  setMaxTransactionFee(Hbar(20LL));
}

//-----
ContractCreateTransaction::ContractCreateTransaction(const proto::TransactionBody& transactionBody)
{
  if (!transactionBody.has_contractcreateinstance())
  {
    throw std::invalid_argument("Transaction body doesn't contain ContractCreateInstance data");
  }

  const proto::ContractCreateTransactionBody& body = transactionBody.contractcreateinstance();

  if (body.has_fileid())
  {
    mBytecodeFileId = FileId::fromProtobuf(body.fileid());
  }

  if (body.has_initcode())
  {
    mBytecode = internal::Utilities::stringToByteVector(body.initcode());
  }

  if (body.has_adminkey())
  {
    mAdminKey = PublicKey::fromProtobuf(body.adminkey());
  }

  mGas = static_cast<uint64_t>(body.gas());
  mInitialBalance = Hbar(body.initialbalance(), HbarUnit::TINYBAR());

  if (body.has_autorenewperiod())
  {
    mAutoRenewPeriod = internal::DurationConverter::fromProtobuf(body.autorenewperiod());
  }

  mConstructorParameters = internal::Utilities::stringToByteVector(body.constructorparameters());
  mMemo = body.memo();
  mMaxAutomaticTokenAssociations = static_cast<uint32_t>(body.max_automatic_token_associations());

  if (body.has_auto_renew_account_id())
  {
    mAutoRenewAccountId = AccountId::fromProtobuf(body.auto_renew_account_id());
  }

  if (body.has_staked_account_id())
  {
    mStakedAccountId = AccountId::fromProtobuf(body.staked_account_id());
  }

  if (body.has_staked_node_id())
  {
    mStakedNodeId = static_cast<uint64_t>(body.staked_node_id());
  }

  mDeclineStakingReward = body.decline_reward();
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setBytecodeFileId(const FileId& fileId)
{
  requireNotFrozen();
  mBytecodeFileId = fileId;
  mBytecode.clear();
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setBytecode(const std::vector<std::byte>& initCode)
{
  requireNotFrozen();
  mBytecode = initCode;
  mBytecodeFileId.reset();
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setAdminKey(const std::shared_ptr<PublicKey>& key)
{
  requireNotFrozen();
  mAdminKey = key;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setGas(const uint64_t& gas)
{
  requireNotFrozen();
  mGas = gas;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setInitialBalance(const Hbar& initialBalance)
{
  requireNotFrozen();
  mInitialBalance = initialBalance;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setAutoRenewPeriod(
  const std::chrono::duration<double>& autoRenewPeriod)
{
  requireNotFrozen();
  mAutoRenewPeriod = autoRenewPeriod;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setConstructorParameters(
  const std::vector<std::byte>& constructorParameters)
{
  requireNotFrozen();
  mConstructorParameters = constructorParameters;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setMemo(std::string_view memo)
{
  requireNotFrozen();
  mMemo = memo;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setMaxAutomaticTokenAssociations(uint32_t associations)
{
  requireNotFrozen();
  mMaxAutomaticTokenAssociations = associations;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setAutoRenewAccountId(const AccountId& autoRenewAccountId)
{
  requireNotFrozen();
  mAutoRenewAccountId = autoRenewAccountId;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setStakedAccountId(const AccountId& stakedAccountId)
{
  requireNotFrozen();
  mStakedAccountId = stakedAccountId;
  mStakedNodeId.reset();
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setStakedNodeId(const uint64_t& stakedNodeId)
{
  requireNotFrozen();
  mStakedNodeId = stakedNodeId;
  mStakedAccountId.reset();
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setDeclineStakingReward(bool declineReward)
{
  requireNotFrozen();
  mDeclineStakingReward = declineReward;
  return *this;
}

//-----
proto::Transaction ContractCreateTransaction::makeRequest(const Client& client,
                                                          const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody transactionBody = generateTransactionBody(client);
  transactionBody.set_allocated_contractcreateinstance(build());

  return signTransaction(transactionBody, client);
}

//-----
grpc::Status ContractCreateTransaction::submitRequest(const Client& client,
                                                      const std::chrono::system_clock::time_point& deadline,
                                                      const std::shared_ptr<internal::Node>& node,
                                                      proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kContractCreateInstance, makeRequest(client, node), deadline, response);
}

//-----
proto::ContractCreateTransactionBody* ContractCreateTransaction::build() const
{
  auto body = std::make_unique<proto::ContractCreateTransactionBody>();

  if (mBytecodeFileId.has_value())
  {
    body->set_allocated_fileid(mBytecodeFileId->toProtobuf().release());
  }
  else
  {
    body->set_allocated_initcode(new std::string(internal::Utilities::byteVectorToString(mBytecode)));
  }

  if (mAdminKey)
  {
    body->set_allocated_adminkey(mAdminKey->toProtobuf().release());
  }

  body->set_gas(static_cast<int64_t>(mGas));
  body->set_initialbalance(mInitialBalance.toTinybars());
  body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(mAutoRenewPeriod));
  body->set_allocated_constructorparameters(
    new std::string(internal::Utilities::byteVectorToString(mConstructorParameters)));
  body->set_allocated_memo(new std::string(mMemo));
  body->set_max_automatic_token_associations(static_cast<int32_t>(mMaxAutomaticTokenAssociations));

  if (mAutoRenewAccountId.has_value())
  {
    body->set_allocated_auto_renew_account_id(mAutoRenewAccountId->toProtobuf().release());
  }

  if (mStakedAccountId.has_value())
  {
    body->set_allocated_staked_account_id(mStakedAccountId->toProtobuf().release());
  }

  if (mStakedNodeId.has_value())
  {
    body->set_staked_node_id(static_cast<int64_t>(*mStakedNodeId));
  }

  body->set_decline_reward(mDeclineStakingReward);

  return body.release();
}

} // namespace Hedera

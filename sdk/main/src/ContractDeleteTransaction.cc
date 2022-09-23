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
#include "ContractDeleteTransaction.h"

#include "contract_delete.pb.h"

namespace Hedera
{
//-----
ContractDeleteTransaction::ContractDeleteTransaction()
  : Transaction()
  , mContractId()
  , mTransferAccountId()
  , mTransferContractId()
  , mPermanentRemoval(false)
{
}

//-----
ContractDeleteTransaction::ContractDeleteTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//-----
ContractDeleteTransaction::ContractDeleteTransaction(
  const proto::TransactionBody& transaction)
  : Transaction(transaction)
{
  initFromTransactionBody();
}

//-----
void
ContractDeleteTransaction::validateChecksums(const Client& client) const
{
  if (mContractId.isValid())
  {
    mContractId.getValue().validateChecksum(client);
  }

  if (mTransferAccountId.isValid())
  {
    mTransferAccountId.getValue().validateChecksum(client);
  }

  if (mTransferContractId.isValid())
  {
    mTransferContractId.getValue().validateChecksum(client);
  }
}

//-----
proto::ContractDeleteTransactionBody
ContractDeleteTransaction::build() const
{
  proto::ContractDeleteTransactionBody body;

  if (mContractId.isValid())
  {
    body.set_allocated_contractid(mContractId.getValue().toProtobuf());
  }

  if (mTransferAccountId.isValid())
  {
    body.set_allocated_transferaccountid(
      mTransferAccountId.getValue().toProtobuf());
  }

  if (mTransferContractId.isValid())
  {
    body.set_allocated_transfercontractid(
      mTransferContractId.getValue().toProtobuf());
  }

  body.set_permanent_removal(mPermanentRemoval);

  return body;
}

//-----
ContractDeleteTransaction&
ContractDeleteTransaction::setContractId(const ContractId& contractId)
{
  requireNotFrozen();

  mContractId.setValue(contractId);
  return *this;
}

//-----
ContractDeleteTransaction&
ContractDeleteTransaction::setTransferAccountId(
  const AccountId& transferAccountId)
{
  requireNotFrozen();

  mTransferAccountId.setValue(transferAccountId);
  return *this;
}

//-----
ContractDeleteTransaction&
ContractDeleteTransaction::setTransferContractId(
  const ContractId& transferContractId)
{
  requireNotFrozen();

  mTransferContractId.setValue(transferContractId);
  return *this;
}

//-----
void
ContractDeleteTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_contractdeleteinstance())
  {
    const proto::ContractDeleteTransactionBody& body =
      mSourceTransactionBody.contractdeleteinstance();

    if (body.has_contractid())
    {
      mContractId.setValue(ContractId::fromProtobuf(body.contractid()));
    }

    if (body.has_transferaccountid())
    {
      mTransferAccountId.setValue(
        AccountId::fromProtobuf(body.transferaccountid()));
    }

    if (body.has_transfercontractid())
    {
      mTransferContractId.setValue(
        ContractId::fromProtobuf(body.transfercontractid()));
    }

    mPermanentRemoval = body.permanent_removal();
  }
}

} // namespace Hedera

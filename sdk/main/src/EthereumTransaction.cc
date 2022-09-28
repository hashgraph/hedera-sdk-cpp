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
#include "EthereumTransaction.h"

namespace Hedera
{
//-----
EthereumTransaction::EthereumTransaction()
  : Transaction()
  , mEthereumData()
  , mCallData()
  , mMaxGasAllowance(0LL)
{
}

//-----
EthereumTransaction::EthereumTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//-----
EthereumTransaction::EthereumTransaction(
  const proto::TransactionBody& transaction)
  : Transaction(transaction)
{
  initFromTransactionBody();
}

//-----
void
EthereumTransaction::validateChecksums(const Client& client) const
{
  if (mCallData.isValid())
  {
    mCallData.getValue().validateChecksum(client);
  }
}

//-----
proto::EthereumTransactionBody
EthereumTransaction::build() const
{
  proto::EthereumTransactionBody proto;

  proto.set_ethereum_data(mEthereumData);

  if (mCallData.isValid())
  {
    proto.set_allocated_call_data(mCallData.getValue().toProtobuf());
  }

  proto.set_max_gas_allowance(mMaxGasAllowance.toTinybars());

  return proto;
}

//-----
EthereumTransaction&
EthereumTransaction::setEthereumData(const std::string& ethereumData)
{
  requireNotFrozen();

  mEthereumData = ethereumData;
  return *this;
}

//-----
EthereumTransaction&
EthereumTransaction::setCallDataFileId(const FileId& fileId)
{
  requireNotFrozen();

  mCallData = fileId;
  return *this;
}

//-----
EthereumTransaction&
EthereumTransaction::setMaxGasAllowanceHbar(const Hbar& maxGasAllowance)
{
  requireNotFrozen();

  mMaxGasAllowance = maxGasAllowance;
  return *this;
}

//-----
void
EthereumTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_ethereumtransaction())
  {
    const proto::EthereumTransactionBody& body =
      mSourceTransactionBody.ethereumtransaction();

    mEthereumData = body.ethereum_data();

    if (body.has_call_data())
    {
      mCallData = FileId::fromProtobuf(body.call_data());
    }

    mMaxGasAllowance = Hbar::fromTinybars(body.max_gas_allowance());
  }
}

} // namespace Hedera
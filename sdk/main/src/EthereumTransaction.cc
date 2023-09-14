/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <proto/ethereum_transaction.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
EthereumTransaction::EthereumTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<EthereumTransaction>(transactionBody)
{
  EthereumTransaction();
}

//-----
EthereumTransaction::EthereumTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<EthereumTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
EthereumTransaction& EthereumTransaction::setEthereumData(const std::vector<std::byte>& ethereumData)
{
  requireNotFrozen();
  mEthereumData = ethereumData;
  return *this;
}

//-----
EthereumTransaction& EthereumTransaction::setCallDataFileId(const FileId& fileId)
{
  requireNotFrozen();
  mCallDataFileId = fileId;
  return *this;
}

//-----
EthereumTransaction& EthereumTransaction::setMaxGasAllowance(const Hbar& maxGasAllowance)
{
  requireNotFrozen();
  mMaxGasAllowance = maxGasAllowance;
  return *this;
}

//-----
grpc::Status EthereumTransaction::submitRequest(const proto::Transaction& request,
                                                const std::shared_ptr<internal::Node>& node,
                                                const std::chrono::system_clock::time_point& deadline,
                                                proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kEthereumTransaction, request, deadline, response);
}

//-----
void EthereumTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_ethereumtransaction(build());
}

//-----
void EthereumTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_ethereumtransaction())
  {
    throw std::invalid_argument("Transaction body doesn't contain EthereumTransaction data");
  }

  const proto::EthereumTransactionBody& body = transactionBody.ethereumtransaction();

  mEthereumData = internal::Utilities::stringToByteVector(body.ethereum_data());

  if (body.has_call_data())
  {
    mCallDataFileId = FileId::fromProtobuf(body.call_data());
  }

  mMaxGasAllowance = Hbar(body.max_gas_allowance(), HbarUnit::TINYBAR());
}

//-----
proto::EthereumTransactionBody* EthereumTransaction::build() const
{
  auto body = std::make_unique<proto::EthereumTransactionBody>();

  body->set_ethereum_data(internal::Utilities::byteVectorToString(mEthereumData));

  if (mCallDataFileId.has_value())
  {
    body->set_allocated_call_data(mCallDataFileId->toProtobuf().release());
  }

  body->set_max_gas_allowance(mMaxGasAllowance.toTinybars());

  return body.release();
}

} // namespace Hedera

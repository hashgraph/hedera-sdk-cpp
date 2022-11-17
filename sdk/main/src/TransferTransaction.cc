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
#include "TransferTransaction.h"
#include "TransactionResponse.h"
#include "impl/Node.h"

#include <proto/crypto_transfer.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction.pb.h>

namespace Hedera
{
//-----
std::unique_ptr<Executable<TransferTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>>
TransferTransaction::clone() const
{
  return std::make_unique<TransferTransaction>(*this);
}

//-----
TransferTransaction& TransferTransaction::addApprovedHbarTransfer(const std::shared_ptr<AccountId>& accountId,
                                                                  const Hbar& amount)
{
  addHbarTransfer(Transfer().setAccountId(accountId).setAmount(amount).setApproved(true));
  return *this;
}
//-----
TransferTransaction& TransferTransaction::addUnapprovedHbarTransfer(const std::shared_ptr<AccountId>& accountId,
                                                                    const Hbar& amount)
{
  addHbarTransfer(Transfer().setAccountId(accountId).setAmount(amount).setApproved(false));
  return *this;
}

//-----
proto::Transaction TransferTransaction::makeRequest(const Client& client, const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody body = generateTransactionBody(client);
  body.set_allocated_cryptotransfer(build());

  return signTransaction(body, client);
}

//-----
std::function<grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
TransferTransaction::getGrpcMethod(const std::shared_ptr<internal::Node>& node) const
{
  return node->getGrpcTransactionMethod(proto::TransactionBody::DataCase::kCryptoTransfer);
}

//-----
proto::CryptoTransferTransactionBody* TransferTransaction::build() const
{
  auto body = std::make_unique<proto::CryptoTransferTransactionBody>();

  for (const Transfer& transfer : mHbarTransfers)
  {
    proto::AccountAmount* amount = body->mutable_transfers()->add_accountamounts();
    amount->set_allocated_accountid(transfer.getAccountId()->toProtobuf().release());
    amount->set_amount(transfer.getAmount().toTinybars());
    amount->set_is_approval(transfer.getApproval());
  }

  return body.release();
}

//----
void TransferTransaction::addHbarTransfer(const Transfer& transfer)
{
  // If a transfer has already been added for an account, just update the amount if the approval status is the same
  for (Transfer& hbarTransfer : mHbarTransfers)
  {
    if (hbarTransfer.getAccountId() == transfer.getAccountId() && hbarTransfer.getApproval() == transfer.getApproval())
    {
      hbarTransfer.setAmount(
        Hbar(hbarTransfer.getAmount().toTinybars() + transfer.getAmount().toTinybars(), HbarUnit::TINYBAR()));
      return;
    }
  }

  mHbarTransfers.push_back(transfer);
}

} // namespace Hedera

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
#include "TransferTransaction.h"

#include "TransactionResponse.h"

#include <proto/crypto_transfer.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction.pb.h>

namespace Hedera
{
//-----
TransferTransaction& TransferTransaction::addApprovedHbarTransfer(const AccountId& accountId, const Hbar& amount)
{
  addHbarTransfer(Transfer().setAccountId(accountId).setAmount(amount).setApproved(true));
  return *this;
}
//-----
TransferTransaction& TransferTransaction::addUnapprovedHbarTransfer(const AccountId& accountId, const Hbar& amount)
{
  addHbarTransfer(Transfer().setAccountId(accountId).setAmount(amount).setApproved(false));
  return *this;
}

//-----
proto::Transaction TransferTransaction::makeRequest() const
{
  proto::Transaction transaction;
  proto::TransactionBody* body = transaction.mutable_body();
  body->set_allocated_cryptotransfer(build().get());

  // TODO: sign here?

  return transaction;
}

//-----
std::shared_ptr<proto::CryptoTransferTransactionBody> TransferTransaction::build() const
{
  auto body = std::make_shared<proto::CryptoTransferTransactionBody>();

  for (const Transfer& transfer : mHbarTransfers)
  {
    proto::AccountAmount* amount = body->mutable_transfers()->add_accountamounts();

    if (transfer.getAccountId().has_value())
    {
      amount->set_allocated_accountid(transfer.getAccountId()->toProtobuf().get());
    }

    amount->set_amount(transfer.getAmount().toTinybars());
    amount->set_is_approval(transfer.getApproval());
  }

  return body;
}

//----
void TransferTransaction::addHbarTransfer(const Transfer& transfer)
{
  for (Transfer& hbarTransfer : mHbarTransfers)
  {
    if (hbarTransfer.getAccountId() == transfer.getAccountId() && hbarTransfer.getApproval() == transfer.getApproval())
    {
      hbarTransfer.setAmount(Hbar::from(hbarTransfer.getAmount().toTinybars() + transfer.getAmount().toTinybars()));
      return;
    }
  }

  mHbarTransfers.push_back(transfer);
}

} // namespace Hedera

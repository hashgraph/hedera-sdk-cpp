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
#include "TransactionReceipt.h"
#include "exceptions/ReceiptStatusException.h"

#include <proto/transaction_receipt.pb.h>

namespace Hedera
{
//-----
TransactionReceipt TransactionReceipt::fromProtobuf(const proto::TransactionReceipt& proto)
{
  TransactionReceipt receipt;
  receipt.mStatus = gProtobufResponseCodeToStatus.at(proto.status());

  if (proto.has_accountid())
  {
    receipt.mAccountId = AccountId::fromProtobuf(proto.accountid());
  }

  if (proto.has_fileid())
  {
    receipt.mFileId = FileId::fromProtobuf(proto.fileid());
  }

  if (proto.has_contractid())
  {
    receipt.mContractId = ContractId::fromProtobuf(proto.contractid());
  }

  if (proto.has_exchangerate())
  {
    receipt.mExchangeRates = ExchangeRateSet::fromProtobuf(proto.exchangerate());
  }

  if (proto.has_tokenid())
  {
    receipt.mTokenId = TokenId::fromProtobuf(proto.tokenid());
  }

  receipt.mNewTotalSupply = proto.newtotalsupply();

  for (int i = 0; i < proto.serialnumbers_size(); ++i)
  {
    receipt.mSerialNumbers.push_back(static_cast<uint64_t>(proto.serialnumbers(i)));
  }

  return receipt;
}

//-----
void TransactionReceipt::validateStatus() const
{
  if (mStatus != Status::SUCCESS)
  {
    throw ReceiptStatusException(gStatusToString.at(mStatus));
  }
}

} // namespace Hedera

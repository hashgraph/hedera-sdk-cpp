/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "TransactionRecord.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <proto/transaction_record.pb.h>

namespace Hedera
{
//-----
TransactionRecord TransactionRecord::fromProtobuf(const proto::TransactionRecord& proto)
{
  TransactionRecord transactionRecord;

  if (proto.has_receipt())
  {
    transactionRecord.mReceipt = TransactionReceipt::fromProtobuf(proto.receipt(), std::nullopt);
  }

  transactionRecord.mTransactionHash = proto.transactionhash();

  if (proto.has_consensustimestamp())
  {
    transactionRecord.mConsensusTimestamp = internal::TimestampConverter::fromProtobuf(proto.consensustimestamp());
  }

  if (proto.has_transactionid())
  {
    transactionRecord.mTransactionID = TransactionId::fromProtobuf(proto.transactionid());
  }

  transactionRecord.mMemo = proto.memo();
  transactionRecord.mTransactionFee = proto.transactionfee();

  if (proto.has_contractcallresult())
  {
    transactionRecord.mContractFunctionResult = ContractFunctionResult::fromProtobuf(proto.contractcallresult());
  }
  else if (proto.has_contractcreateresult())
  {
    transactionRecord.mContractFunctionResult = ContractFunctionResult::fromProtobuf(proto.contractcreateresult());
  }

  if (proto.has_transferlist())
  {
    for (int i = 0; i < proto.transferlist().accountamounts_size(); ++i)
    {
      transactionRecord.mHbarTransferList.push_back(HbarTransfer::fromProtobuf(proto.transferlist().accountamounts(i)));
    }
  }

  for (int i = 0; i < proto.tokentransferlists_size(); ++i)
  {
    const proto::TokenTransferList& list = proto.tokentransferlists(i);
    const TokenId tokenId = TokenId::fromProtobuf(list.token());

    // Fungible token
    const std::vector<TokenTransfer> fungibleTransfers = TokenTransfer::fromProtobuf(list);
    transactionRecord.mTokenTransferList.insert(
      transactionRecord.mTokenTransferList.cend(), fungibleTransfers.cbegin(), fungibleTransfers.cend());

    // NFT
    for (int j = 0; j < list.nfttransfers_size(); ++j)
    {
      const proto::NftTransfer& nftTransfer = list.nfttransfers(j);

      TokenNftTransfer transfer;
      transfer.setNftId(NftId(tokenId, static_cast<uint64_t>(nftTransfer.serialnumber())));
      transfer.setSenderAccountId(AccountId::fromProtobuf(nftTransfer.senderaccountid()));
      transfer.setReceiverAccountId(AccountId::fromProtobuf(nftTransfer.receiveraccountid()));
      transfer.setApproval(nftTransfer.is_approval());

      transactionRecord.mNftTransferList.push_back(transfer);
    }
  }

  if (proto.has_scheduleref())
  {
    transactionRecord.mScheduleRef = ScheduleId::fromProtobuf(proto.scheduleref());
  }

  for (int i = 0; i < proto.assessed_custom_fees_size(); ++i)
  {
    transactionRecord.mAssessedCustomFees.push_back(AssessedCustomFee::fromProtobuf(proto.assessed_custom_fees(i)));
  }

  for (int i = 0; i < proto.automatic_token_associations_size(); ++i)
  {
    transactionRecord.mAutomaticTokenAssociations.push_back(
      TokenAssociation::fromProtobuf(proto.automatic_token_associations(i)));
  }

  if (!proto.evm_address().empty())
  {
    transactionRecord.mEvmAddress = EvmAddress::fromBytes(internal::Utilities::stringToByteVector(proto.evm_address()));
  }

  return transactionRecord;
}

} // namespace Hedera

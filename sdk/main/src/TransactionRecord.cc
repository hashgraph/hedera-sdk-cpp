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

#include <proto/transaction_get_record.pb.h>
#include <proto/transaction_record.pb.h>

namespace Hedera
{
//-----
TransactionRecord TransactionRecord::fromProtobuf(const proto::TransactionGetRecordResponse& proto)
{
  TransactionRecord record = TransactionRecord::fromProtobuf(proto.transactionrecord());

  for (int i = 0; i < proto.duplicatetransactionrecords_size(); ++i)
  {
    record.mDuplicates.push_back(TransactionRecord::fromProtobuf(proto.duplicatetransactionrecords(i)));
  }

  for (int i = 0; i < proto.child_transaction_records_size(); ++i)
  {
    record.mChildren.push_back(TransactionRecord::fromProtobuf(proto.child_transaction_records(i)));
  }

  return record;
}

//-----
TransactionRecord TransactionRecord::fromProtobuf(const proto::TransactionRecord& proto)
{
  TransactionRecord transactionRecord;

  if (proto.has_transactionid())
  {
    transactionRecord.mTransactionID = TransactionId::fromProtobuf(proto.transactionid());

    if (proto.has_receipt())
    {
      transactionRecord.mReceipt =
        TransactionReceipt::fromProtobuf(proto.receipt(), transactionRecord.mTransactionID.value());
    }
  }
  else
  {
    transactionRecord.mReceipt = TransactionReceipt::fromProtobuf(proto.receipt());
  }

  transactionRecord.mTransactionHash = proto.transactionhash();

  if (proto.has_consensustimestamp())
  {
    transactionRecord.mConsensusTimestamp = internal::TimestampConverter::fromProtobuf(proto.consensustimestamp());
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
    for (int j = 0; j < list.transfers_size(); ++j)
    {
      transactionRecord.mTokenTransferList.push_back(TokenTransfer::fromProtobuf(
        list.transfers(j), tokenId, list.has_expected_decimals() ? list.expected_decimals().value() : 0U));
    }

    // NFT
    for (int j = 0; j < list.nfttransfers_size(); ++j)
    {
      transactionRecord.mNftTransferList.push_back(TokenNftTransfer::fromProtobuf(list.nfttransfers(j), tokenId));
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

  if (proto.has_parent_consensus_timestamp())
  {
    transactionRecord.mParentConsensusTimestamp =
      internal::TimestampConverter::fromProtobuf(proto.parent_consensus_timestamp());
  }

  if (!proto.alias().empty())
  {
    transactionRecord.mAlias = PublicKey::fromAliasBytes(internal::Utilities::stringToByteVector(proto.alias()));
  }

  if (!proto.ethereum_hash().empty())
  {
    transactionRecord.mEthereumHash = internal::Utilities::stringToByteVector(proto.ethereum_hash());
  }

  for (int i = 0; i < proto.paid_staking_rewards_size(); ++i)
  {
    transactionRecord.mPaidStakingRewards.push_back(HbarTransfer::fromProtobuf(proto.paid_staking_rewards(i)));
  }

  if (proto.has_prng_bytes())
  {
    transactionRecord.mPrngBytes = internal::Utilities::stringToByteVector(proto.prng_bytes());
  }
  else if (proto.has_prng_number())
  {
    transactionRecord.mPrngNumber = proto.prng_number();
  }

  if (!proto.evm_address().empty())
  {
    transactionRecord.mEvmAddress = EvmAddress::fromBytes(internal::Utilities::stringToByteVector(proto.evm_address()));
  }

  return transactionRecord;
}

} // namespace Hedera

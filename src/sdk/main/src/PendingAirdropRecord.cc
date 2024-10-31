/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "PendingAirdropRecord.h"
#include "PendingAirdropId.h"

#include <proto/basic_types.pb.h>
#include <proto/transaction_record.pb.h>

namespace Hedera
{
//-----
PendingAirdropRecord::PendingAirdropRecord(PendingAirdropId pendingAirdropId, uint64_t amount)
  : mPendingAirdropId(pendingAirdropId)
  , mAmount(amount)
{
}

//-----
PendingAirdropRecord PendingAirdropRecord::fromProtobuf(const proto::PendingAirdropRecord& proto)
{
  PendingAirdropRecord pendingAirdropRecord;

  pendingAirdropRecord.mPendingAirdropId = PendingAirdropId::fromProtobuf(proto.pending_airdrop_id());
  pendingAirdropRecord.mAmount = proto.pending_airdrop_value().amount();

  return pendingAirdropRecord;
}

//-----
std::unique_ptr<proto::PendingAirdropRecord> PendingAirdropRecord::toProtobuf() const
{
  auto proto = std::make_unique<proto::PendingAirdropRecord>();

  proto->set_allocated_pending_airdrop_id(mPendingAirdropId.toProtobuf().release());

  auto value = std::make_unique<proto::PendingAirdropValue>();
  value->set_amount(mAmount);

  proto->set_allocated_pending_airdrop_value(value.release());

  return proto;
}

//-----
std::string PendingAirdropRecord::toString() const
{
  return mPendingAirdropId.toString() + " " + std::to_string(mAmount);
}

} // namespace Hedera
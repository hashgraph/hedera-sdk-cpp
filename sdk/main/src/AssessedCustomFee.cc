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
#include "AssessedCustomFee.h"
#include "impl/Utilities.h"

#include <proto/custom_fees.pb.h>

namespace Hedera
{
//-----
AssessedCustomFee AssessedCustomFee::fromProtobuf(const proto::AssessedCustomFee& proto)
{
  AssessedCustomFee assessedCustomFee;
  assessedCustomFee.mAmount = proto.amount();

  if (proto.has_token_id())
  {
    assessedCustomFee.mTokenId = TokenId::fromProtobuf(proto.token_id());
  }

  if (proto.has_fee_collector_account_id())
  {
    assessedCustomFee.mFeeCollectorAccountId = AccountId::fromProtobuf(proto.fee_collector_account_id());
  }

  for (int i = 0; i < proto.effective_payer_account_id_size(); ++i)
  {
    assessedCustomFee.mPayerAccountIdList.push_back(AccountId::fromProtobuf(proto.effective_payer_account_id(i)));
  }

  return assessedCustomFee;
}

//-----
AssessedCustomFee AssessedCustomFee::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::AssessedCustomFee assessedCustomFee;
  assessedCustomFee.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(assessedCustomFee);
}

//-----
std::unique_ptr<proto::AssessedCustomFee> AssessedCustomFee::toProtobuf() const
{
  auto assessedCustomFee = std::make_unique<proto::AssessedCustomFee>();
  assessedCustomFee->set_amount(mAmount);

  if (mTokenId.has_value())
  {
    assessedCustomFee->set_allocated_token_id(mTokenId->toProtobuf().release());
  }

  assessedCustomFee->set_allocated_fee_collector_account_id(mFeeCollectorAccountId.toProtobuf().release());

  for (const auto& accountId : mPayerAccountIdList)
  {
    *assessedCustomFee->add_effective_payer_account_id() = *accountId.toProtobuf();
  }

  return assessedCustomFee;
}

//-----
std::vector<std::byte> AssessedCustomFee::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

} // namespace Hedera

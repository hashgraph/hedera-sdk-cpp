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
#include "TokenTransfer.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
TokenTransfer::TokenTransfer(const TokenId& tokenId, AccountId accountId, int64_t amount, bool isApproved)
  : mTokenId(tokenId)
  , mAccountId(std::move(accountId))
  , mAmount(amount)
  , mIsApproval(isApproved)
{
}

//-----
TokenTransfer::TokenTransfer(const TokenId& tokenId,
                             AccountId accountId,
                             int64_t amount,
                             uint32_t decimals,
                             bool isApproved)
  : mTokenId(tokenId)
  , mAccountId(std::move(accountId))
  , mAmount(amount)
  , mExpectedDecimals(decimals)
  , mIsApproval(isApproved)
{
}

//-----
std::vector<TokenTransfer> TokenTransfer::fromProtobuf(const proto::TokenTransferList& proto)
{
  std::vector<TokenTransfer> transfers;
  const TokenId tokenId = TokenId::fromProtobuf(proto.token());

  for (int i = 0; i < proto.transfers_size(); ++i)
  {
    const proto::AccountAmount& amount = proto.transfers(i);
    transfers.emplace_back(tokenId,
                           AccountId::fromProtobuf(amount.accountid()),
                           amount.amount(),
                           proto.has_expected_decimals() ? proto.expected_decimals().value() : 0U,
                           amount.is_approval());
  }

  return transfers;
}

//-----
std::unique_ptr<proto::AccountAmount> TokenTransfer::toProtobuf() const
{
  auto accountAmount = std::make_unique<proto::AccountAmount>();
  accountAmount->set_allocated_accountid(mAccountId.toProtobuf().release());
  accountAmount->set_amount(mAmount);
  accountAmount->set_is_approval(mIsApproval);
  return accountAmount;
}

} // namespace Hedera

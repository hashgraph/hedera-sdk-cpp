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
#include "TokenNftTransfer.h"

#include <limits>
#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
TokenNftTransfer TokenNftTransfer::fromProtobuf(const proto::NftTransfer& proto)
{
  TokenNftTransfer tokenNftTransfer;

  if (proto.has_senderaccountid())
  {
    tokenNftTransfer.mSenderAccountID = AccountId::fromProtobuf(proto.senderaccountid());
  }

  if (proto.has_receiveraccountid())
  {
    tokenNftTransfer.mReceiverAccountID = AccountId::fromProtobuf(proto.receiveraccountid());
  }

  tokenNftTransfer.mNftId.setSerialNum(static_cast<uint64_t>(proto.serialnumber()));
  tokenNftTransfer.mIsApproval = proto.is_approval();
  return tokenNftTransfer;
}

//-----
std::unique_ptr<proto::NftTransfer> TokenNftTransfer::toProtobuf() const
{
  auto proto = std::make_unique<proto::NftTransfer>();
  proto->set_allocated_senderaccountid(mSenderAccountID.toProtobuf().release());
  proto->set_allocated_receiveraccountid(mReceiverAccountID.toProtobuf().release());
  proto->set_serialnumber(static_cast<int64_t>(mNftId.getSerialNum()));
  proto->set_is_approval(mIsApproval);
  return proto;
}

//-----
TokenNftTransfer& TokenNftTransfer::setNftId(const NftId& nftId)
{
  mNftId = nftId;
  return *this;
}

//-----
TokenNftTransfer& TokenNftTransfer::setSenderAccountId(const AccountId& accountId)
{
  mSenderAccountID = accountId;
  return *this;
}

//-----
TokenNftTransfer& TokenNftTransfer::setReceiverAccountId(const AccountId& accountId)
{
  mReceiverAccountID = accountId;
  return *this;
}

//-----
TokenNftTransfer& TokenNftTransfer::setApproval(bool approval)
{
  mIsApproval = approval;
  return *this;
}

} // namespace Hedera

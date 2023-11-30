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
#include "AccountInfo.h"
#include "impl/DurationConverter.h"
#include "impl/HexConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <nlohmann/json.hpp>
#include <proto/crypto_get_info.pb.h>

namespace Hedera
{
//-----
AccountInfo AccountInfo::fromProtobuf(const proto::CryptoGetInfoResponse_AccountInfo& proto)
{
  AccountInfo accountInfo;

  if (proto.has_accountid())
  {
    accountInfo.mAccountId = AccountId::fromProtobuf(proto.accountid());
  }

  accountInfo.mContractAccountId = proto.contractaccountid();
  accountInfo.mIsDeleted = proto.deleted();
  accountInfo.mProxyReceived = Hbar(proto.proxyreceived(), HbarUnit::TINYBAR());

  if (proto.has_key())
  {
    accountInfo.mKey = Key::fromProtobuf(proto.key());
  }

  accountInfo.mBalance = Hbar(static_cast<int64_t>(proto.balance()), HbarUnit::TINYBAR());
  accountInfo.mReceiverSignatureRequired = proto.receiversigrequired();

  if (proto.has_expirationtime())
  {
    accountInfo.mExpirationTime = internal::TimestampConverter::fromProtobuf(proto.expirationtime());
  }

  if (proto.has_autorenewperiod())
  {
    accountInfo.mAutoRenewPeriod = internal::DurationConverter::fromProtobuf(proto.autorenewperiod());
  }

  accountInfo.mMemo = proto.memo();
  accountInfo.mOwnedNfts = static_cast<uint64_t>(proto.ownednfts());
  accountInfo.mMaxAutomaticTokenAssociations = static_cast<uint32_t>(proto.max_automatic_token_associations());

  if (!proto.alias().empty())
  {
    if (proto.alias().size() == EvmAddress::NUM_BYTES)
    {
      accountInfo.mEvmAddressAlias = EvmAddress::fromBytes(internal::Utilities::stringToByteVector(proto.alias()));
    }
    else
    {
      accountInfo.mPublicKeyAlias = PublicKey::fromAliasBytes(internal::Utilities::stringToByteVector(proto.alias()));
    }
  }

  if (!proto.ledger_id().empty())
  {
    accountInfo.mLedgerId = LedgerId(internal::Utilities::stringToByteVector(proto.ledger_id()));
  }

  if (proto.has_staking_info())
  {
    accountInfo.mStakingInfo = StakingInfo::fromProtobuf(proto.staking_info());
  }

  return accountInfo;
}

//-----
AccountInfo AccountInfo::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::CryptoGetInfoResponse_AccountInfo proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::CryptoGetInfoResponse_AccountInfo> AccountInfo::toProtobuf() const
{
  auto proto = std::make_unique<proto::CryptoGetInfoResponse_AccountInfo>();
  proto->set_allocated_accountid(mAccountId.toProtobuf().release());
  proto->set_contractaccountid(mContractAccountId);
  proto->set_deleted(mIsDeleted);
  proto->set_proxyreceived(mProxyReceived.toTinybars());

  if (mKey)
  {
    proto->set_allocated_key(mKey->toProtobufKey().release());
  }

  proto->set_balance(static_cast<uint64_t>(mBalance.toTinybars()));
  proto->set_receiversigrequired(mReceiverSignatureRequired);
  proto->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(mExpirationTime));
  proto->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(mAutoRenewPeriod));
  proto->set_memo(mMemo);
  proto->set_ownednfts(static_cast<int64_t>(mOwnedNfts));
  proto->set_max_automatic_token_associations(static_cast<int32_t>(mMaxAutomaticTokenAssociations));

  if (mPublicKeyAlias)
  {
    proto->set_alias(mPublicKeyAlias->toProtobufKey()->SerializeAsString());
  }
  else if (mEvmAddressAlias.has_value())
  {
    proto->set_alias(internal::Utilities::byteVectorToString(mEvmAddressAlias->toBytes()));
  }

  proto->set_ledger_id(internal::Utilities::byteVectorToString(mLedgerId.toBytes()));
  proto->set_allocated_staking_info(mStakingInfo.toProtobuf().release());

  return proto;
}

//-----
std::vector<std::byte> AccountInfo::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string AccountInfo::toString() const
{
  nlohmann::json json;
  json["mAccountId"] = mAccountId.toString();
  json["mContractAccountId"] = mContractAccountId;
  json["mIsDeleted"] = (mIsDeleted ? "true" : "false");
  json["mProxyReceived"] = mProxyReceived.toString();

  if (mKey != nullptr)
  {
    json["mKey"] = internal::HexConverter::bytesToHex(mKey->toBytes());
  }

  json["mBalance"] = mBalance.toString();
  json["mReceiverSignatureRequired"] = (mReceiverSignatureRequired ? "true" : "false");
  json["mExpirationTime"] = internal::TimestampConverter::toString(mExpirationTime);
  json["mAutoRenewPeriod"] = std::to_string(mAutoRenewPeriod.count());
  json["mMemo"] = mMemo;
  json["mOwnedNfts"] = mOwnedNfts;
  json["mMaxAutomaticTokenAssociations"] = mMaxAutomaticTokenAssociations;

  if (mPublicKeyAlias != nullptr)
  {
    json["mPublicKeyAlias"] = mPublicKeyAlias->toStringDer();
  }

  if (mEvmAddressAlias.has_value())
  {
    json["mEvmAddressAlias"] = mEvmAddressAlias->toString();
  }

  json["mLedgerId"] = mLedgerId.toString();
  json["mStakingInfo"] = mStakingInfo.toString();

  return json.dump();
}

//-----
std::ostream& operator<<(std::ostream& os, const AccountInfo& info)
{
  os << info.toString();
  return os;
}

} // namespace Hedera

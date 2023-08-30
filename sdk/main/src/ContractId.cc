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
#include "ContractId.h"
#include "impl/Utilities.h"

#include <limits>
#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
ContractId::ContractId(const uint64_t& num)
  : mContractNum(num)
{
  checkContractNum();
}

//-----
ContractId::ContractId(const EvmAddress& address)
  : mEvmAddress(address)
{
}

//-----
ContractId::ContractId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mContractNum(num)
{
  checkShardNum();
  checkRealmNum();
  checkContractNum();
}

//-----
ContractId::ContractId(const uint64_t& shard, const uint64_t& realm, const EvmAddress& address)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mEvmAddress(address)
{
  checkShardNum();
  checkRealmNum();
}

//-----
bool ContractId::operator==(const ContractId& other) const
{
  return (mShardNum == other.mShardNum) && (mRealmNum == other.mRealmNum) &&
         ((mContractNum && other.mContractNum && *mContractNum == *other.mContractNum) ||
          (mEvmAddress && other.mEvmAddress && mEvmAddress->toBytes() == other.mEvmAddress->toBytes()) ||
          (!mContractNum && !other.mContractNum && !mEvmAddress && !other.mEvmAddress));
}

//-----
ContractId ContractId::fromProtobuf(const proto::ContractID& proto)
{
  ContractId contractId;
  contractId.mShardNum = static_cast<uint64_t>(proto.shardnum());
  contractId.mRealmNum = static_cast<uint64_t>(proto.realmnum());

  if (proto.contract_case() == proto::ContractID::ContractCase::kContractNum)
  {
    contractId.mContractNum = static_cast<uint64_t>(proto.contractnum());
  }
  else if (proto.contract_case() == proto::ContractID::ContractCase::kEvmAddress)
  {
    contractId.mEvmAddress = EvmAddress::fromBytes(internal::Utilities::stringToByteVector(proto.evm_address()));
  }

  return contractId;
}

//-----
std::unique_ptr<Key> ContractId::clone() const
{
  return std::make_unique<ContractId>(*this);
}

//-----
std::unique_ptr<proto::Key> ContractId::toProtobufKey() const
{
  auto proto = std::make_unique<proto::Key>();
  proto->set_allocated_contractid(toProtobuf().release());
  return proto;
}

//-----
std::unique_ptr<proto::ContractID> ContractId::toProtobuf() const
{
  auto proto = std::make_unique<proto::ContractID>();
  proto->set_shardnum(static_cast<int64_t>(mShardNum));
  proto->set_realmnum(static_cast<int64_t>(mRealmNum));

  if (mContractNum)
  {
    proto->set_contractnum(static_cast<int64_t>(*mContractNum));
  }
  else if (mEvmAddress)
  {
    proto->set_evm_address(internal::Utilities::byteVectorToString(mEvmAddress->toBytes()));
  }

  return proto;
}

//-----
std::string ContractId::toString() const
{
  std::string str = std::to_string(mShardNum) + '.' + std::to_string(mRealmNum) + '.';

  if (mContractNum)
  {
    return str + std::to_string(*mContractNum);
  }
  else if (mEvmAddress)
  {
    return str + mEvmAddress->toString();
  }
  else
  {
    // Uninitialized case
    return str + '0';
  }
}

//-----
ContractId& ContractId::setShardNum(const uint64_t& num)
{
  mShardNum = num;
  checkShardNum();
  return *this;
}

//-----
ContractId& ContractId::setRealmNum(const uint64_t& num)
{
  mRealmNum = num;
  checkRealmNum();
  return *this;
}

//-----
ContractId& ContractId::setContractNum(const uint64_t& num)
{
  mContractNum = num;
  checkContractNum();
  mEvmAddress.reset();
  return *this;
}

//-----
ContractId& ContractId::setEvmAddress(const EvmAddress& address)
{
  mEvmAddress = address;
  mContractNum.reset();
  return *this;
}

//-----
void ContractId::checkShardNum() const
{
  if (mShardNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input shard number is too large");
  }
}

//-----
void ContractId::checkRealmNum() const
{
  if (mRealmNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input realm number is too large");
  }
}

//-----
void ContractId::checkContractNum() const
{
  if (mContractNum && *mContractNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input contract number is too large");
  }
}

} // namespace Hedera

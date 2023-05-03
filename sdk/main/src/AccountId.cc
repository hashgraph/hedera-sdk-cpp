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
#include "AccountId.h"
#include "PublicKey.h"
#include "exceptions/BadKeyException.h"
#include "impl/Utilities.h"

#include <charconv>
#include <limits>
#include <proto/basic_types.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
AccountId::AccountId(const uint64_t& num)
  : mAccountNum(num)
{
  checkAccountNum();
}

//-----
AccountId::AccountId(const std::shared_ptr<PublicKey>& alias)
  : mPublicKeyAlias(alias)
{
}

//-----
AccountId::AccountId(const EvmAddress& address)
  : mEvmAddressAlias(address)
{
}

//-----
AccountId::AccountId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mAccountNum(num)
{
  checkShardNum();
  checkRealmNum();
  checkAccountNum();
}

//-----
AccountId::AccountId(const uint64_t& shard, const uint64_t& realm, const std::shared_ptr<PublicKey>& alias)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mPublicKeyAlias(alias)
{
  checkShardNum();
  checkRealmNum();
}

//-----
AccountId::AccountId(const uint64_t& shard, const uint64_t& realm, const EvmAddress& address)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mEvmAddressAlias(address)
{
  checkShardNum();
  checkRealmNum();
}

//-----
AccountId AccountId::fromString(std::string_view id)
{
  AccountId accountId;

  // Get the indices of the two delimiter '.'
  const size_t firstDot = id.find_first_of('.');
  const size_t secondDot = id.find_last_of('.');

  // Make sure there are at least two dots
  if (firstDot == secondDot)
  {
    throw std::invalid_argument("Input account ID string is malformed");
  }

  // Grab the three strings
  const std::string_view shardStr = id.substr(0, firstDot);
  const std::string_view realmStr = id.substr(firstDot + 1, secondDot - firstDot - 1);
  const std::string_view accountNumStr = id.substr(secondDot + 1, id.size() - secondDot - 1);

  // Convert the shard number
  auto result = std::from_chars(shardStr.data(), shardStr.data() + shardStr.size(), accountId.mShardNum);
  if (result.ec != std::errc() || result.ptr != shardStr.data() + shardStr.size())
  {
    throw std::invalid_argument("Input account ID string is malformed");
  }
  accountId.checkShardNum();

  // Convert the realm number
  result = std::from_chars(realmStr.data(), realmStr.data() + realmStr.size(), accountId.mRealmNum);
  if (result.ec != std::errc() || result.ptr != realmStr.data() + realmStr.size())
  {
    throw std::invalid_argument("Input account ID string is malformed");
  }
  accountId.checkRealmNum();

  // Determine what the input account number is. First determine if it is an alias (stringified DER-encoded PublicKey)
  try
  {
    accountId.mPublicKeyAlias = PublicKey::fromStringDer(accountNumStr);
    return accountId;
  }
  catch (const BadKeyException&)
  {
    // If not an alias, determine if it is an EVM address
    try
    {
      accountId.mEvmAddressAlias = EvmAddress::fromString(accountNumStr);
    }
    catch (const std::invalid_argument&)
    {
      // If not an EVM address, then treat as a normal account number
      uint64_t accountNum;
      result = std::from_chars(accountNumStr.data(), accountNumStr.data() + accountNumStr.size(), accountNum);
      if (result.ec != std::errc() || result.ptr != accountNumStr.data() + accountNumStr.size())
      {
        throw std::invalid_argument("Input account ID string is malformed");
      }
      accountId.mAccountNum = accountNum;
      accountId.checkAccountNum();
    }
  }

  return accountId;
}

//-----
bool AccountId::operator==(const AccountId& other) const
{
  return (mShardNum == other.mShardNum) && (mRealmNum == other.mRealmNum) &&
         ((mAccountNum && other.mAccountNum && mAccountNum == other.mAccountNum) ||
          (mPublicKeyAlias && other.mPublicKeyAlias &&
           mPublicKeyAlias->toStringDer() == other.mPublicKeyAlias->toStringDer()) ||
          (mEvmAddressAlias && other.mEvmAddressAlias &&
           mEvmAddressAlias->toString() == other.mEvmAddressAlias->toString()) ||
          (!mAccountNum && !other.mAccountNum && !mPublicKeyAlias && !other.mPublicKeyAlias && !mEvmAddressAlias &&
           !other.mEvmAddressAlias));
}

//-----
AccountId AccountId::fromProtobuf(const proto::AccountID& proto)
{
  AccountId accountId;
  accountId.mShardNum = static_cast<uint64_t>(proto.shardnum());
  accountId.mRealmNum = static_cast<uint64_t>(proto.realmnum());

  switch (proto.account_case())
  {
    case proto::AccountID::kAccountNum:
    {
      accountId.mAccountNum = static_cast<uint64_t>(proto.accountnum());
      break;
    }
    case proto::AccountID::kAlias:
    {
      if (proto.alias().size() == EvmAddress::NUM_BYTES)
      {
        accountId.mEvmAddressAlias = EvmAddress::fromBytes(internal::Utilities::stringToByteVector(proto.alias()));
      }
      else
      {
        try
        {
          accountId.mPublicKeyAlias = PublicKey::fromBytesDer(internal::Utilities::stringToByteVector(proto.alias()));
        }
        catch (const BadKeyException& ex)
        {
          std::cout << "Cannot decode AccountID protobuf alias: " << ex.what() << std::endl;
        }
      }
      break;
    }
    default:
      break;
  }

  return accountId;
}

//-----
std::unique_ptr<proto::AccountID> AccountId::toProtobuf() const
{
  auto proto = std::make_unique<proto::AccountID>();
  proto->set_shardnum(static_cast<int64_t>(mShardNum));
  proto->set_realmnum(static_cast<int64_t>(mRealmNum));

  if (mAccountNum)
  {
    proto->set_accountnum(static_cast<int64_t>(*mAccountNum));
  }
  else if (mPublicKeyAlias)
  {
    proto->set_alias(internal::Utilities::byteVectorToString(mPublicKeyAlias->toBytesRaw()));
  }
  else if (mEvmAddressAlias)
  {
    proto->set_alias(internal::Utilities::byteVectorToString(mEvmAddressAlias->toBytes()));
  }

  return proto;
}

//-----
std::string AccountId::toString() const
{
  std::string str = std::to_string(mShardNum) + '.' + std::to_string(mRealmNum) + '.';

  if (mAccountNum)
  {
    return str + std::to_string(*mAccountNum);
  }
  else if (mPublicKeyAlias)
  {
    return str + mPublicKeyAlias->toStringDer();
  }
  else if (mEvmAddressAlias)
  {
    return str + mEvmAddressAlias->toString();
  }
  else
  {
    // Uninitialized case
    return str + '0';
  }
}

//-----
AccountId& AccountId::setShardNum(const uint64_t& num)
{
  mShardNum = num;
  checkShardNum();
  return *this;
}

//-----
AccountId& AccountId::setRealmNum(const uint64_t& num)
{
  mRealmNum = num;
  checkRealmNum();
  return *this;
}

//-----
AccountId& AccountId::setAccountNum(const uint64_t& num)
{
  mAccountNum = num;
  checkAccountNum();

  mPublicKeyAlias = nullptr;
  mEvmAddressAlias.reset();
  return *this;
}

//-----
AccountId& AccountId::setPublicKeyAlias(const std::shared_ptr<PublicKey>& alias)
{
  mPublicKeyAlias = alias;
  mAccountNum.reset();
  mEvmAddressAlias.reset();
  return *this;
}

//-----
AccountId& AccountId::setEvmAddressAlias(const EvmAddress& address)
{
  mEvmAddressAlias = address;
  mAccountNum.reset();
  mPublicKeyAlias = nullptr;
  return *this;
}

//-----
void AccountId::checkShardNum() const
{
  if (mShardNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input shard number is too large");
  }
}

//-----
void AccountId::checkRealmNum() const
{
  if (mRealmNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input realm number is too large");
  }
}

//-----
void AccountId::checkAccountNum() const
{
  if (*mAccountNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input account number is too large");
  }
}

} // namespace Hedera

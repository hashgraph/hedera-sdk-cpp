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
AccountId::AccountId(const std::string& str)
{
  constexpr size_t numStrings = 3;              // Looking to make three strings (shard, realm, account number)
  std::vector<std::string> numbers(numStrings); // Create container in which to put the strings
  size_t curStringIndex = 0;                    // The index of the current string being built
  bool previousWasDot = true;                   // Keep track of if the previous character was a '.'

  // Make one pass over input string, constructing string values for shard, realm, and account number
  for (const char c : str)
  {
    if (c == '.')
    {
      if (previousWasDot)
      {
        // Two dots next to each other, or dot at beginning
        throw std::invalid_argument("AccountId string is malformed");
      }
      else
      {
        ++curStringIndex;
        previousWasDot = true;
      }
    }
    else
    {
      if (isdigit(c) && curStringIndex < numStrings)
      {
        numbers[curStringIndex].push_back(c);
        previousWasDot = false;
      }
      else
      {
        // Either not a digit or too many dots found
        throw std::invalid_argument("AccountId string is malformed");
      }
    }
  }

  // Make sure all numbers where constructed
  if (numbers.at(0).empty() || numbers.at(1).empty() || numbers.at(2).empty())
  {
    throw std::invalid_argument("AccountId string is malformed");
  }

  // Translate out_of_range exception to invalid_argument to allow for more descriptive exception
  try
  {
    mShardNum = std::stoll(numbers.at(0));
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("Input shard number is too big");
  }

  try
  {
    mRealmNum = std::stoll(numbers.at(1));
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("Input realm number is too big");
  }

  try
  {
    mAccountNum = std::stoll(numbers.at(2));
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("Input account number is too big");
  }

  // Make sure the numbers aren't too big
  checkShardNum();
  checkRealmNum();
  checkAccountNum();
}

//-----
bool AccountId::operator==(const AccountId& other) const
{
  return (mShardNum == other.mShardNum) && (mRealmNum == other.mRealmNum) && (mAccountNum == other.mAccountNum);
}

//-----
AccountId AccountId::fromProtobuf(const proto::AccountID& proto)
{
  return AccountId(static_cast<uint64_t>(proto.shardnum()),
                   static_cast<uint64_t>(proto.realmnum()),
                   static_cast<uint64_t>(proto.accountnum()));
}

//-----
std::unique_ptr<proto::AccountID> AccountId::toProtobuf() const
{
  auto proto = std::make_unique<proto::AccountID>();
  proto->set_shardnum(static_cast<int64_t>(mShardNum));
  proto->set_realmnum(static_cast<int64_t>(mRealmNum));
  proto->set_accountnum(static_cast<int64_t>(mAccountNum));

  return proto;
}

//-----
std::string AccountId::toString() const
{
  return std::to_string(mShardNum) + '.' + std::to_string(mRealmNum) + '.' + std::to_string(mAccountNum);
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
  if (mAccountNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input account number is too large");
  }
}

} // namespace Hedera

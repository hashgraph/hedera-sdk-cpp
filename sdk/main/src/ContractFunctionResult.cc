/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "ContractFunctionResult.h"

#include "contract_call_local.pb.h"

namespace Hedera
{
//-----
ContractFunctionResult::ContractFunctionResult()
  : mContractId()
  , mContractCallResult()
  , mErrorMessage(std::string())
  , mBloom(std::string())
  , mGasUsed(0ULL)
  , mLogInfo()
  , mEvmAddress()
  , mGas(0LL)
  , mAmount(0LL)
  , mFunctionParameters()
  , mSenderAccountId()
{
}

//-----
ContractFunctionResult::ContractFunctionResult(
  const proto::ContractFunctionResult& proto)
{
  if (proto.has_contractid())
  {
    mContractId.setValue(ContractId::fromProtobuf(proto.contractid()));
  }

  mContractCallResult = proto.contractcallresult();
  mErrorMessage = proto.errormessage();

  // Remove the function selector bytes if an exception was thrown
  if (!mErrorMessage.empty() &&
      mContractCallResult.find_first_of(ERROR_PREFIX) != 0)
  {
    mContractCallResult = mContractCallResult.substr(4);
  }

  mBloom = proto.bloom();
  mGasUsed = proto.gasused();

  for (size_t i = 0; i < proto.loginfo_size(); ++i)
  {
    mLogInfo.push_back(ContractLogInfo::fromProtobuf(proto.loginfo(i)));
  }

  if (proto.has_evm_address())
  {
    mEvmAddress.setValue(proto.evm_address().value());
  }

  mGas = proto.gas();
  mAmount = Hbar::fromTinybars(proto.amount());
  mFunctionParameters = proto.functionparameters();

  if (proto.has_sender_id())
  {
    mSenderAccountId.setValue(AccountId::fromProtobuf(proto.sender_id()));
  }
}

//-----
ContractFunctionResult
ContractFunctionResult::fromProtobuf(const proto::ContractFunctionResult& proto)
{
  return ContractFunctionResult(proto);
}

//-----
proto::ContractFunctionResult
ContractFunctionResult::toProtobuf() const
{
  proto::ContractFunctionResult proto;

  if (mContractId.isValid())
  {
    proto.set_allocated_contractid(mContractId.getValue().toProtobuf());
  }

  proto.set_contractcallresult(mContractCallResult);

  if (!mErrorMessage.empty())
  {
    proto.set_errormessage(mErrorMessage);
  }

  proto.set_bloom(mBloom);

  for (size_t i = 0; i < mLogInfo.size(); ++i)
  {
    proto::ContractLoginfo* logInfo = proto.add_loginfo();
    // TODO: set loginfo fields
  }

  if (mEvmAddress.isValid())
  {
    google::protobuf::BytesValue* value = new google::protobuf::BytesValue;
    value->set_allocated_value(new std::string(mEvmAddress.getValue()));
    proto.set_allocated_evm_address(value);
  }

  proto.set_gas(mGas);
  proto.set_amount(mAmount.toTinybars());
  proto.set_functionparameters(mFunctionParameters);

  if (mSenderAccountId.isValid())
  {
    proto.set_allocated_sender_id(mSenderAccountId.getValue().toProtobuf());
  }

  return proto;
}

//-----
std::string
ContractFunctionResult::getString(const size_t& index) const
{
  return getDynamicBytes(index);
}

//-----
std::vector<std::string>
ContractFunctionResult::getStringArray(const size_t& index) const
{
  const int offset = getInt32(index);
  const int count = getIntegerValueAt(offset);

  std::vector<std::string> strings;

  for (size_t i = 0; i < count; ++i)
  {
    const int strOffset = getIntegerValueAt(offset + 32 + (i * 32));
    const int len = getIntegerValueAt(offset + strOffset + 32);
    const std::string str =
      getByteString(offset + strOffset + 64, offset + strOffset + 64 + len);

    strings.push_back(str);
  }

  return strings;
}

//-----
bool
ContractFunctionResult::getBool(const size_t& index) const
{
  return getInt8(index) != 0;
}

//-----
int8_t
ContractFunctionResult::getInt8(const size_t& index) const
{
  if (mFunctionParameters.size() < 32 * index + 31)
  {
    // TODO: throw
  }

  return static_cast<int8_t>(mFunctionParameters.at(32 * index + 31));
}

//-----
int32_t
ContractFunctionResult::getInt32(const size_t& index) const
{
  return getIntegerValueAt(index * 32);
}

//-----
int64_t
ContractFunctionResult::getInt64(const size_t& index) const
{
  if (mFunctionParameters.size() < 32 * index + 24)
  {
    // TODO: throw
  }

  return *(
    reinterpret_cast<const int64_t*>(&mFunctionParameters.at(32 * index + 24)));
}

//-----
uint8_t
ContractFunctionResult::getUInt8(const size_t index) const
{
  return static_cast<uint8_t>(getInt8(index));
}

//-----
uint32_t
ContractFunctionResult::getUInt32(const size_t index) const
{
  return static_cast<uint32_t>(getInt32(index));
}

//-----
uint64_t
ContractFunctionResult::getUInt64(const size_t index) const
{
  return static_cast<uint64_t>(getInt64(index));
}

//-----
std::string
ContractFunctionResult::getAddress(const size_t index) const
{
  const int offset = index * 32;
  return getByteString(offset + 12, offset + 32);
}

//-----
std::string
ContractFunctionResult::getDynamicBytes(const size_t& index) const
{
  const int offset = getInt32(index);
  const int len = getIntegerValueAt(offset);
  return getByteString(offset + 32, offset + 32 + len);
}

//-----
int
ContractFunctionResult::getIntegerValueAt(const size_t& index) const
{
  if (mFunctionParameters.size() < 28 + index + sizeof(int))
  {
    // TODO: throw
  }

  return *(reinterpret_cast<const int*>(&mFunctionParameters.at(28 + index)));
}

//-----
std::string
ContractFunctionResult::getByteString(const size_t& start,
                                      const size_t& end) const
{
  return mFunctionParameters.substr(start, end - start);
}

} // namespace Hedera

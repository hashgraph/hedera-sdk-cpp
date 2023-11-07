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
#include "ContractFunctionResult.h"
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

#include <proto/contract_call_local.pb.h>

namespace Hedera
{
//-----
ContractFunctionResult ContractFunctionResult::fromProtobuf(const proto::ContractFunctionResult& proto)
{
  ContractFunctionResult contractFunctionResult;

  if (proto.has_contractid())
  {
    contractFunctionResult.mContractId = ContractId::fromProtobuf(proto.contractid());
  }

  contractFunctionResult.mContractCallResult = internal::Utilities::stringToByteVector(proto.contractcallresult());
  contractFunctionResult.mErrorMessage = proto.errormessage();

  // Remove the function selector bytes if an exception was thrown
  if (!contractFunctionResult.mErrorMessage.empty() &&
      internal::Utilities::isPrefixOf(contractFunctionResult.mContractCallResult, ERROR_PREFIX))
  {
    contractFunctionResult.mContractCallResult = internal::Utilities::removePrefix(
      contractFunctionResult.mContractCallResult, static_cast<long>(ERROR_PREFIX.size()));
  }

  contractFunctionResult.mBloom = internal::Utilities::stringToByteVector(proto.bloom());
  contractFunctionResult.mGasUsed = proto.gasused();

  for (int i = 0; i < proto.loginfo_size(); ++i)
  {
    contractFunctionResult.mLogs.push_back(ContractLogInfo::fromProtobuf(proto.loginfo(i)));
  }

  if (proto.has_evm_address())
  {
    contractFunctionResult.mEvmAddress =
      EvmAddress::fromBytes(internal::Utilities::stringToByteVector(proto.evm_address().value()));
  }

  contractFunctionResult.mGas = static_cast<uint64_t>(proto.gas());
  contractFunctionResult.mHbarAmount = Hbar(proto.amount(), HbarUnit::TINYBAR());
  contractFunctionResult.mFunctionParameters = internal::Utilities::stringToByteVector(proto.functionparameters());

  if (proto.has_sender_id())
  {
    contractFunctionResult.mSenderAccountId = AccountId::fromProtobuf(proto.sender_id());
  }

  for (int i = 0; i < proto.contract_nonces_size(); ++i)
  {
    contractFunctionResult.mContractNonces.push_back(ContractNonceInfo::fromProtobuf(proto.contract_nonces(i)));
  }

  return contractFunctionResult;
}

//-----
std::string ContractFunctionResult::getString(int index) const
{
  return internal::Utilities::byteVectorToString(getDynamicBytes(index));
}

//-----
std::vector<std::string> ContractFunctionResult::getStringArray(int index) const
{
  const int offset = getInt32(index);
  const int count = getIntValueAt(offset);
  std::vector<std::string> strings;

  for (int i = 0; i < count; ++i)
  {
    const int strOffset = getIntValueAt(offset + 32 + (i * 32));
    strings.push_back(internal::Utilities::byteVectorToString(
      getByteString(offset + strOffset + 64, offset + strOffset + 64 + getIntValueAt(offset + strOffset + 32))));
  }

  return strings;
}

//-----
std::vector<std::byte> ContractFunctionResult::getByteArray(int index) const
{
  return getDynamicBytes(index);
}

//-----
std::vector<std::byte> ContractFunctionResult::getBytes32(int index) const
{
  return getByteString(index * 32, (index + 1) * 32);
}

//-----
bool ContractFunctionResult::getBool(int index) const
{
  return getInt8(index) != 0;
}

//-----
int8_t ContractFunctionResult::getInt8(int index) const
{
  return static_cast<int8_t>(mContractCallResult.at((index * 32) + 31));
}

//-----
int32_t ContractFunctionResult::getInt32(int index) const
{
  return getIntValueAt(index * 32);
}

//-----
int64_t ContractFunctionResult::getInt64(int index) const
{
  const int64_t value = *internal::Utilities::toTypePtr<int64_t>(&mContractCallResult.at((index * 32) + 24));
  return internal::Utilities::swapEndianness(value);
}

//-----
uint8_t ContractFunctionResult::getUint8(int index) const
{
  return static_cast<uint8_t>(getInt8(index));
}

//-----
uint32_t ContractFunctionResult::getUint32(int index) const
{
  return static_cast<uint32_t>(getInt32(index));
}

//-----
uint64_t ContractFunctionResult::getUint64(int index) const
{
  return static_cast<uint64_t>(getInt64(index));
}

//-----
std::string ContractFunctionResult::getAddress(int index) const
{
  return internal::HexConverter::bytesToHex(getByteString(index * 32 + 12, (index + 1) * 32));
}

//-----
std::vector<std::byte> ContractFunctionResult::getDynamicBytes(int index) const
{
  const int offset = getInt32(index);
  return getByteString(offset + 32, offset + 32 + getIntValueAt(offset));
}

//-----
int ContractFunctionResult::getIntValueAt(int offset) const
{
  const int value = *internal::Utilities::toTypePtr<int>(&mContractCallResult.at(offset + 28));
  return internal::Utilities::swapEndianness(value);
}

//-----
std::vector<std::byte> ContractFunctionResult::getByteString(int start, int end) const
{
  return { mContractCallResult.cbegin() + start, mContractCallResult.cbegin() + end };
}

} // namespace Hedera

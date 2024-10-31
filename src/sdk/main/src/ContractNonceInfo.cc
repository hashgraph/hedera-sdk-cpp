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
#include "ContractNonceInfo.h"
#include "impl/Utilities.h"

#include <contract_types.pb.h>

namespace Hedera
{
//-----
ContractNonceInfo::ContractNonceInfo(ContractId contractId, int64_t nonce)
  : mContractId(std::move(contractId))
  , mNonce(nonce)
{
}

//-----
bool ContractNonceInfo::operator==(const ContractNonceInfo& other) const
{
  return (mContractId == other.mContractId) && (mNonce == other.mNonce);
}

//-----
ContractNonceInfo ContractNonceInfo::fromProtobuf(const proto::ContractNonceInfo& proto)
{
  return { ContractId::fromProtobuf(proto.contract_id()), proto.nonce() };
}

//-----
ContractNonceInfo ContractNonceInfo::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::ContractNonceInfo protoContractNonceInfo;
  protoContractNonceInfo.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(protoContractNonceInfo);
}

//-----
std::unique_ptr<proto::ContractNonceInfo> ContractNonceInfo::toProtobuf() const
{
  auto protoContractNonceInfo = std::make_unique<proto::ContractNonceInfo>();
  protoContractNonceInfo->set_allocated_contract_id(mContractId.toProtobuf().release());
  protoContractNonceInfo->set_nonce(mNonce);
  return protoContractNonceInfo;
}

//-----
std::vector<std::byte> ContractNonceInfo::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string ContractNonceInfo::toString() const
{
  std::string str = mContractId.toString();

  if (mNonce)
  {
    str += '.' + std::to_string(mNonce);
  }

  return str;
}

//-----
std::ostream& operator<<(std::ostream& os, const ContractNonceInfo& nonceInfo)
{
  os << nonceInfo.toString();
  return os;
}

} // namespace Hedera

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
#include "ContractLogInfo.h"
#include "impl/Utilities.h"

#include <proto/contract_call_local.pb.h>

namespace Hedera
{
//-----
ContractLogInfo ContractLogInfo::fromProtobuf(const proto::ContractLoginfo& proto)
{
  ContractLogInfo contractLogInfo;
  contractLogInfo.mContractId = ContractId::fromProtobuf(proto.contractid());
  contractLogInfo.mBloom = internal::Utilities::stringToByteVector(proto.bloom());

  for (int i = 0; i < proto.topic_size(); ++i)
  {
    contractLogInfo.mTopics.push_back(internal::Utilities::stringToByteVector(proto.topic(i)));
  }

  contractLogInfo.mData = internal::Utilities::stringToByteVector(proto.data());
  return contractLogInfo;
}

//-----
std::unique_ptr<proto::ContractLoginfo> ContractLogInfo::toProtobuf() const
{
  auto proto = std::make_unique<proto::ContractLoginfo>();
  proto->set_allocated_contractid(mContractId.toProtobuf().release());
  proto->set_bloom(internal::Utilities::byteVectorToString(mBloom));

  for (const auto& topic : mTopics)
  {
    proto->add_topic(internal::Utilities::byteVectorToString(topic));
  }

  proto->set_data(internal::Utilities::byteVectorToString(mData));
  return proto;
}

} // namespace Hedera

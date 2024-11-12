/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "TransactionFeeSchedule.h"
#include "impl/Utilities.h"

#include "nlohmann/json.hpp"
#include <basic_types.pb.h>

namespace Hiero
{
//-----
TransactionFeeSchedule TransactionFeeSchedule::fromProtobuf(const proto::TransactionFeeSchedule& proto)
{
  TransactionFeeSchedule transactionFeeSchedule;
  transactionFeeSchedule.setRequestType(gProtobufHederaFunctionalityToRequestType.at(proto.hederafunctionality()));

  for (int i = 0; i < proto.fees_size(); ++i)
  {
    transactionFeeSchedule.addFee(FeeData::fromProtobuf(proto.fees(i)));
  }

  return transactionFeeSchedule;
}

//-----
TransactionFeeSchedule TransactionFeeSchedule::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::TransactionFeeSchedule proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::TransactionFeeSchedule> TransactionFeeSchedule::toProtobuf() const
{
  auto proto = std::make_unique<proto::TransactionFeeSchedule>();
  proto->set_hederafunctionality(gRequestTypeToProtobufHederaFunctionality.at(mRequestType));
  std::for_each(
    mFees.cbegin(), mFees.cend(), [&proto](const FeeData& feeData) { *proto->add_fees() = *feeData.toProtobuf(); });
  return proto;
}

//-----
std::vector<std::byte> TransactionFeeSchedule::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string TransactionFeeSchedule::toString() const
{
  nlohmann::json json;
  json["mRequestType"] = gRequestTypeToString.at(mRequestType);
  std::for_each(
    mFees.cbegin(), mFees.cend(), [&json](const FeeData& feeData) { json["mFees"].push_back(feeData.toString()); });
  return json.dump();
}

} // namespace Hiero

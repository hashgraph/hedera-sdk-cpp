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
#include "FeeSchedules.h"
#include "impl/Utilities.h"

#include <nlohmann/json.hpp>
#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
FeeSchedules FeeSchedules::fromProtobuf(const proto::CurrentAndNextFeeSchedule& proto)
{
  FeeSchedules feeSchedules;

  if (proto.has_currentfeeschedule())
  {
    feeSchedules.mCurrentFeeSchedule = FeeSchedule::fromProtobuf(proto.currentfeeschedule());
  }

  if (proto.has_nextfeeschedule())
  {
    feeSchedules.mNextFeeSchedule = FeeSchedule::fromProtobuf(proto.nextfeeschedule());
  }

  return feeSchedules;
}

//-----
FeeSchedules FeeSchedules::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::CurrentAndNextFeeSchedule proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::CurrentAndNextFeeSchedule> FeeSchedules::toProtobuf() const
{
  auto proto = std::make_unique<proto::CurrentAndNextFeeSchedule>();
  proto->set_allocated_currentfeeschedule(mCurrentFeeSchedule.toProtobuf().release());
  proto->set_allocated_nextfeeschedule(mNextFeeSchedule.toProtobuf().release());
  return proto;
}

//-----
std::vector<std::byte> FeeSchedules::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string FeeSchedules::toString() const
{
  nlohmann::json json;
  json["mCurrentFeeSchedule"] = mCurrentFeeSchedule.toString();
  json["mNextFeeSchedule"] = mNextFeeSchedule.toString();
  return json.dump();
}

} // namespace Hedera

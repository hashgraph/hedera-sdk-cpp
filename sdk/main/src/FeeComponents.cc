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
#include "FeeComponents.h"
#include "impl/Utilities.h"

#include <nlohmann/json.hpp>
#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
FeeComponents FeeComponents::fromProtobuf(const proto::FeeComponents& proto)
{
  return FeeComponents()
    .setMin(proto.min())
    .setMax(proto.max())
    .setConstant(proto.constant())
    .setTransactionBandwidthBytes(proto.bpt())
    .setTransactionVerification(proto.vpt())
    .setTransactionRamByteHour(proto.rbh())
    .setTransactionStorageByteHour(proto.sbh())
    .setContractTransactionGas(proto.gas())
    .setTransferVolumeHbar(proto.tv())
    .setResponseMemoryByte(proto.bpr())
    .setResponseDiskByte(proto.sbpr());
}

//-----
FeeComponents FeeComponents::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::FeeComponents proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::FeeComponents> FeeComponents::toProtobuf() const
{
  auto proto = std::make_unique<proto::FeeComponents>();
  proto->set_min(mMin);
  proto->set_max(mMax);
  proto->set_constant(mConstant);
  proto->set_bpt(mTransactionBandwidthBytes);
  proto->set_vpt(mTransactionVerification);
  proto->set_rbh(mTransactionRamByteHour);
  proto->set_sbh(mTransactionStorageByteHour);
  proto->set_gas(mContractTransactionGas);
  proto->set_tv(mTransferVolumeHbar);
  proto->set_bpr(mResponseMemoryByte);
  proto->set_sbpr(mResponseDiskByte);
  return proto;
}

//-----
std::vector<std::byte> FeeComponents::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string FeeComponents::toString() const
{
  nlohmann::json json;
  json["mMin"] = mMin;
  json["mMax"] = mMax;
  json["mConstant"] = mConstant;
  json["mTransactionBandwidthBytes"] = mTransactionBandwidthBytes;
  json["mTransactionVerification"] = mTransactionVerification;
  json["mTransactionRamByteHour"] = mTransactionRamByteHour;
  json["mTransactionStorageByteHour"] = mTransactionStorageByteHour;
  json["mContractTransactionGas"] = mContractTransactionGas;
  json["mTransferVolumeHbar"] = mTransferVolumeHbar;
  json["mResponseMemoryByte"] = mResponseMemoryByte;
  json["mResponseDiskByte"] = mResponseDiskByte;
  return json.dump();
}

} // namespace Hedera

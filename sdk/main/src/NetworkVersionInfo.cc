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
#include "NetworkVersionInfo.h"
#include "impl/Utilities.h"

#include <proto/network_get_version_info.pb.h>

namespace Hedera
{
//-----
NetworkVersionInfo::NetworkVersionInfo(const SemanticVersion& hapi, const SemanticVersion& hedera)
  : mProtobufVersion(hapi)
  , mServicesVersion(hedera)
{
}

//-----
NetworkVersionInfo NetworkVersionInfo::fromProtobuf(const proto::NetworkGetVersionInfoResponse& proto)
{
  return NetworkVersionInfo(SemanticVersion::fromProtobuf(proto.hapiprotoversion()),
                            SemanticVersion::fromProtobuf(proto.hederaservicesversion()));
}

//-----
NetworkVersionInfo NetworkVersionInfo::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::NetworkGetVersionInfoResponse networkGetVersionInfoResponse;
  networkGetVersionInfoResponse.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(networkGetVersionInfoResponse);
}

//-----
std::unique_ptr<proto::NetworkGetVersionInfoResponse> NetworkVersionInfo::toProtobuf() const
{
  auto proto = std::make_unique<proto::NetworkGetVersionInfoResponse>();
  proto->set_allocated_hapiprotoversion(mProtobufVersion.toProtobuf().release());
  proto->set_allocated_hederaservicesversion(mServicesVersion.toProtobuf().release());
  return proto;
}

//-----
std::vector<std::byte> NetworkVersionInfo::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

} // namespace Hedera

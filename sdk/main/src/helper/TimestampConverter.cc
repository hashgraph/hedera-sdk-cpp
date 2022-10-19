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
#include "helper/TimestampConverter.h"

#include <proto/timestamp.pb.h>

namespace Hedera
{
namespace TimestampConverter
{
//-----
std::chrono::sys_time<std::chrono::duration<double>> fromProtobuf(const proto::Timestamp& timestamp)
{
  return std::chrono::sys_time<std::chrono::nanoseconds>(
    std::chrono::nanoseconds((timestamp.seconds() * 1000000000) + timestamp.nanos()));
}

//-----
std::shared_ptr<proto::Timestamp> toProtobuf(const std::chrono::sys_time<std::chrono::duration<double>>& time)
{
  proto::Timestamp timestamp;
  std::chrono::nanoseconds nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch());
  timestamp.set_seconds(nanos.count() / 1000000000);
  timestamp.set_nanos(nanos.count() % 1000000000);
  return std::make_shared<proto::Timestamp>(timestamp);
}

} // namespace InstantConverter
} // namespace Hedera

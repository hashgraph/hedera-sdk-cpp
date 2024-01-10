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
#include "impl/TimestampConverter.h"

#include <proto/timestamp.pb.h>

namespace Hedera::internal::TimestampConverter
{
//-----
std::chrono::system_clock::time_point fromProtobuf(const proto::Timestamp& timestamp)
{
  // Convert the nanoseconds to a point in time using the precision of the system's clock
  return std::chrono::system_clock::time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(
    std::chrono::nanoseconds(timestamp.seconds() * 1000000000 + timestamp.nanos())));
}

//-----
std::chrono::system_clock::time_point fromProtobuf(const proto::TimestampSeconds& timestamp)
{
  return std::chrono::system_clock::time_point(std::chrono::seconds(timestamp.seconds()));
}

//-----
proto::Timestamp* toProtobuf(const std::chrono::system_clock::time_point& time)
{
  auto timestamp = std::make_unique<proto::Timestamp>();
  const std::chrono::nanoseconds nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch());
  timestamp->set_seconds(nanos.count() / 1000000000);
  timestamp->set_nanos(nanos.count() % 1000000000);
  return timestamp.release();
}

//-----
proto::TimestampSeconds* toSecondsProtobuf(const std::chrono::system_clock::time_point& time)
{
  auto timestamp = std::make_unique<proto::TimestampSeconds>();
  timestamp->set_seconds(std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count());
  return timestamp.release();
}

//-----
std::string toString(const std::chrono::system_clock::time_point& timestamp)
{
  const std::chrono::system_clock::duration durationSinceEpoch = timestamp.time_since_epoch();
  const std::chrono::seconds secondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch);

  return std::to_string(secondsSinceEpoch.count()) + '.' +
         std::to_string(
           std::chrono::duration_cast<std::chrono::nanoseconds>(durationSinceEpoch - secondsSinceEpoch).count());
}

} // namespace Hedera::internal::TimestampConverter

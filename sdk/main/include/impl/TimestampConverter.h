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
#ifndef HEDERA_SDK_CPP_IMPL_TIMESTAMP_CONVERTER_H_
#define HEDERA_SDK_CPP_IMPL_TIMESTAMP_CONVERTER_H_

#include <chrono>
#include <memory>

namespace proto
{
class Timestamp;
class TimestampSeconds;
}

namespace Hedera::internal::TimestampConverter
{
/**
 * Create a system time object from a Timestamp protobuf.
 *
 * @param timestamp The Timestamp protobuf.
 * @return The system time represented by the input Timestamp protobuf.
 */
std::chrono::system_clock::time_point fromProtobuf(const proto::Timestamp& timestamp);

/**
 * Create a system time object from a TimestampSeconds protobuf.
 *
 * @param timestamp The TimestampSeconds protobuf.
 * @return The system time represented by the input TimestampSeconds protobuf.
 */
std::chrono::system_clock::time_point fromProtobuf(const proto::TimestampSeconds& timestamp);

/**
 * Create a Timestamp protobuf from a system time.
 *
 * @param time The system time.
 * @return The Timestamp protobuf represented by the input system time.
 */
proto::Timestamp* toProtobuf(const std::chrono::system_clock::time_point& time);

} // namespace Hedera::internal::TimestampConverter

#endif // HEDERA_SDK_CPP_IMPL_TIMESTAMP_CONVERTER_H_

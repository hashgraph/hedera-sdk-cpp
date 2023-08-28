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
 * Get the time point represented by a Timestamp protobuf object.
 *
 * @param timestamp The Timestamp protobuf from which to get the time point.
 * @return The time point represented by the input Timestamp protobuf.
 */
std::chrono::system_clock::time_point fromProtobuf(const proto::Timestamp& timestamp);

/**
 * Get the time point represented by a TimestampSeconds protobuf object.
 *
 * @param timestamp The TimestampSeconds protobuf from which to get the time point.
 * @return The time point represented by the input TimestampSeconds protobuf.
 */
std::chrono::system_clock::time_point fromProtobuf(const proto::TimestampSeconds& timestamp);

/**
 * Create a Timestamp protobuf object from a time point.
 *
 * @param duration The duration object from which to create a Timestamp protobuf object.
 * @return A pointer to the created Timestamp protobuf object.
 */
proto::Timestamp* toProtobuf(const std::chrono::system_clock::time_point& time);

} // namespace Hedera::internal::TimestampConverter

#endif // HEDERA_SDK_CPP_IMPL_TIMESTAMP_CONVERTER_H_

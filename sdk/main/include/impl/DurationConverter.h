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
#ifndef HEDERA_SDK_CPP_IMPL_DURATION_CONVERTER_H_
#define HEDERA_SDK_CPP_IMPL_DURATION_CONVERTER_H_

#include <chrono>
#include <memory>

namespace proto
{
class Duration;
}

namespace Hedera::internal::DurationConverter
{
/**
 * Get the duration in seconds represented by a Duration protobuf object.
 *
 * @param duration The Duration protobuf object from which to get the duration.
 * @return The duration in seconds of the input Duration protobuf object.
 */
std::chrono::seconds fromProtobuf(const proto::Duration& duration);

/**
 * Create a Duration protobuf object from a generic duration.
 *
 * @param duration The duration object from which to create a Duration protobuf object.
 * @return A pointer to the created Duration protobuf object.
 */
proto::Duration* toProtobuf(const std::chrono::system_clock::duration& duration);

} // namespace Hedera::internal::DurationConverter

#endif // HEDERA_SDK_CPP_IMPL_DURATION_CONVERTER_H_

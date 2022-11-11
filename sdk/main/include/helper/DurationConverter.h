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
#ifndef HELPER_DURATION_CONVERTER_H_
#define HELPER_DURATION_CONVERTER_H_

#include <chrono>
#include <memory>

namespace proto
{
class Duration;
}

namespace Hedera::DurationConverter
{
/**
 * Create a duration object from a protobuf.
 *
 * @param duration The duration protobuf.
 * @return The duration represented by the input duration.
 */
std::chrono::seconds fromProtobuf(const proto::Duration& duration);

/**
 * Create a duration protobuf object from a duration object.
 *
 * @param duration The duration object.
 * @return The duration protobuf object.
 */
proto::Duration* toProtobuf(const std::chrono::duration<int64_t>& duration);

} // namespace Hedera::DurationConverter

#endif // HELPER_DURATION_CONVERTER_H_

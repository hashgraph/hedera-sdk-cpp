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
#include "impl/DurationConverter.h"

#include <proto/duration.pb.h>

namespace Hedera::internal::DurationConverter
{
//----
std::chrono::seconds fromProtobuf(const proto::Duration& duration)
{
  return std::chrono::seconds(duration.seconds());
}

//-----
proto::Duration* toProtobuf(const std::chrono::duration<double>& duration)
{
  auto proto = std::make_unique<proto::Duration>();
  proto->set_seconds(std::chrono::duration_cast<std::chrono::seconds>(duration).count());
  return proto.release();
}

} // namespace Hedera::internal::DurationConverter

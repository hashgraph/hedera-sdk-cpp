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
#include "helper/DurationConverter.h"

#include <proto/duration.pb.h>

namespace Hedera
{
namespace DurationConverter
{
//----
std::chrono::seconds fromProtobuf(const proto::Duration& duration)
{
  return std::chrono::seconds(duration.seconds());
}

//-----
std::shared_ptr<proto::Duration> toProtobuf(const std::chrono::seconds& duration)
{
  std::shared_ptr<proto::Duration> proto = std::make_shared<proto::Duration>();
  proto->set_seconds(duration.count());
  return proto;
}

} // namespace DurationConverter
} // namespace Hedera

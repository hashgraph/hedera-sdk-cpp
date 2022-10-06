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
#ifndef STAKING_INFO_H_
#define STAKING_INFO_H_

/**
 * Protobuf includes
 */
#include "basic_types.pb.h"

namespace Hedera
{
class StakingInfo
{
public:
  static StakingInfo fromProtobuf(const proto::StakingInfo& proto)
  {
    return StakingInfo();
  }

  proto::StakingInfo* toProtobuf() const { return new proto::StakingInfo(); }
};
} // namespace Hedera

#endif // STAKING_INFO_H_
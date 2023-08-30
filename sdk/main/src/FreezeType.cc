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
#include "FreezeType.h"

#include <proto/freeze_type.pb.h>

namespace Hedera
{
//-----
const std::unordered_map<proto::FreezeType, FreezeType> gProtobufFreezeTypeToFreezeType = {
  {proto::FreezeType::UNKNOWN_FREEZE_TYPE, FreezeType::UNKNOWN_FREEZE_TYPE},
  { proto::FreezeType::FREEZE_ONLY,        FreezeType::FREEZE_ONLY        },
  { proto::FreezeType::PREPARE_UPGRADE,    FreezeType::PREPARE_UPGRADE    },
  { proto::FreezeType::FREEZE_UPGRADE,     FreezeType::FREEZE_UPGRADE     },
  { proto::FreezeType::FREEZE_ABORT,       FreezeType::FREEZE_ABORT       },
  { proto::FreezeType::TELEMETRY_UPGRADE,  FreezeType::TELEMETRY_UPGRADE  },
};

//-----
const std::unordered_map<FreezeType, proto::FreezeType> gFreezeTypeToProtobufFreezeType = {
  {FreezeType::UNKNOWN_FREEZE_TYPE, proto::FreezeType::UNKNOWN_FREEZE_TYPE},
  { FreezeType::FREEZE_ONLY,        proto::FreezeType::FREEZE_ONLY        },
  { FreezeType::PREPARE_UPGRADE,    proto::FreezeType::PREPARE_UPGRADE    },
  { FreezeType::FREEZE_UPGRADE,     proto::FreezeType::FREEZE_UPGRADE     },
  { FreezeType::FREEZE_ABORT,       proto::FreezeType::FREEZE_ABORT       },
  { FreezeType::TELEMETRY_UPGRADE,  proto::FreezeType::TELEMETRY_UPGRADE  },
};

//-----
const std::unordered_map<FreezeType, const char*> gFreezeTypeToString = {
  {FreezeType::UNKNOWN_FREEZE_TYPE, "UNKNOWN_FREEZE_TYPE"},
  { FreezeType::FREEZE_ONLY,        "FREEZE_ONLY"        },
  { FreezeType::PREPARE_UPGRADE,    "PREPARE_UPGRADE"    },
  { FreezeType::FREEZE_UPGRADE,     "FREEZE_UPGRADE"     },
  { FreezeType::FREEZE_ABORT,       "FREEZE_ABORT"       },
  { FreezeType::TELEMETRY_UPGRADE,  "TELEMETRY_UPGRADE"  },
};

} // namespace Hedera

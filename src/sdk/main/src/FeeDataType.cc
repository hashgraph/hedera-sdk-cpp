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
#include "FeeDataType.h"

#include <basic_types.pb.h>

namespace Hedera
{
//-----
const std::unordered_map<proto::SubType, FeeDataType> gProtobufSubTypeToFeeDataType = {
  { proto::SubType::DEFAULT,                                    FeeDataType::DEFAULT                                },
  { proto::SubType::TOKEN_FUNGIBLE_COMMON,                      FeeDataType::TOKEN_FUNGIBLE_COMMON                  },
  { proto::SubType::TOKEN_NON_FUNGIBLE_UNIQUE,                  FeeDataType::TOKEN_NON_FUNGIBLE_UNIQUE              },
  { proto::SubType::TOKEN_FUNGIBLE_COMMON_WITH_CUSTOM_FEES,     FeeDataType::TOKEN_FUNGIBLE_COMMON_WITH_CUSTOM_FEES },
  { proto::SubType::TOKEN_NON_FUNGIBLE_UNIQUE_WITH_CUSTOM_FEES,
   FeeDataType::TOKEN_NON_FUNGIBLE_UNIQUE_WITH_CUSTOM_FEES                                                          },
  { proto::SubType::SCHEDULE_CREATE_CONTRACT_CALL,              FeeDataType::SCHEDULE_CREATE_CONTRACT_CALL          }
};

//-----
const std::unordered_map<FeeDataType, proto::SubType> gFeeDataTypeToProtobufSubType = {
  { FeeDataType::DEFAULT,                                    proto::SubType::DEFAULT                                },
  { FeeDataType::TOKEN_FUNGIBLE_COMMON,                      proto::SubType::TOKEN_FUNGIBLE_COMMON                  },
  { FeeDataType::TOKEN_NON_FUNGIBLE_UNIQUE,                  proto::SubType::TOKEN_NON_FUNGIBLE_UNIQUE              },
  { FeeDataType::TOKEN_FUNGIBLE_COMMON_WITH_CUSTOM_FEES,     proto::SubType::TOKEN_FUNGIBLE_COMMON_WITH_CUSTOM_FEES },
  { FeeDataType::TOKEN_NON_FUNGIBLE_UNIQUE_WITH_CUSTOM_FEES,
   proto::SubType::TOKEN_NON_FUNGIBLE_UNIQUE_WITH_CUSTOM_FEES                                                       },
  { FeeDataType::SCHEDULE_CREATE_CONTRACT_CALL,              proto::SubType::SCHEDULE_CREATE_CONTRACT_CALL          },
};

//-----
const std::unordered_map<FeeDataType, std::string> gFeeDataTypeToString = {
  { FeeDataType::DEFAULT,                                    "DEFAULT"                                    },
  { FeeDataType::TOKEN_FUNGIBLE_COMMON,                      "TOKEN_FUNGIBLE_COMMON"                      },
  { FeeDataType::TOKEN_NON_FUNGIBLE_UNIQUE,                  "TOKEN_NON_FUNGIBLE_UNIQUE"                  },
  { FeeDataType::TOKEN_FUNGIBLE_COMMON_WITH_CUSTOM_FEES,     "TOKEN_FUNGIBLE_COMMON_WITH_CUSTOM_FEES"     },
  { FeeDataType::TOKEN_NON_FUNGIBLE_UNIQUE_WITH_CUSTOM_FEES, "TOKEN_NON_FUNGIBLE_UNIQUE_WITH_CUSTOM_FEES" },
  { FeeDataType::SCHEDULE_CREATE_CONTRACT_CALL,              "SCHEDULE_CREATE_CONTRACT_CALL"              },
};

} // namespace Hedera

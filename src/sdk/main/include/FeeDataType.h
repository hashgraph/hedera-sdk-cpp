/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#ifndef HIERO_SDK_CPP_FEE_DATA_TYPE_H_
#define HIERO_SDK_CPP_FEE_DATA_TYPE_H_

#include <string>
#include <unordered_map>

namespace proto
{
enum SubType : int;
}

namespace Hiero
{
/**
 * Enumeration of fee data types.
 */
enum class FeeDataType
{
  /**
   * The resource prices have no special scope.
   */
  DEFAULT,
  /**
   * The resource prices are scoped to an operation on a fungible common token.
   */
  TOKEN_FUNGIBLE_COMMON,
  /**
   * The resource prices are scoped to an operation on a non-fungible unique token.
   */
  TOKEN_NON_FUNGIBLE_UNIQUE,
  /**
   * The resource prices are scoped to an operation on a fungible common token with a custom fee schedule.
   */
  TOKEN_FUNGIBLE_COMMON_WITH_CUSTOM_FEES,
  /**
   * The resource prices are scoped to an operation on a non-fungible unique token with a custom fee schedule.
   */
  TOKEN_NON_FUNGIBLE_UNIQUE_WITH_CUSTOM_FEES,
  /**
   * The resource prices are scoped to a ScheduleCreate containing a ContractCall.
   */
  SCHEDULE_CREATE_CONTRACT_CALL
};

/**
 * Map of protobuf SubTypes to their corresponding FeeDataTypes.
 */
extern const std::unordered_map<proto::SubType, FeeDataType> gProtobufSubTypeToFeeDataType;

/**
 * Map of FeeDataTypes to their corresponding protobuf SubTypes.
 */
extern const std::unordered_map<FeeDataType, proto::SubType> gFeeDataTypeToProtobufSubType;

/**
 * Map of FeeDataType to its corresponding string.
 */
[[maybe_unused]] extern const std::unordered_map<FeeDataType, std::string> gFeeDataTypeToString;

} // namespace Hiero

#endif // HIERO_SDK_CPP_FEE_DATA_TYPE_H_

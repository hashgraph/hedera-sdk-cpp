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
#ifndef HEDERA_SDK_CPP_FEE_ASSESSMENT_METHOD_H_
#define HEDERA_SDK_CPP_FEE_ASSESSMENT_METHOD_H_

#include <string>
#include <unordered_map>

namespace Hedera
{
/**
 * An enumeration the describes how a fee should be assessed.
 */
enum class FeeAssessmentMethod
{
  /**
   * If Alice is paying Bob and an INCLUSIVE fractional fee is collected to be sent to Charlie, the amount Alice
   * declares she will pay in the transfer transaction includes the fee amount. In other words, Bob receives the amount
   * that Alice intended to send minus the fee.
   */
  INCLUSIVE,
  /**
   * If Alice is paying Bob, and an EXCLUSIVE fractional fee is collected to be sent to Charlie, the amount Alice
   * declares she will pay in the transfer transaction does not include the fee amount. In other words, Alice is charged
   * the fee in addition to the amount she intended to send to Bob.
   */
  EXCLUSIVE
};

/**
 * Map of FeeAssessmentMethod to its corresponding string.
 */
const std::unordered_map<FeeAssessmentMethod, std::string> gFeeAssessmentMethodToString = {
  {FeeAssessmentMethod::INCLUSIVE,  "INCLUSIVE"},
  { FeeAssessmentMethod::EXCLUSIVE, "EXCLUSIVE"}
};

/**
 * Map of FeeAssessmentMethod string representation to its corresponding enum value.
 */
[[maybe_unused]] const std::unordered_map<std::string, FeeAssessmentMethod> gStringToFeeAssessmentMethod = {
  {"INCLUSIVE",  FeeAssessmentMethod::INCLUSIVE},
  { "EXCLUSIVE", FeeAssessmentMethod::EXCLUSIVE}
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_FEE_ASSESSMENT_METHOD_H_

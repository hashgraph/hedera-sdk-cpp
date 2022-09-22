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
#ifndef CONTRACT_FUNCTION_PARAMETERS_H_
#define CONTRACT_FUNCTION_PARAMETERS_H_

namespace Hedera
{
class ContractFunctionParameters
{
public:
  ContractFunctionParameters() {}

  std::string toByteArrayWithName(const std::string& name) const
  {
    return std::string();
  }
};

} // namespace Hedera

#endif // CONTRACT_FUNCTION_PARAMETERS_H_
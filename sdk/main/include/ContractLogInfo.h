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
#ifndef CONTRACT_LOG_INFO_H_
#define CONTRACT_LOG_INFO_H_

namespace proto
{
class ContractLoginfo;
} // namespace proto

namespace Hedera
{
class ContractLogInfo
{
public:
  static ContractLogInfo fromProtobuf(const proto::ContractLoginfo& proto)
  {
    return ContractLogInfo();
  }
};
} // namespace Hedera

#endif // CONTRACT_LOG_INFO_H_
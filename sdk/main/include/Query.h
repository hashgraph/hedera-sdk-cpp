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
#ifndef HEDERA_SDK_CPP_QUERY_H_
#define HEDERA_SDK_CPP_QUERY_H_

#include "Executable.h"

namespace proto
{
class Query;
class Response;
}

namespace Hedera
{
/**
 * Base class for all queries that can be submitted to Hedera.
 *
 * @tparam SdkRequestType  The SDK request type.
 * @tparam SdkResponseType The SDK response type.
 */
template<typename SdkRequestType, typename SdkResponseType>
class Query : public Executable<SdkRequestType, proto::Query, proto::Response, SdkResponseType>
{
public:
  ~Query() override = default;

protected:
  /**
   * Prevent public copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  Query() = default;
  Query(const Query&) = default;
  Query& operator=(const Query&) = default;
  Query(Query&&) noexcept = default;
  Query& operator=(Query&&) noexcept = default;

  /**
   * Derived from Executable. Perform any operations needed when this Query is being executed.
   */
  void onExecute(const Client&) override
  { // Intentionally unimplemented
  }
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_QUERY_H_

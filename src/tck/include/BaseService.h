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
#ifndef HEDERA_TCK_CPP_BASE_SERVICE_H_
#define HEDERA_TCK_CPP_BASE_SERVICE_H_

#include "SdkClient.h"

#include <memory>

namespace Hedera::TCK
{
/**
 * Base class for SDK services that abstracts away SDK client usage.
 */
class BaseService
{
public:
  /**
   * Set the SDK client to use to submit and execute requests.
   *
   * @param client The SDK client to set for this AccountService to use.
   */
  void setSdkClient(const std::shared_ptr<SdkClient>& client);

protected:
  /**
   * Get the SDK client to use to submit and execute requests.
   *
   * @return Pointer to the SDK client.
   */
  [[nodiscard]] std::shared_ptr<SdkClient> getSdkClient() const { return mClient; }

private:
  /**
   * The SDK client to use to submit and execute requests. shared_ptr allows the SDK client to be used by multiple
   * different services.
   */
  std::shared_ptr<SdkClient> mClient;
};

} // namespace Hedera::TCK

#endif // HEDERA_TCK_CPP_BASE_SERVICE_H_

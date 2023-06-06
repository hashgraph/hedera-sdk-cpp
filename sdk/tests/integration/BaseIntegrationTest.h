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
#ifndef HEDERA_SDK_CPP_BASE_INTEGRATION_TEST_H_
#define HEDERA_SDK_CPP_BASE_INTEGRATION_TEST_H_

#include "Client.h"

#include <gtest/gtest.h>

namespace Hedera
{

class BaseIntegrationTest : public testing::Test
{
protected:
  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }

  void SetUp() override;

private:
  Client mClient;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_BASE_INTEGRATION_TEST_H_
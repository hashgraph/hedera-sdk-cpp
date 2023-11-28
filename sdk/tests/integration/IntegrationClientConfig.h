/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_INTEGRATION_CLIENT_CONFIG_H_
#define HEDERA_SDK_CPP_INTEGRATION_CLIENT_CONFIG_H_

#include "Client.h"

#include <gtest/gtest.h>

namespace Hedera
{
    /**
     * @class IntegrationClientConfig
     * @brief Configuration class for integration testing that provides access to a Client instance.
     *
     * This class is responsible for managing the configuration and initialization
     * of a Client instance for integration testing purposes.
     */
    class IntegrationClientConfig
    {
    public:
        /**
         * @brief Get a reference to the configured Client instance.
         * @return A reference to the configured Client instance.
         */
        static Client& getInstance();

    private:
        /**
         * @brief Private constructor to prevent instantiation.
         */
        IntegrationClientConfig();

        // Disable copy and move operations
        IntegrationClientConfig(const IntegrationClientConfig&) = delete;
        IntegrationClientConfig& operator=(const IntegrationClientConfig&) = delete;

        static Client instance; ///< The configured Client instance.
        static bool initialized; ///< Flag indicating whether the Client instance has been initialized.
    };
}

#endif // HEDERA_SDK_CPP_INTEGRATION_CLIENT_CONFIG_H_

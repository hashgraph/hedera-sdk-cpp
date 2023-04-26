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
#ifndef HEDERA_SDK_CPP_DEFAULTS_H_
#define HEDERA_SDK_CPP_DEFAULTS_H_

#include "Hbar.h"

#include <chrono>

namespace Hedera
{
/**
 * The default maximum number of times a request will attempt to be submitted before considering the execution failed.
 */
constexpr auto DEFAULT_MAX_ATTEMPTS = 10U;
/**
 * The default minimum duration of time to wait before retrying to submit a previously-failed request to the same node.
 */
constexpr auto DEFAULT_MIN_BACKOFF = std::chrono::milliseconds(250);
/**
 * The default maximum duration of time to wait before retrying to submit a previously-failed request to the same node.
 */
constexpr auto DEFAULT_MAX_BACKOFF = std::chrono::seconds(8);
/**
 * The default maximum transaction fee.
 */
constexpr auto DEFAULT_MAX_TRANSACTION_FEE = Hbar(2LL);
/**
 * The default auto-renew period.
 */
constexpr auto DEFAULT_AUTO_RENEW_PERIOD = std::chrono::hours(2160);
}

#endif // HEDERA_SDK_CPP_DEFAULTS_H_
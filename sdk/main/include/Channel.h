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
#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <string>

namespace Hedera
{
class Channel
{
public:
  Channel();

private:
  /**
   * Implementation object used to hide implementation details and gRPC headers.
   */
  class ChannelImpl;
  ChannelImpl* mImpl;
};

} // namespace Hedera

#endif // CHANNEL_H_

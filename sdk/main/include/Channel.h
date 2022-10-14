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

#include <proto/crypto_service.grpc.pb.h>

#include <memory>
#include <string>

namespace Hedera
{
class Channel
{
public:
  /**
   * Construct this channel with a node URL with which to communicate.
   *
   * @param url The URL and port of the gRPC service with which this channel
   *            should communicate.
   */
  explicit Channel(const std::string& url);

  /**
   * Shutdown the channel.
   */
  void shutdown();

private:
  /**
   * Helper function to close/shutdown/delete channels.
   */
  void shutdownChannel();

  /**
   * Implementation object used to hide implementation details and gRPC headers.
   */
  std::unique_ptr<proto::CryptoService::Stub> mCryptoStub;
};

} // namespace Hedera

#endif // CHANNEL_H_

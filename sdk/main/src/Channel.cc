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
#include "Channel.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

namespace Hedera
{
//-----
void Channel::initChannel(const std::string& url)
{
  shutdownChannel();

  mCryptoStub = new proto::CryptoService::Stub(
    grpc::CreateChannel(url, grpc::InsecureChannelCredentials()));
}

//-----
Channel::~Channel()
{
  shutdownChannel();
}

//-----
void Channel::shutdown()
{
  shutdownChannel();
}

//-----
void Channel::shutdownChannel()
{
  if (mCryptoStub)
  {
    delete mCryptoStub;
    mCryptoStub = nullptr;
  }
}

} // namespace Hedera

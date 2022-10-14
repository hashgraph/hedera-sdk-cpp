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

#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <proto/crypto_service.grpc.pb.h>

namespace Hedera
{
//-----
class Channel::ChannelImpl
{
public:
  ChannelImpl() = default;

  explicit ChannelImpl(const std::string& url)
    : mUrl(url)
    , mCryptoStub(new proto::CryptoService::Stub(
        grpc::CreateChannel(url, grpc::InsecureChannelCredentials())))
  {
  }

  ~ChannelImpl() { delete mCryptoStub; }

  std::string mUrl;
  proto::CryptoService::Stub* mCryptoStub;
};

//-----
Channel::Channel()
  : mImpl(new ChannelImpl)
{
}

//-----
Channel::~Channel()
{
  delete mImpl;
}

//-----
Channel::Channel(const std::string& url)
  : mImpl(new ChannelImpl(url))
{
}

//-----
Channel::Channel(const Hedera::Channel& other)
  : Channel(other.mImpl->mUrl)
{
}

//-----
Channel::Channel(const Hedera::Channel&& other)
  : mImpl(other.mImpl)
{
}

//-----
Channel& Channel::operator=(const Hedera::Channel& other)
{
  initChannel(other.mImpl->mUrl);
  return *this;
}

//-----
Channel& Channel::operator=(const Hedera::Channel&& other)
{
  mImpl = other.mImpl;
  return *this;
}

//-----
void Channel::initChannel(const std::string& url)
{
  shutdownChannel();

  mImpl->mUrl = url;
  mImpl->mCryptoStub = new proto::CryptoService::Stub(
    grpc::CreateChannel(url, grpc::InsecureChannelCredentials()));
}

//-----
void Channel::shutdown()
{
  shutdownChannel();
}

//-----
void Channel::shutdownChannel()
{
  if (mImpl->mCryptoStub)
  {
    delete mImpl->mCryptoStub;
  }
}

} // namespace Hedera

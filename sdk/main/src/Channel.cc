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

#include <proto/crypto_service.grpc.pb.h>

#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

namespace Hedera
{
//-----
struct Channel::ChannelImpl
{
  std::string mUrl;
  std::unique_ptr<proto::CryptoService::Stub> mCryptoStub;
};

//-----
Channel::Channel()
  : mImpl(std::make_unique<ChannelImpl>())
{
}

//-----
Channel::Channel(const std::string& url)
  : Channel()
{
  initChannel(url);
}

//-----
Channel::~Channel() = default;

//-----
Channel::Channel(const Channel& other)
  : Channel()
{
  initChannel(other.mImpl->mUrl);
}

//-----
Channel& Channel::operator=(const Hedera::Channel& other)
{
  initChannel(other.mImpl->mUrl);
  return *this;
}

//-----
void Channel::initChannel(const std::string& url)
{
  shutdownChannel();

  mImpl->mUrl = url;
  mImpl->mCryptoStub = proto::CryptoService::NewStub(grpc::CreateChannel(url, grpc::InsecureChannelCredentials()));
}

//-----
std::pair<proto::Response, grpc::Status> Channel::submitRequest(const proto::Query& request,
                                                                const std::chrono::duration<double>& timeout)
{
  grpc::ClientContext context;
  std::chrono::system_clock::time_point deadline =
    std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::milliseconds>(timeout);
  context.set_deadline(deadline);

  proto::Response response;
  switch (request.query_case())
  {
    case proto::Query::QueryCase::kCryptogetAccountBalance:
    {
      grpc::Status grpcStatus = mImpl->mCryptoStub->cryptoGetBalance(&context, request, &response);
      return { response, grpcStatus };
    }
    default:
    {
      return { response, grpc::Status::CANCELLED };
    }
  }
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
    mImpl->mCryptoStub.reset();
  }
}

} // namespace Hedera

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

#include <stdexcept>
#include <unistd.h>

namespace Hedera
{
//-----
struct Channel::ChannelImpl
{
  std::string mUrl;
  std::shared_ptr<grpc::Channel> mChannel;
  std::shared_ptr<proto::CryptoService::Stub> mCryptoStub;
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
void Channel::initChannel(const std::string& url)
{
  shutdownChannel();

  mImpl->mUrl = url;
  mImpl->mChannel = grpc::CreateChannel(url, grpc::InsecureChannelCredentials());
  while (mImpl->mChannel->GetState(true) != grpc_connectivity_state::GRPC_CHANNEL_READY)
  {
    usleep(1000000);
  }
  mImpl->mCryptoStub = std::move(proto::CryptoService::NewStub(mImpl->mChannel));
}

//-----
std::function<grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
Channel::getGrpcTransactionMethod(int transactionBodyDataCase) const
{
  std::cout << __FUNCTION__ << "(): transactionBodyDataCase=" << transactionBodyDataCase << std::endl;
  switch (transactionBodyDataCase)
  {
    case proto::TransactionBody::DataCase::kCryptoAddLiveHash:
      return std::bind_front(&proto::CryptoService::Stub::addLiveHash, mImpl->mCryptoStub);
    case proto::TransactionBody::DataCase::kCryptoApproveAllowance:
      return std::bind_front(&proto::CryptoService::Stub::approveAllowances, mImpl->mCryptoStub);
    case proto::TransactionBody::DataCase::kCryptoDeleteAllowance:
      return std::bind_front(&proto::CryptoService::Stub::deleteAllowances, mImpl->mCryptoStub);
    case proto::TransactionBody::DataCase::kCryptoCreateAccount:
      return std::bind_front(&proto::CryptoService::Stub::createAccount, mImpl->mCryptoStub);
    case proto::TransactionBody::DataCase::kCryptoDelete:
      return std::bind_front(&proto::CryptoService::Stub::cryptoDelete, mImpl->mCryptoStub);
    case proto::TransactionBody::DataCase::kCryptoDeleteLiveHash:
      return std::bind_front(&proto::CryptoService::Stub::deleteLiveHash, mImpl->mCryptoStub);
    case proto::TransactionBody::DataCase::kCryptoTransfer:
      return std::bind_front(&proto::CryptoService::Stub::cryptoTransfer, mImpl->mCryptoStub);
    case proto::TransactionBody::DataCase::kCryptoUpdateAccount:
      return std::bind_front(&proto::CryptoService::Stub::updateAccount, mImpl->mCryptoStub);
    default:
      // This should never happen
      throw std::invalid_argument("Unrecognized gRPC transaction method case");
  }
}

//-----
std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> Channel::getGrpcQueryMethod(
  int queryBodyDataCase) const
{
  switch (queryBodyDataCase)
  {
    case proto::Query::QueryCase::kCryptogetAccountBalance:
      return std::bind_front(&proto::CryptoService::Stub::cryptoGetBalance, mImpl->mCryptoStub);
    case proto::Query::QueryCase::kCryptoGetAccountRecords:
      return std::bind_front(&proto::CryptoService::Stub::getAccountRecords, mImpl->mCryptoStub);
    case proto::Query::QueryCase::kCryptoGetInfo:
      return std::bind_front(&proto::CryptoService::Stub::getAccountInfo, mImpl->mCryptoStub);
    case proto::Query::QueryCase::kCryptoGetLiveHash:
      return std::bind_front(&proto::CryptoService::Stub::getLiveHash, mImpl->mCryptoStub);
    case proto::Query::QueryCase::kCryptoGetProxyStakers:
      return std::bind_front(&proto::CryptoService::Stub::getStakersByAccountID, mImpl->mCryptoStub);
    case proto::Query::QueryCase::kTransactionGetReceipt:
      return std::bind_front(&proto::CryptoService::Stub::getTransactionReceipts, mImpl->mCryptoStub);
    case proto::Query::QueryCase::kTransactionGetRecord:
      return std::bind_front(&proto::CryptoService::Stub::getTxRecordByTxID, mImpl->mCryptoStub);
    default:
      // This should never happen
      throw std::invalid_argument("Unrecognized gRPC query method case");
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

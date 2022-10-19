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
void Channel::initChannel(const std::string& url)
{
  shutdownChannel();

  mImpl->mUrl = url;
  mImpl->mCryptoStub = proto::CryptoService::NewStub(grpc::CreateChannel(url, grpc::InsecureChannelCredentials()));
}

//-----
std::pair<proto::Response, grpc::Status> Channel::submitRequest(const proto::Query& query,
                                                                const std::chrono::duration<double>& timeout)
{
  grpc::ClientContext context;
  context.set_deadline(std::chrono::system_clock::now() +
                       std::chrono::duration_cast<std::chrono::milliseconds>(timeout));

  proto::Response response;
  switch (query.query_case())
  {
    case proto::Query::QueryCase::kCryptogetAccountBalance:
      return { response, mImpl->mCryptoStub->cryptoGetBalance(&context, query, &response) };
    case proto::Query::QueryCase::kCryptoGetAccountRecords:
      return { response, mImpl->mCryptoStub->getAccountRecords(&context, query, &response) };
    case proto::Query::QueryCase::kCryptoGetInfo:
      return { response, mImpl->mCryptoStub->getAccountInfo(&context, query, &response) };
    case proto::Query::QueryCase::kCryptoGetLiveHash:
      return { response, mImpl->mCryptoStub->getLiveHash(&context, query, &response) };
    case proto::Query::QueryCase::kCryptoGetProxyStakers:
      return { response, mImpl->mCryptoStub->getStakersByAccountID(&context, query, &response) };
    case proto::Query::QueryCase::kTransactionGetReceipt:
      return { response, mImpl->mCryptoStub->getTransactionReceipts(&context, query, &response) };
    case proto::Query::QueryCase::kTransactionGetRecord:
      return { response, mImpl->mCryptoStub->getTxRecordByTxID(&context, query, &response) };
    default:
      return { response, grpc::Status::CANCELLED };
  }
}

//-----
std::pair<proto::TransactionResponse, grpc::Status> Channel::submitRequest(const proto::Transaction& transaction,
                                                                           const std::chrono::duration<double>& timeout)
{
  grpc::ClientContext context;
  context.set_deadline(std::chrono::system_clock::now() +
                       std::chrono::duration_cast<std::chrono::milliseconds>(timeout));

  proto::TransactionResponse response;
  grpc::Status status;
  switch (transaction.body().data_case())
  {
    case proto::TransactionBody::DataCase::kCryptoAddLiveHash:
      status = mImpl->mCryptoStub->addLiveHash(&context, transaction, &response);
      break;
    case proto::TransactionBody::DataCase::kCryptoApproveAllowance:
      status = mImpl->mCryptoStub->approveAllowances(&context, transaction, &response);
      break;
    case proto::TransactionBody::DataCase::kCryptoDeleteAllowance:
      status = mImpl->mCryptoStub->deleteAllowances(&context, transaction, &response);
      break;
    case proto::TransactionBody::DataCase::kCryptoCreateAccount:
      status = mImpl->mCryptoStub->createAccount(&context, transaction, &response);
      break;
    case proto::TransactionBody::DataCase::kCryptoDelete:
      status = mImpl->mCryptoStub->cryptoDelete(&context, transaction, &response);
      break;
    case proto::TransactionBody::DataCase::kCryptoDeleteLiveHash:
      status = mImpl->mCryptoStub->deleteLiveHash(&context, transaction, &response);
      break;
    case proto::TransactionBody::DataCase::kCryptoTransfer:
      status = mImpl->mCryptoStub->cryptoTransfer(&context, transaction, &response);
      break;
    case proto::TransactionBody::DataCase::kCryptoUpdateAccount:
      status = mImpl->mCryptoStub->updateAccount(&context, transaction, &response);
      break;
    default:
      status = grpc::Status::CANCELLED;
      break;
  }

  return { response, status };
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

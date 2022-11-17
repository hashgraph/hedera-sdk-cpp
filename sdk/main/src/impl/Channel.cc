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
#include "impl/Channel.h"
#include "impl/HederaCertificateVerifier.h"

#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <proto/crypto_service.grpc.pb.h>
#include <stdexcept>

namespace Hedera::internal
{
//-----
struct Channel::ChannelImpl
{
  std::shared_ptr<grpc::Channel> mChannel;
  std::shared_ptr<proto::CryptoService::Stub> mCryptoStub;
};

//-----
Channel::Channel()
  : mImpl(std::make_unique<ChannelImpl>())
{
}

//-----
Channel::~Channel() = default;

//-----
bool Channel::initializeEncryptedChannel(const std::string& url, const std::string& nodeCertHash)
{
  std::shared_ptr<grpc::experimental::CertificateVerifier> verifier =
    grpc::experimental::ExternalCertificateVerifier::Create<HederaCertificateVerifier>(nodeCertHash);

  auto credentialsOptions = grpc::experimental::TlsChannelCredentialsOptions();

  // don't do normal cert verification. we are doing custom verification using the node's cert chain hash
  credentialsOptions.set_verify_server_certs(false);
  credentialsOptions.set_check_call_host(false);
  credentialsOptions.set_certificate_verifier(verifier);

  return initializeChannel(url, grpc::experimental::TlsCredentials(credentialsOptions));
}

bool Channel::initializeUnencryptedChannel(const std::string& url)
{
  return initializeChannel(url, grpc::InsecureChannelCredentials());
}

//-----
std::function<grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
Channel::getGrpcTransactionMethod(int transactionBodyDataCase) const
{
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
  if (mImpl->mCryptoStub)
  {
    mImpl->mCryptoStub.reset();
  }

  mInitialized = false;
}

bool Channel::getInitialized() const
{
  return mInitialized;
}

bool Channel::initializeChannel(const std::string& url, const std::shared_ptr<grpc::ChannelCredentials>& credentials)
{
  shutdown();

  mImpl->mChannel = grpc::CreateChannel(url, credentials);

  if (mImpl->mChannel->WaitForConnected(std::chrono::system_clock::now() + std::chrono::seconds(1)))
  {
    mImpl->mCryptoStub = proto::CryptoService::NewStub(mImpl->mChannel);
    mInitialized = true;

    return true;
  }

  return false;
}

} // namespace Hedera::internal

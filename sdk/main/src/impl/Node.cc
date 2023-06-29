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
#include "impl/Node.h"
#include "exceptions/UninitializedException.h"
#include "impl/HederaCertificateVerifier.h"

#include <algorithm>
#include <grpcpp/create_channel.h>
#include <utility>

namespace Hedera::internal
{
//-----
Node::Node(std::shared_ptr<NodeAddress> address, TLSBehavior tls)
  : mAddress(std::move(address))
{
  // In order to use TLS, a node certificate chain must be provided
  if (tls == TLSBehavior::REQUIRE && mAddress->getNodeCertHash().empty())
  {
    throw UninitializedException("NodeAddress has empty certificate chain hash for TLS connection");
  }

  grpc::experimental::TlsChannelCredentialsOptions tlsChannelCredentialsOptions;

  // Custom verification using the node's certificate chain hash
  tlsChannelCredentialsOptions.set_verify_server_certs(false);
  tlsChannelCredentialsOptions.set_check_call_host(false);
  tlsChannelCredentialsOptions.set_certificate_verifier(
    grpc::experimental::ExternalCertificateVerifier::Create<HederaCertificateVerifier>(mAddress->getNodeCertHash()));

  // Feed in the root CA's file manually for Windows (this is a bug in the gRPC implementation and says here
  // https://deploy-preview-763--grpc-io.netlify.app/docs/guides/auth/#using-client-side-ssltls that this needs to be
  // specified manually).
#ifdef _WIN32
  tlsChannelCredentialsOptions.set_certificate_provider(
    std::make_shared<grpc::experimental::FileWatcherCertificateProvider>("roots.pem", -1));
  tlsChannelCredentialsOptions.watch_root_certs();
#endif

  mTlsChannelCredentials = grpc::experimental::TlsCredentials(tlsChannelCredentialsOptions);
}

//-----
bool Node::connect(const std::chrono::system_clock::time_point& timeout)
{
  return mIsInitialized || initializeChannel(timeout);
}

//-----
void Node::shutdown()
{
  if (mCryptoStub)
  {
    mCryptoStub.reset();
  }

  if (mSmartContractStub)
  {
    mSmartContractStub.reset();
  }

  if (mFileStub)
  {
    mFileStub.reset();
  }

  if (mTokenStub)
  {
    mTokenStub.reset();
  }

  if (mChannel)
  {
    mChannel.reset();
  }

  mIsInitialized = false;
}

//-----
grpc::Status Node::submitQuery(proto::Query::QueryCase funcEnum,
                               const proto::Query& query,
                               const std::chrono::system_clock::time_point& deadline,
                               proto::Response* response)
{
  if (!connect(deadline))
  {
    return grpc::Status::CANCELLED;
  }

  grpc::ClientContext context;
  context.set_deadline(deadline);

  switch (funcEnum)
  {
    case proto::Query::QueryCase::kContractCallLocal:
      return mSmartContractStub->contractCallLocalMethod(&context, query, response);
    case proto::Query::QueryCase::kContractGetBytecode:
      return mSmartContractStub->ContractGetBytecode(&context, query, response);
    case proto::Query::QueryCase::kContractGetInfo:
      return mSmartContractStub->getContractInfo(&context, query, response);
    case proto::Query::QueryCase::kCryptogetAccountBalance:
      return mCryptoStub->cryptoGetBalance(&context, query, response);
    case proto::Query::QueryCase::kCryptoGetAccountRecords:
      return mCryptoStub->getAccountRecords(&context, query, response);
    case proto::Query::QueryCase::kCryptoGetInfo:
      return mCryptoStub->getAccountInfo(&context, query, response);
    case proto::Query::QueryCase::kCryptoGetLiveHash:
      return mCryptoStub->getLiveHash(&context, query, response);
    case proto::Query::QueryCase::kCryptoGetProxyStakers:
      return mCryptoStub->getStakersByAccountID(&context, query, response);
    case proto::Query::QueryCase::kFileGetContents:
      return mFileStub->getFileContent(&context, query, response);
    case proto::Query::QueryCase::kFileGetInfo:
      return mFileStub->getFileInfo(&context, query, response);
    case proto::Query::QueryCase::kTokenGetInfo:
      return mTokenStub->getTokenInfo(&context, query, response);
    case proto::Query::QueryCase::kTransactionGetReceipt:
      return mCryptoStub->getTransactionReceipts(&context, query, response);
    case proto::Query::QueryCase::kTransactionGetRecord:
      return mCryptoStub->getTxRecordByTxID(&context, query, response);
    default:
      // This should never happen
      throw std::invalid_argument("Unrecognized gRPC query method case");
  }
}

//-----
grpc::Status Node::submitTransaction(proto::TransactionBody::DataCase funcEnum,
                                     const proto::Transaction& transaction,
                                     const std::chrono::system_clock::time_point& deadline,
                                     proto::TransactionResponse* response)
{
  if (!connect(deadline))
  {
    return grpc::Status::CANCELLED;
  }

  grpc::ClientContext context;
  context.set_deadline(deadline);

  switch (funcEnum)
  {
    case proto::TransactionBody::DataCase::kContractCall:
      return mSmartContractStub->contractCallMethod(&context, transaction, response);
    case proto::TransactionBody::DataCase::kContractCreateInstance:
      return mSmartContractStub->createContract(&context, transaction, response);
    case proto::TransactionBody::DataCase::kContractDeleteInstance:
      return mSmartContractStub->deleteContract(&context, transaction, response);
    case proto::TransactionBody::DataCase::kContractUpdateInstance:
      return mSmartContractStub->updateContract(&context, transaction, response);
    case proto::TransactionBody::DataCase::kCryptoAddLiveHash:
      return mCryptoStub->addLiveHash(&context, transaction, response);
    case proto::TransactionBody::DataCase::kCryptoApproveAllowance:
      return mCryptoStub->approveAllowances(&context, transaction, response);
    case proto::TransactionBody::DataCase::kCryptoDeleteAllowance:
      return mCryptoStub->deleteAllowances(&context, transaction, response);
    case proto::TransactionBody::DataCase::kCryptoCreateAccount:
      return mCryptoStub->createAccount(&context, transaction, response);
    case proto::TransactionBody::DataCase::kCryptoDelete:
      return mCryptoStub->cryptoDelete(&context, transaction, response);
    case proto::TransactionBody::DataCase::kCryptoDeleteLiveHash:
      return mCryptoStub->deleteLiveHash(&context, transaction, response);
    case proto::TransactionBody::DataCase::kCryptoTransfer:
      return mCryptoStub->cryptoTransfer(&context, transaction, response);
    case proto::TransactionBody::DataCase::kCryptoUpdateAccount:
      return mCryptoStub->updateAccount(&context, transaction, response);
    case proto::TransactionBody::DataCase::kEthereumTransaction:
      return mSmartContractStub->callEthereum(&context, transaction, response);
    case proto::TransactionBody::DataCase::kFileAppend:
      return mFileStub->appendContent(&context, transaction, response);
    case proto::TransactionBody::DataCase::kFileCreate:
      return mFileStub->createFile(&context, transaction, response);
    case proto::TransactionBody::DataCase::kFileDelete:
      return mFileStub->deleteFile(&context, transaction, response);
    case proto::TransactionBody::DataCase::kFileUpdate:
      return mFileStub->updateFile(&context, transaction, response);
    case proto::TransactionBody::DataCase::kTokenAssociate:
      return mTokenStub->associateTokens(&context, transaction, response);
    case proto::TransactionBody::DataCase::kTokenCreation:
      return mTokenStub->createToken(&context, transaction, response);
    case proto::TransactionBody::DataCase::kTokenDeletion:
      return mTokenStub->deleteToken(&context, transaction, response);
    case proto::TransactionBody::DataCase::kTokenFeeScheduleUpdate:
      return mTokenStub->updateTokenFeeSchedule(&context, transaction, response);
    case proto::TransactionBody::DataCase::kTokenMint:
      return mTokenStub->mintToken(&context, transaction, response);
    case proto::TransactionBody::DataCase::kTokenUpdate:
      return mTokenStub->updateToken(&context, transaction, response);
    default:
      // This should never happen
      throw std::invalid_argument("Unrecognized gRPC transaction method case");
  }
}

//-----
void Node::setTLSBehavior(TLSBehavior desiredBehavior)
{
  if (desiredBehavior == mTLSBehavior)
  {
    return;
  }

  // In order to use TLS, a node certificate chain must be provided
  if (desiredBehavior == TLSBehavior::REQUIRE && mAddress->getNodeCertHash().empty())
  {
    throw UninitializedException("NodeAddress has empty certificate chain hash for TLS connection");
  }

  mTLSBehavior = desiredBehavior;
  shutdown();
}

//-----
void Node::setMinBackoff(const std::chrono::duration<double>& backoff)
{
  mMinBackoff = backoff;
}

//-----
void Node::setMaxBackoff(const std::chrono::duration<double>& backoff)
{
  mMaxBackoff = backoff;
}

//-----
bool Node::isHealthy() const
{
  return mReadmitTime < std::chrono::system_clock::now();
}

//-----
void Node::increaseBackoff()
{
  mReadmitTime =
    std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::system_clock::duration>(mCurrentBackoff);
  mCurrentBackoff = std::min(mCurrentBackoff * 2, mMaxBackoff);
}

//-----
void Node::decreaseBackoff()
{
  mCurrentBackoff /= 2.0;
  mCurrentBackoff = std::max(mCurrentBackoff / 2.0, mMinBackoff);
}

//-----
std::chrono::duration<double> Node::getRemainingTimeForBackoff() const
{
  return mReadmitTime - std::chrono::system_clock::now();
}

//-----
bool Node::initializeChannel(const std::chrono::system_clock::time_point& deadline)
{
  const std::vector<std::shared_ptr<Endpoint>> endpoints = mAddress->getEndpoints();

  std::shared_ptr<grpc::ChannelCredentials> channelCredentials = nullptr;
  for (const auto& endpoint : endpoints)
  {
    switch (mTLSBehavior)
    {
      case TLSBehavior::REQUIRE:
      {
        if (NodeAddress::isTlsPort(endpoint->getPort()))
        {
          channelCredentials = mTlsChannelCredentials;
        }

        break;
      }

      case TLSBehavior::DISABLE:
      {
        if (NodeAddress::isNonTlsPort(endpoint->getPort()))
        {
          channelCredentials = grpc::InsecureChannelCredentials();
        }

        break;
      }

      default:
      {
        continue;
      }
    }

    if (channelCredentials)
    {
      shutdown();

      mChannel = grpc::CreateChannel(endpoint->toString(), channelCredentials);

      if (mChannel->WaitForConnected(deadline))
      {
        mCryptoStub = proto::CryptoService::NewStub(mChannel);
        mFileStub = proto::FileService::NewStub(mChannel);
        mSmartContractStub = proto::SmartContractService::NewStub(mChannel);
        mTokenStub = proto::TokenService::NewStub(mChannel);
        mIsInitialized = true;

        return true;
      }
      else
      {
        mChannel = nullptr;
      }
    }
    else
    {
      channelCredentials = nullptr;
    }
  }

  return false;
}

} // namespace Hedera::internal

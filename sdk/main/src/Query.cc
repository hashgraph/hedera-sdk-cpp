/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "Query.h"
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountRecords.h"
#include "AccountRecordsQuery.h"
#include "Client.h"
#include "ContractCallQuery.h"
#include "ContractFunctionResult.h"
#include "ContractInfo.h"
#include "ContractInfoQuery.h"
#include "FileContentsQuery.h"
#include "FileInfo.h"
#include "FileInfoQuery.h"
#include "Hbar.h"
#include "ScheduleInfo.h"
#include "ScheduleInfoQuery.h"
#include "Status.h"
#include "TokenInfo.h"
#include "TokenInfoQuery.h"
#include "TokenNftInfo.h"
#include "TokenNftInfoQuery.h"
#include "TopicInfo.h"
#include "TopicInfoQuery.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionReceiptQuery.h"
#include "TransactionRecord.h"
#include "TransactionRecordQuery.h"
#include "TransferTransaction.h"
#include "exceptions/MaxQueryPaymentExceededException.h"
#include "impl/Network.h"

#include <optional>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/transaction.pb.h>
#include <unordered_map>

namespace Hedera
{
//-----
template<typename SdkRequestType, typename SdkResponseType>
struct Query<SdkRequestType, SdkResponseType>::QueryImpl
{
  // The explicit amount to pay for this Query.
  std::optional<Hbar> mPayment;

  // The maximum amount to pay for this Query.
  std::optional<Hbar> mMaxPayment;

  // Is this Query meant to get the cost?
  bool mGetCost = false;

  // The cost to execute this Query.
  Hbar mCost;

  // The Client that should be used to pay for the payment transaction of this Query.
  Client* mClient;

  // The TransactionID of the payment transactions for this Query.
  TransactionId mPaymentTransactionId;

  // The map of node account IDs to their respective payment Transaction protobuf objects that could be sent.
  std::unordered_map<AccountId, proto::Transaction> mPaymentTransactions;
};

//-----
template<typename SdkRequestType, typename SdkResponseType>
SdkRequestType& Query<SdkRequestType, SdkResponseType>::setQueryPayment(const Hbar& amount)
{
  mImpl->mPayment = amount;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
SdkRequestType& Query<SdkRequestType, SdkResponseType>::setMaxQueryPayment(const Hbar& maxAmount)
{
  mImpl->mMaxPayment = maxAmount;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
Hbar Query<SdkRequestType, SdkResponseType>::getCost(const Client& client)
{
  return getCost(client, client.getRequestTimeout());
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
Hbar Query<SdkRequestType, SdkResponseType>::getCost(const Client& client, const std::chrono::duration<double>& timeout)
{
  // Initialize this Query with node account IDs.
  setNodeAccountIds(client);

  // Construct a separate derived Query and set it to get the cost.
  SdkRequestType costQuery;
  costQuery.mImpl->mGetCost = true;
  costQuery.setNodeAccountIds(client);

  // Execute the cost query and return the cost.
  costQuery.execute(client, timeout);
  return costQuery.mImpl->mCost;
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
void Query<SdkRequestType, SdkResponseType>::saveCostFromHeader(const proto::ResponseHeader& header)
{
  if (mImpl->mGetCost)
  {
    mImpl->mCost = Hbar(static_cast<int64_t>(header.cost()), HbarUnit::TINYBAR());
  }
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
Query<SdkRequestType, SdkResponseType>::~Query() = default;

//-----
template<typename SdkRequestType, typename SdkResponseType>
Query<SdkRequestType, SdkResponseType>::Query(const Query<SdkRequestType, SdkResponseType>& other)
  : mImpl(std::make_unique<QueryImpl>(*other.mImpl))
{
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
Query<SdkRequestType, SdkResponseType>& Query<SdkRequestType, SdkResponseType>::operator=(
  const Query<SdkRequestType, SdkResponseType>& other)
{
  if (this != &other)
  {
    mImpl = std::make_unique<QueryImpl>(*other.mImpl);
  }

  return *this;
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
Query<SdkRequestType, SdkResponseType>::Query(Query<SdkRequestType, SdkResponseType>&& other) noexcept
  : mImpl(std::move(other.mImpl))
{
  // Leave moved-from object in a valid state.
  other.mImpl = std::make_unique<QueryImpl>();
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
Query<SdkRequestType, SdkResponseType>& Query<SdkRequestType, SdkResponseType>::operator=(
  Query<SdkRequestType, SdkResponseType>&& other) noexcept
{
  if (this != &other)
  {
    mImpl = std::move(other.mImpl);

    // Leave moved-from object in a valid state.
    other.mImpl = std::make_unique<QueryImpl>();
  }

  return *this;
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
proto::Query Query<SdkRequestType, SdkResponseType>::makeRequest(unsigned int index) const
{
  auto header = std::make_unique<proto::QueryHeader>();

  if (isPaymentRequired() && !mImpl->mGetCost)
  {
    // Get the node account ID for this index.
    const AccountId accountId =
      Executable<SdkRequestType, proto::Query, proto::Response, SdkResponseType>::getNodeAccountIds().at(index);

    // See if the payment transaction has been created for this index already.
    if (mImpl->mPaymentTransactions.find(accountId) == mImpl->mPaymentTransactions.end())
    {
      if (mImpl->mPaymentTransactionId == TransactionId())
      {
        mImpl->mPaymentTransactionId = TransactionId::generate(mImpl->mClient->getOperatorAccountId().value());
      }

      mImpl->mPaymentTransactions[accountId] =
        makePaymentTransaction(mImpl->mPaymentTransactionId, accountId, *mImpl->mClient, mImpl->mCost);
    }

    header->set_allocated_payment(
      std::make_unique<proto::Transaction>(mImpl->mPaymentTransactions.at(accountId)).release());
  }

  header->set_responsetype(mImpl->mGetCost ? proto::ResponseType::COST_ANSWER : proto::ResponseType::ANSWER_ONLY);
  return buildRequest(header.release());
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
Status Query<SdkRequestType, SdkResponseType>::mapResponseStatus(const proto::Response& response) const
{
  return gProtobufResponseCodeToStatus.at(mapResponseHeader(response).nodetransactionprecheckcode());
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
void Query<SdkRequestType, SdkResponseType>::onExecute(const Client& client)
{
  if (!isPaymentRequired())
  {
    return;
  }

  // Get the cost and make sure it's willing to be paid.
  mImpl->mCost = getCost(client);
  if (mImpl->mCost.toTinybars() > ((mImpl->mMaxPayment.has_value()) ? mImpl->mMaxPayment->toTinybars()
                                                                    : ((client.getMaxQueryPayment().has_value())
                                                                         ? client.getMaxQueryPayment()->toTinybars()
                                                                         : DEFAULT_MAX_QUERY_PAYMENT.toTinybars())))
  {
    throw MaxQueryPaymentExceededException("Cost to execute Query (" + std::to_string(mImpl->mCost.toTinybars()) +
                                           HbarUnit::TINYBAR().getSymbol() + ") is larger than allowed amount.");
  }
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
proto::Transaction Query<SdkRequestType, SdkResponseType>::makePaymentTransaction(const TransactionId& transactionId,
                                                                                  const AccountId& nodeAccountId,
                                                                                  const Client& client,
                                                                                  const Hbar& amount) const
{
  return TransferTransaction()
    .setTransactionId(transactionId)
    .setNodeAccountIds({ nodeAccountId })
    .setMaxTransactionFee(Hbar(1LL))
    .addHbarTransfer(client.getOperatorAccountId().value(), amount.negated())
    .addHbarTransfer(nodeAccountId, amount)
    .freeze()
    .signWith(client.getOperatorPublicKey(), client.getOperatorSigner())
    // There's only one node account ID, therefore only one Transaction protobuf object will be created, and that will
    // be put in the 0th index.
    .makeRequest(0);
}

//-----
template<typename SdkRequestType, typename SdkResponseType>
void Query<SdkRequestType, SdkResponseType>::setNodeAccountIds(const Client& client)
{
  // Do nothing if node account IDs have already been set.
  if (!Executable<SdkRequestType, proto::Query, proto::Response, SdkResponseType>::getNodeAccountIds().empty())
  {
    Executable<SdkRequestType, proto::Query, proto::Response, SdkResponseType>::setNodeAccountIds(
      client.getNetwork()->getNodeAccountIdsForExecute());
  }
}

/**
 * Explicit template instantiations.
 */
template class Query<AccountBalanceQuery, AccountBalance>;
template class Query<AccountRecordsQuery, AccountRecords>;
template class Query<ContractCallQuery, ContractFunctionResult>;
template class Query<ContractInfoQuery, ContractInfo>;
template class Query<FileContentsQuery, FileContents>;
template class Query<FileInfoQuery, FileInfo>;
template class Query<ScheduleInfoQuery, ScheduleInfo>;
template class Query<TokenInfoQuery, TokenInfo>;
template class Query<TokenNftInfoQuery, TokenNftInfo>;
template class Query<TopicInfoQuery, TopicInfo>;
template class Query<TransactionReceiptQuery, TransactionReceipt>;
template class Query<TransactionRecordQuery, TransactionRecord>;

} // namespace Hedera

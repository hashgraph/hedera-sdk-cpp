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
#include "Transaction.h"

#include "AccountCreateTransaction.h"
#include "Client.h"
#include "TransactionId.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include "helper/DurationConverter.h"

#include <proto/basic_types.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <proto/transaction_contents.pb.h>

#include <vector>

namespace Hedera
{
//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setValidTransactionDuration(const std::chrono::duration<double>& duration)
{
  mTransactionValidDuration = duration;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setMaxTransactionFee(const Hedera::Hbar& fee)
{
  mMaxTransactionFee = fee;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setTransactionMemo(const std::string& memo)
{
  mTransactionMemo = memo;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setTransactionId(const TransactionId& id)
{
  mTransactionId = id;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
TransactionResponse Transaction<SdkRequestType>::mapResponse(const proto::TransactionResponse& response) const
{
  return TransactionResponse::fromProtobuf(response).setTransactionId(mTransactionId);
}

//-----
template<typename SdkRequestType>
void Transaction<SdkRequestType>::onExecute(const Client& client)
{
  mTransactionId = TransactionId::generate(client.getOperatorAccountId());
}

//-----
template<typename SdkRequestType>
void Transaction<SdkRequestType>::onSelectNode(const std::shared_ptr<Node>& node)
{
  mNodeAccountId = node->getAccountId();
}

//-----
template<typename SdkRequestType>
proto::Transaction Transaction<SdkRequestType>::signTransaction(const proto::TransactionBody& transaction,
                                                                const Client& client) const
{
  // Make sure the operator has been set, and therefore has an account ID and key.
  if (client.getOperator())
  {
    // Generate a signature from the TransactionBody
    auto transactionBodySerialized = new std::string(transaction.SerializeAsString());
    const std::vector<unsigned char> signature = client.getOperator()->mPrivateKey->sign(
      { transactionBodySerialized->cbegin(), transactionBodySerialized->cend() });

    // Generate a protobuf SignaturePair from a protobuf SignatureMap
    auto signatureMap = new proto::SignatureMap();
    signatureMap->add_sigpair()->set_allocated_ed25519(new std::string(signature.cbegin(), signature.cend()));

    // Create a protobuf SignedTransaction from the TransactionBody and SignatureMap
    proto::SignedTransaction signedTransaction;
    signedTransaction.set_allocated_bodybytes(transactionBodySerialized);
    signedTransaction.set_allocated_sigmap(signatureMap);

    // Serialize the protobuf SignedTransaction to a protobuf Transaction
    proto::Transaction transactionToReturn;
    transactionToReturn.set_allocated_signedtransactionbytes(new std::string(signedTransaction.SerializeAsString()));

    return transactionToReturn;
  }

  throw std::invalid_argument("Invalid client used to sign transaction");
}

//-----
template<typename SdkRequestType>
proto::TransactionBody Transaction<SdkRequestType>::generateTransactionBody(const Client& client) const
{
  proto::TransactionBody body;
  body.set_allocated_transactionid(mTransactionId.toProtobuf());
  body.set_transactionfee(static_cast<uint64_t>(getMaxTransactionFee(client).toTinybars()));
  body.set_allocated_memo(new std::string(mTransactionMemo));
  body.set_allocated_transactionvalidduration(DurationConverter::toProtobuf(mTransactionValidDuration));
  body.set_allocated_nodeaccountid(mNodeAccountId.toProtobuf());
  return body;
}

//-----
template<typename SdkRequestType>
Hbar Transaction<SdkRequestType>::getMaxTransactionFee(const Client& client) const
{
  if (mMaxTransactionFee != mDefaultMaxTransactionFee)
  {
    return mMaxTransactionFee;
  }

  else if (const std::shared_ptr<Hbar> defaultMaxTxFee = client.getDefaultMaxTransactionFee())
  {
    return *defaultMaxTxFee;
  }

  else
  {
    return mDefaultMaxTransactionFee;
  }
}

/**
 * Explicit template instantiation
 */
template class Transaction<AccountCreateTransaction>;
template class Transaction<TransferTransaction>;

} // namespace Hedera

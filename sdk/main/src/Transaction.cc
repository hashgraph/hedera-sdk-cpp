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
#include "PrivateKey.h"
#include "TransactionId.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

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
proto::Transaction Transaction<SdkRequestType>::signTransaction(const proto::TransactionBody& transaction,
                                                                const Client& client) const
{
  // Make sure the operator key and account ID are valid, and make sure the account ID for this transaction matches
  // the operator's account ID.
  if (client.getOperatorPublicKey() && client.getOperatorAccountId().has_value() &&
      mTransactionId.getAccountId().has_value() && mTransactionId.getAccountId() == client.getOperatorAccountId())
  {
    // Generate a signature from the TransactionBody
    auto transactionBodySerialized = std::make_shared<std::string>(transaction.SerializeAsString());
    const std::vector<unsigned char> signature = client.getOperatorPublicKey()->getPrivateKey()->sign(
      { transactionBodySerialized->cbegin(), transactionBodySerialized->cend() });

    // Generate a protobuf SignaturePair from a protobuf SignatureMap
    auto signatureMap = std::make_shared<proto::SignatureMap>();
    signatureMap->add_sigpair()->set_allocated_ed25519(new std::string{ signature.cbegin(), signature.cend() });

    // Create a protobuf SignedTransaction from the TransactionBody and SignatureMap
    proto::SignedTransaction signedTransaction;
    signedTransaction.set_allocated_bodybytes(transactionBodySerialized.get());
    signedTransaction.set_allocated_sigmap(signatureMap.get());

    // Serialize the protobuf SignedTransaction to a protobuf Transaction
    proto::Transaction transactionToReturn;
    transactionToReturn.set_allocated_signedtransactionbytes(new std::string(signedTransaction.SerializeAsString()));

    return transactionToReturn;
  }

  // TODO: throw?
  return proto::Transaction();
}

/**
 * Explicit template instantiation
 */
template class Transaction<AccountCreateTransaction>;
template class Transaction<TransferTransaction>;

} // namespace Hedera

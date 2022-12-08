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
#include "Status.h"
#include "TransactionId.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "impl/DurationConverter.h"
#include "impl/Node.h"

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
SdkRequestType& Transaction<SdkRequestType>::setMaxTransactionFee(const Hbar& fee)
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
SdkRequestType& Transaction<SdkRequestType>::setRegenerateTransactionIdPolicy(bool regenerate)
{
  mTransactionIdRegenerationPolicy = regenerate;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
void Transaction<SdkRequestType>::onSelectNode(const std::shared_ptr<internal::Node>& node)
{
  Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::onSelectNode(node);
  mNodeAccountId = node->getAccountId();
}

//-----
template<typename SdkRequestType>
proto::Transaction Transaction<SdkRequestType>::signTransaction(const proto::TransactionBody& transaction,
                                                                const Client& client) const
{
  // Make sure the operator has been set, and therefore has an account ID and key.
  if (client.getOperatorAccountId() && client.getOperatorPublicKey())
  {
    // Generate a signature from the TransactionBody
    auto transactionBodySerialized = std::make_unique<std::string>(transaction.SerializeAsString());
    const std::vector<unsigned char> signature =
      client.sign({ transactionBodySerialized->cbegin(), transactionBodySerialized->cend() });

    // Generate a protobuf SignaturePair from a protobuf SignatureMap
    auto signatureMap = std::make_unique<proto::SignatureMap>();
    signatureMap->add_sigpair()->set_allocated_ed25519(new std::string(signature.cbegin(), signature.cend()));

    // Create a protobuf SignedTransaction from the TransactionBody and SignatureMap
    proto::SignedTransaction signedTransaction;
    signedTransaction.set_allocated_bodybytes(transactionBodySerialized.release());
    signedTransaction.set_allocated_sigmap(signatureMap.release());

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
  body.set_allocated_transactionid(mTransactionId.toProtobuf().release());
  body.set_transactionfee(static_cast<uint64_t>(getMaxTransactionFee(client).toTinybars()));
  body.set_allocated_memo(new std::string(mTransactionMemo));
  body.set_allocated_transactionvalidduration(internal::DurationConverter::toProtobuf(mTransactionValidDuration));
  body.set_allocated_nodeaccountid(mNodeAccountId.toProtobuf().release());
  return body;
}

//-----
template<typename SdkRequestType>
Hbar Transaction<SdkRequestType>::getMaxTransactionFee(const Client& client) const
{
  return mMaxTransactionFee              ? *mMaxTransactionFee
         : client.getMaxTransactionFee() ? *client.getMaxTransactionFee()
                                         : DEFAULT_MAX_TRANSACTION_FEE;
}

//-----
template<typename SdkRequestType>
TransactionResponse Transaction<SdkRequestType>::mapResponse(const proto::TransactionResponse& response) const
{
  TransactionResponse txResp = TransactionResponse::fromProtobuf(response);
  txResp.mTransactionId = mTransactionId;
  return txResp;
}

//-----
template<typename SdkRequestType>
Status Transaction<SdkRequestType>::mapResponseStatus(const proto::TransactionResponse& response) const
{
  return STATUS_MAP.at(response.nodetransactionprecheckcode());
}

//-----
template<typename SdkRequestType>
typename Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
  ExecutionStatus
  Transaction<SdkRequestType>::shouldRetry(Status status,
                                           const Client& client,
                                           const proto::TransactionResponse& response)
{
  if (status == Status::TRANSACTION_EXPIRED)
  {
    if (mTransactionIdRegenerationPolicy)
    {
      if (*mTransactionIdRegenerationPolicy)
      {
        mTransactionId = TransactionId::generate(mTransactionId.getAccountId());
        return Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
          ExecutionStatus::RETRY;
      }
    }

    else if (const std::optional<bool> clientTxIdRegenPolicy = client.getTransactionIdRegenerationPolicy();
             clientTxIdRegenPolicy)
    {
      if (*clientTxIdRegenPolicy)
      {
        mTransactionId = TransactionId::generate(mTransactionId.getAccountId());
        return Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
          ExecutionStatus::RETRY;
      }
    }

    // Regenerate transaction IDs by default
    else
    {
      mTransactionId = TransactionId::generate(mTransactionId.getAccountId());
      return Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
        ExecutionStatus::RETRY;
    }
  }

  // If the Transaction's not expired, forward to the base class.
  else
  {
    return Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::shouldRetry(
      status, client, response);
  }

  // Return REQUEST_ERROR if the transaction expired but transaction IDs aren't allowed to be regenerated.
  return Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
    ExecutionStatus::REQUEST_ERROR;
}

//-----
template<typename SdkRequestType>
void Transaction<SdkRequestType>::onExecute(const Client& client)
{
  mTransactionId = TransactionId::generate(*client.getOperatorAccountId());
}

/**
 * Explicit template instantiation.
 */
template class Transaction<AccountCreateTransaction>;
template class Transaction<TransferTransaction>;

} // namespace Hedera

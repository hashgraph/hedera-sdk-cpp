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
#include "AccountAllowanceApproveTransaction.h"
#include "AccountAllowanceDeleteTransaction.h"
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountUpdateTransaction.h"
#include "Client.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractExecuteTransaction.h"
#include "ContractUpdateTransaction.h"
#include "ED25519PublicKey.h"
#include "EthereumTransaction.h"
#include "FileAppendTransaction.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileUpdateTransaction.h"
#include "PrivateKey.h"
#include "Status.h"
#include "TokenAssociateTransaction.h"
#include "TokenBurnTransaction.h"
#include "TokenCreateTransaction.h"
#include "TokenDeleteTransaction.h"
#include "TokenFeeScheduleUpdateTransaction.h"
#include "TokenMintTransaction.h"
#include "TokenUpdateTransaction.h"
#include "TokenWipeTransaction.h"
#include "TransactionId.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "exceptions/IllegalStateException.h"
#include "exceptions/UninitializedException.h"
#include "impl/DurationConverter.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <proto/basic_types.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <proto/transaction_contents.pb.h>
#include <proto/transaction_list.pb.h>
#include <vector>

namespace Hedera
{
//-----
template<typename SdkRequestType>
std::pair<int,
          std::variant<AccountCreateTransaction,
                       TransferTransaction,
                       AccountUpdateTransaction,
                       AccountDeleteTransaction,
                       AccountAllowanceApproveTransaction,
                       AccountAllowanceDeleteTransaction,
                       ContractCreateTransaction,
                       ContractDeleteTransaction,
                       FileCreateTransaction,
                       FileDeleteTransaction,
                       ContractExecuteTransaction,
                       ContractUpdateTransaction,
                       EthereumTransaction,
                       FileUpdateTransaction,
                       FileAppendTransaction,
                       TokenCreateTransaction,
                       TokenDeleteTransaction,
                       TokenAssociateTransaction,
                       TokenMintTransaction,
                       TokenUpdateTransaction,
                       TokenWipeTransaction,
                       TokenBurnTransaction,
                       TokenFeeScheduleUpdateTransaction>>
Transaction<SdkRequestType>::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::TransactionBody txBody;

  // TransactionList protobuf object
  if (proto::TransactionList txList;
      txList.ParseFromArray(bytes.data(), static_cast<int>(bytes.size())) && txList.transaction_list_size() > 0)
  {
    proto::SignedTransaction signedTx;
    signedTx.ParseFromArray(txList.transaction_list(0).signedtransactionbytes().data(),
                            static_cast<int>(txList.transaction_list(0).signedtransactionbytes().size()));
    txBody.ParseFromArray(signedTx.bodybytes().data(), static_cast<int>(signedTx.bodybytes().size()));
  }

  // Transaction protobuf object
  if (proto::Transaction tx; txBody.data_case() == proto::TransactionBody::DataCase::DATA_NOT_SET &&
                             tx.ParseFromArray(bytes.data(), static_cast<int>(bytes.size())) &&
                             !tx.signedtransactionbytes().empty())
  {
    proto::SignedTransaction signedTx;
    signedTx.ParseFromArray(tx.signedtransactionbytes().data(), static_cast<int>(tx.signedtransactionbytes().size()));
    txBody.ParseFromArray(signedTx.bodybytes().data(), static_cast<int>(signedTx.bodybytes().size()));
  }

  // SignedTransaction protobuf object
  if (proto::SignedTransaction signedTx; txBody.data_case() == proto::TransactionBody::DataCase::DATA_NOT_SET &&
                                         signedTx.ParseFromArray(bytes.data(), static_cast<int>(bytes.size())) &&
                                         !signedTx.bodybytes().empty())
  {
    txBody.ParseFromArray(signedTx.bodybytes().data(), static_cast<int>(signedTx.bodybytes().size()));
  }

  // If not a TransactionBody protobuf object, throw
  if (txBody.data_case() == proto::TransactionBody::DataCase::DATA_NOT_SET &&
      !txBody.ParseFromArray(bytes.data(), static_cast<int>(bytes.size())))
  {
    throw std::invalid_argument("Unable to construct Transaction from input bytes");
  }

  switch (txBody.data_case())
  {
    case proto::TransactionBody::kCryptoCreateAccount:
      return { 0, AccountCreateTransaction(txBody) };
    case proto::TransactionBody::kCryptoTransfer:
      return { 1, TransferTransaction(txBody) };
    case proto::TransactionBody::kCryptoUpdateAccount:
      return { 2, AccountUpdateTransaction(txBody) };
    case proto::TransactionBody::kCryptoDelete:
      return { 3, AccountDeleteTransaction(txBody) };
    case proto::TransactionBody::kCryptoApproveAllowance:
      return { 4, AccountAllowanceApproveTransaction(txBody) };
    case proto::TransactionBody::kCryptoDeleteAllowance:
      return { 5, AccountAllowanceDeleteTransaction(txBody) };
    case proto::TransactionBody::kContractCreateInstance:
      return { 6, ContractCreateTransaction(txBody) };
    case proto::TransactionBody::kContractDeleteInstance:
      return { 7, ContractDeleteTransaction(txBody) };
    case proto::TransactionBody::kFileCreate:
      return { 8, FileCreateTransaction(txBody) };
    case proto::TransactionBody::kFileDelete:
      return { 9, FileDeleteTransaction(txBody) };
    case proto::TransactionBody::kContractCall:
      return { 10, ContractExecuteTransaction(txBody) };
    case proto::TransactionBody::kContractUpdateInstance:
      return { 11, ContractUpdateTransaction(txBody) };
    case proto::TransactionBody::kEthereumTransaction:
      return { 12, EthereumTransaction(txBody) };
    case proto::TransactionBody::kFileUpdate:
      return { 13, FileUpdateTransaction(txBody) };
    case proto::TransactionBody::kFileAppend:
      return { 14, FileAppendTransaction(txBody) };
    case proto::TransactionBody::kTokenCreation:
      return { 15, TokenCreateTransaction(txBody) };
    case proto::TransactionBody::kTokenDeletion:
      return { 16, TokenDeleteTransaction(txBody) };
    case proto::TransactionBody::kTokenAssociate:
      return { 17, TokenAssociateTransaction(txBody) };
    case proto::TransactionBody::kTokenMint:
      return { 18, TokenMintTransaction(txBody) };
    case proto::TransactionBody::kTokenUpdate:
      return { 19, TokenUpdateTransaction(txBody) };
    case proto::TransactionBody::kTokenWipe:
      return { 20, TokenWipeTransaction(txBody) };
    case proto::TransactionBody::kTokenBurn:
      return { 21, TokenBurnTransaction(txBody) };
    case proto::TransactionBody::kTokenFeeScheduleUpdate:
      return { 22, TokenFeeScheduleUpdateTransaction(txBody) };
    default:
      throw std::invalid_argument("Type of transaction cannot be determined from input bytes");
  }
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::sign(const PrivateKey* key)
{
  return signWith(key->getPublicKey(), [key](const std::vector<std::byte>& vec) { return key->sign(vec); });
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::signWith(
  const std::shared_ptr<PublicKey>& key,
  const std::function<std::vector<std::byte>(const std::vector<std::byte>&)>& signer)
{
  if (!mIsFrozen)
  {
    throw IllegalStateException("Transaction must be frozen in order to sign");
  }

  mSignatures.emplace_back(key, signer);

  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::freezeWith(const Client& client)
{
  if (!client.getOperatorAccountId().has_value())
  {
    throw UninitializedException("Client operator has not been initialized and cannot freeze transaction");
  }

  mIsFrozen = true;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setValidTransactionDuration(const std::chrono::duration<double>& duration)
{
  requireNotFrozen();

  mTransactionValidDuration = duration;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setMaxTransactionFee(const Hbar& fee)
{
  requireNotFrozen();

  mMaxTransactionFee = fee;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setTransactionMemo(const std::string& memo)
{
  requireNotFrozen();

  mTransactionMemo = memo;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setTransactionId(const TransactionId& id)
{
  requireNotFrozen();

  mTransactionId = id;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setRegenerateTransactionIdPolicy(bool regenerate)
{
  requireNotFrozen();

  mTransactionIdRegenerationPolicy = regenerate;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
Transaction<SdkRequestType>::Transaction(const proto::TransactionBody& transactionBody)
{
  if (transactionBody.has_nodeaccountid())
  {
    mNodeAccountId = AccountId::fromProtobuf(transactionBody.nodeaccountid());
  }

  mTransactionMemo = transactionBody.memo();

  if (transactionBody.has_transactionvalidduration())
  {
    mTransactionValidDuration = internal::DurationConverter::fromProtobuf(transactionBody.transactionvalidduration());
  }

  if (transactionBody.has_transactionid())
  {
    mTransactionId = TransactionId::fromProtobuf(transactionBody.transactionid());
  }

  if (transactionBody.transactionfee() != static_cast<uint64_t>(DEFAULT_MAX_TRANSACTION_FEE.toTinybars()))
  {
    mMaxTransactionFee = Hbar(static_cast<int64_t>(transactionBody.transactionfee()), HbarUnit::TINYBAR());
  }
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
    std::vector<std::byte> signature = client.sign(internal::Utilities::stringToByteVector(*transactionBodySerialized));

    // Generate a protobuf SignaturePair from a protobuf SignatureMap
    auto signatureMap = std::make_unique<proto::SignatureMap>();
    proto::SignaturePair* signaturePair = signatureMap->add_sigpair();
    signaturePair->set_allocated_pubkeyprefix(
      new std::string(internal::Utilities::byteVectorToString(client.getOperatorPublicKey()->toBytesRaw())));

    if (dynamic_cast<ED25519PublicKey*>(client.getOperatorPublicKey().get()))
    {
      signaturePair->set_allocated_ed25519(new std::string(internal::Utilities::byteVectorToString(signature)));
    }
    else
    {
      signaturePair->set_allocated_ecdsa_secp256k1(new std::string(internal::Utilities::byteVectorToString(signature)));
    }

    // Add other signatures
    for (const auto& [publicKey, signer] : mSignatures)
    {
      signature = signer(internal::Utilities::stringToByteVector(*transactionBodySerialized));
      signaturePair = signatureMap->add_sigpair();
      signaturePair->set_allocated_pubkeyprefix(
        new std::string(internal::Utilities::byteVectorToString(publicKey->toBytesRaw())));

      if (dynamic_cast<ED25519PublicKey*>(publicKey.get()))
      {
        signaturePair->set_allocated_ed25519(new std::string(internal::Utilities::byteVectorToString(signature)));
      }
      else
      {
        signaturePair->set_allocated_ecdsa_secp256k1(
          new std::string(internal::Utilities::byteVectorToString(signature)));
      }
    }

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
void Transaction<SdkRequestType>::requireNotFrozen() const
{
  if (mIsFrozen)
  {
    throw IllegalStateException("Transaction is immutable and cannot be edited");
  }
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
  return gProtobufResponseCodeToStatus.at(response.nodetransactionprecheckcode());
}

//-----
template<typename SdkRequestType>
typename Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
  ExecutionStatus
  Transaction<SdkRequestType>::determineStatus(Status status,
                                               const Client& client,
                                               const proto::TransactionResponse& response)
{
  // If the Transaction's not expired, forward to the base class.
  if (status != Status::TRANSACTION_EXPIRED)
  {
    return Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
      determineStatus(status, client, response);
  }

  // Regenerate transaction IDs by default
  bool shouldRegenerate = true;

  // Follow this Transaction's policy if it has been explicitly set
  if (mTransactionIdRegenerationPolicy)
  {
    shouldRegenerate = *mTransactionIdRegenerationPolicy;
  }

  // Follow the Client's policy if this Transaction's policy hasn't been explicitly set and the Client's policy has
  // been
  else if (const std::optional<bool> clientTxIdRegenPolicy = client.getTransactionIdRegenerationPolicy();
           clientTxIdRegenPolicy)
  {
    shouldRegenerate = *clientTxIdRegenPolicy;
  }

  if (shouldRegenerate)
  {
    // Regenerate the transaction ID and return RETRY if transaction IDs are allowed to be regenerated
    mTransactionId = TransactionId::generate(mTransactionId.getAccountId());
    return Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
      ExecutionStatus::RETRY;
  }
  else
  {
    // Return REQUEST_ERROR if the transaction expired but transaction IDs aren't allowed to be regenerated
    return Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
      ExecutionStatus::REQUEST_ERROR;
  }
}

//-----
template<typename SdkRequestType>
void Transaction<SdkRequestType>::onExecute(const Client& client)
{
  if (!client.getOperatorAccountId())
  {
    throw UninitializedException("No client operator private key with which to sign");
  }

  // Set the transaction ID if it has not already been manually set.
  if (mTransactionId.getAccountId() == AccountId())
  {
    mTransactionId = TransactionId::generate(*client.getOperatorAccountId());
  }
}

/**
 * Explicit template instantiation.
 */
template class Transaction<AccountAllowanceApproveTransaction>;
template class Transaction<AccountAllowanceDeleteTransaction>;
template class Transaction<AccountCreateTransaction>;
template class Transaction<AccountDeleteTransaction>;
template class Transaction<AccountUpdateTransaction>;
template class Transaction<ContractCreateTransaction>;
template class Transaction<ContractDeleteTransaction>;
template class Transaction<ContractExecuteTransaction>;
template class Transaction<ContractUpdateTransaction>;
template class Transaction<EthereumTransaction>;
template class Transaction<FileAppendTransaction>;
template class Transaction<FileCreateTransaction>;
template class Transaction<FileDeleteTransaction>;
template class Transaction<FileUpdateTransaction>;
template class Transaction<TokenAssociateTransaction>;
template class Transaction<TokenBurnTransaction>;
template class Transaction<TokenCreateTransaction>;
template class Transaction<TokenDeleteTransaction>;
template class Transaction<TokenFeeScheduleUpdateTransaction>;
template class Transaction<TokenMintTransaction>;
template class Transaction<TokenUpdateTransaction>;
template class Transaction<TokenWipeTransaction>;
template class Transaction<TransferTransaction>;

} // namespace Hedera

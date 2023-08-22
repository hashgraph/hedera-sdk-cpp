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
#include "ScheduleCreateTransaction.h"
#include "ScheduleDeleteTransaction.h"
#include "ScheduleSignTransaction.h"
#include "Status.h"
#include "TokenAssociateTransaction.h"
#include "TokenBurnTransaction.h"
#include "TokenCreateTransaction.h"
#include "TokenDeleteTransaction.h"
#include "TokenDissociateTransaction.h"
#include "TokenFeeScheduleUpdateTransaction.h"
#include "TokenFreezeTransaction.h"
#include "TokenGrantKycTransaction.h"
#include "TokenMintTransaction.h"
#include "TokenPauseTransaction.h"
#include "TokenRevokeKycTransaction.h"
#include "TokenUnfreezeTransaction.h"
#include "TokenUnpauseTransaction.h"
#include "TokenUpdateTransaction.h"
#include "TokenWipeTransaction.h"
#include "TopicCreateTransaction.h"
#include "TopicDeleteTransaction.h"
#include "TopicMessageSubmitTransaction.h"
#include "TopicUpdateTransaction.h"
#include "TransactionId.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "WrappedTransaction.h"
#include "exceptions/IllegalStateException.h"
#include "exceptions/UnsupportedOperationException.h"
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
WrappedTransaction Transaction<SdkRequestType>::fromBytes(const std::vector<std::byte>& bytes)
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
    case proto::TransactionBody::kCryptoApproveAllowance:
      return WrappedTransaction(AccountAllowanceApproveTransaction(txBody));
    case proto::TransactionBody::kCryptoDeleteAllowance:
      return WrappedTransaction(AccountAllowanceDeleteTransaction(txBody));
    case proto::TransactionBody::kCryptoCreateAccount:
      return WrappedTransaction(AccountCreateTransaction(txBody));
    case proto::TransactionBody::kCryptoDelete:
      return WrappedTransaction(AccountDeleteTransaction(txBody));
    case proto::TransactionBody::kCryptoUpdateAccount:
      return WrappedTransaction(AccountUpdateTransaction(txBody));
    case proto::TransactionBody::kContractCreateInstance:
      return WrappedTransaction(ContractCreateTransaction(txBody));
    case proto::TransactionBody::kContractDeleteInstance:
      return WrappedTransaction(ContractDeleteTransaction(txBody));
    case proto::TransactionBody::kContractCall:
      return WrappedTransaction(ContractExecuteTransaction(txBody));
    case proto::TransactionBody::kContractUpdateInstance:
      return WrappedTransaction(ContractUpdateTransaction(txBody));
    case proto::TransactionBody::kEthereumTransaction:
      return WrappedTransaction(EthereumTransaction(txBody));
    case proto::TransactionBody::kFileAppend:
      return WrappedTransaction(FileAppendTransaction(txBody));
    case proto::TransactionBody::kFileCreate:
      return WrappedTransaction(FileCreateTransaction(txBody));
    case proto::TransactionBody::kFileDelete:
      return WrappedTransaction(FileDeleteTransaction(txBody));
    case proto::TransactionBody::kFileUpdate:
      return WrappedTransaction(FileUpdateTransaction(txBody));
    case proto::TransactionBody::kScheduleCreate:
      return WrappedTransaction(ScheduleCreateTransaction(txBody));
    case proto::TransactionBody::kScheduleDelete:
      return WrappedTransaction(ScheduleDeleteTransaction(txBody));
    case proto::TransactionBody::kScheduleSign:
      return WrappedTransaction(ScheduleSignTransaction(txBody));
    case proto::TransactionBody::kTokenAssociate:
      return WrappedTransaction(TokenAssociateTransaction(txBody));
    case proto::TransactionBody::kTokenBurn:
      return WrappedTransaction(TokenBurnTransaction(txBody));
    case proto::TransactionBody::kTokenCreation:
      return WrappedTransaction(TokenCreateTransaction(txBody));
    case proto::TransactionBody::kTokenDeletion:
      return WrappedTransaction(TokenDeleteTransaction(txBody));
    case proto::TransactionBody::kTokenDissociate:
      return WrappedTransaction(TokenDissociateTransaction(txBody));
    case proto::TransactionBody::kTokenFeeScheduleUpdate:
      return WrappedTransaction(TokenFeeScheduleUpdateTransaction(txBody));
    case proto::TransactionBody::kTokenFreeze:
      return WrappedTransaction(TokenFreezeTransaction(txBody));
    case proto::TransactionBody::kTokenGrantKyc:
      return WrappedTransaction(TokenGrantKycTransaction(txBody));
    case proto::TransactionBody::kTokenMint:
      return WrappedTransaction(TokenMintTransaction(txBody));
    case proto::TransactionBody::kTokenPause:
      return WrappedTransaction(TokenPauseTransaction(txBody));
    case proto::TransactionBody::kTokenRevokeKyc:
      return WrappedTransaction(TokenRevokeKycTransaction(txBody));
    case proto::TransactionBody::kTokenUnfreeze:
      return WrappedTransaction(TokenUnfreezeTransaction(txBody));
    case proto::TransactionBody::kTokenUnpause:
      return WrappedTransaction(TokenUnpauseTransaction(txBody));
    case proto::TransactionBody::kTokenUpdate:
      return WrappedTransaction(TokenUpdateTransaction(txBody));
    case proto::TransactionBody::kTokenWipe:
      return WrappedTransaction(TokenWipeTransaction(txBody));
    case proto::TransactionBody::kConsensusCreateTopic:
      return WrappedTransaction(TopicCreateTransaction(txBody));
    case proto::TransactionBody::kConsensusDeleteTopic:
      return WrappedTransaction(TopicDeleteTransaction(txBody));
    case proto::TransactionBody::kConsensusSubmitMessage:
      return WrappedTransaction(TopicMessageSubmitTransaction(txBody));
    case proto::TransactionBody::kConsensusUpdateTopic:
      return WrappedTransaction(TopicUpdateTransaction(txBody));
    case proto::TransactionBody::kCryptoTransfer:
      return WrappedTransaction(TransferTransaction(txBody));
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
SdkRequestType& Transaction<SdkRequestType>::freezeWith(const Client* client)
{
  if (mIsFrozen)
  {
    return static_cast<SdkRequestType&>(*this);
  }

  if (mTransactionId == TransactionId())
  {
    if (!client)
    {
      throw IllegalStateException(
        "If no client is provided to freeze transaction, the transaction ID must be manually set");
    }

    if (!client->getOperatorAccountId().has_value())
    {
      throw IllegalStateException("Client operator has not been initialized and cannot freeze transaction");
    }

    // Generate a transaction ID with the client.
    mTransactionId = TransactionId::generate(client->getOperatorAccountId().value());
  }

  mIsFrozen = true;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
ScheduleCreateTransaction Transaction<SdkRequestType>::schedule() const
{
  requireNotFrozen();
  if (!Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
         getNodeAccountIds()
           .empty())
  {
    throw IllegalStateException("Underlying transaction for a scheduled transaction cannot have node account IDs set");
  }

  return ScheduleCreateTransaction().setScheduledTransaction(
    WrappedTransaction::fromProtobuf(generateTransactionBody(nullptr)));
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
TransactionId Transaction<SdkRequestType>::getTransactionId() const
{
  if (mTransactionId == TransactionId() || !mIsFrozen)
  {
    throw IllegalStateException("No transaction ID generated yet. Try freezing the transaction");
  }

  return mTransactionId;
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
proto::TransactionBody Transaction<SdkRequestType>::generateTransactionBody(const Client* client) const
{
  proto::TransactionBody body;
  body.set_allocated_transactionid(mTransactionId.toProtobuf().release());
  body.set_transactionfee(static_cast<uint64_t>(getMaxTransactionFee(client).toTinybars()));
  body.set_memo(mTransactionMemo);
  body.set_allocated_transactionvalidduration(internal::DurationConverter::toProtobuf(mTransactionValidDuration));
  body.set_allocated_nodeaccountid(mNodeAccountId.toProtobuf().release());

  // Add derived Transaction fields to TransactionBody.
  addToBody(body);

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
void Transaction<SdkRequestType>::onExecute(const Client& client)
{
  if (!mIsFrozen)
  {
    freezeWith(&client);
  }
}

//-----
template<typename SdkRequestType>
Hbar Transaction<SdkRequestType>::getMaxTransactionFee(const Client* client) const
{
  if (mMaxTransactionFee.has_value())
  {
    return mMaxTransactionFee.value();
  }

  if (client && client->getMaxTransactionFee().has_value())
  {
    return client->getMaxTransactionFee().value();
  }

  return DEFAULT_MAX_TRANSACTION_FEE;
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
    // Regenerate the transaction ID and return RETRY if transaction IDs are allowed to be regenerated.
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

/**
 * Explicit template instantiations.
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
template class Transaction<ScheduleCreateTransaction>;
template class Transaction<ScheduleDeleteTransaction>;
template class Transaction<ScheduleSignTransaction>;
template class Transaction<TokenAssociateTransaction>;
template class Transaction<TokenBurnTransaction>;
template class Transaction<TokenCreateTransaction>;
template class Transaction<TokenDeleteTransaction>;
template class Transaction<TokenDissociateTransaction>;
template class Transaction<TokenFeeScheduleUpdateTransaction>;
template class Transaction<TokenFreezeTransaction>;
template class Transaction<TokenGrantKycTransaction>;
template class Transaction<TokenMintTransaction>;
template class Transaction<TokenPauseTransaction>;
template class Transaction<TokenRevokeKycTransaction>;
template class Transaction<TokenUnfreezeTransaction>;
template class Transaction<TokenUnpauseTransaction>;
template class Transaction<TokenUpdateTransaction>;
template class Transaction<TokenWipeTransaction>;
template class Transaction<TopicCreateTransaction>;
template class Transaction<TopicDeleteTransaction>;
template class Transaction<TopicMessageSubmitTransaction>;
template class Transaction<TopicUpdateTransaction>;
template class Transaction<TransferTransaction>;

} // namespace Hedera

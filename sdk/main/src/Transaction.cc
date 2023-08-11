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
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(AccountAllowanceApproveTransaction(txBody)));
    case proto::TransactionBody::kCryptoDeleteAllowance:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(AccountAllowanceDeleteTransaction(txBody)));
    case proto::TransactionBody::kCryptoCreateAccount:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(AccountCreateTransaction(txBody)));
    case proto::TransactionBody::kCryptoDelete:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(AccountDeleteTransaction(txBody)));
    case proto::TransactionBody::kCryptoUpdateAccount:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(AccountUpdateTransaction(txBody)));
    case proto::TransactionBody::kContractCreateInstance:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(ContractCreateTransaction(txBody)));
    case proto::TransactionBody::kContractDeleteInstance:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(ContractDeleteTransaction(txBody)));
    case proto::TransactionBody::kContractCall:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(ContractExecuteTransaction(txBody)));
    case proto::TransactionBody::kContractUpdateInstance:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(ContractUpdateTransaction(txBody)));
    case proto::TransactionBody::kEthereumTransaction:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(EthereumTransaction(txBody)));
    case proto::TransactionBody::kFileAppend:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(FileAppendTransaction(txBody)));
    case proto::TransactionBody::kFileCreate:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(FileCreateTransaction(txBody)));
    case proto::TransactionBody::kFileDelete:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(FileDeleteTransaction(txBody)));
    case proto::TransactionBody::kFileUpdate:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(FileUpdateTransaction(txBody)));
    case proto::TransactionBody::kTokenAssociate:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenAssociateTransaction(txBody)));
    case proto::TransactionBody::kTokenBurn:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenBurnTransaction(txBody)));
    case proto::TransactionBody::kTokenCreation:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenCreateTransaction(txBody)));
    case proto::TransactionBody::kTokenDeletion:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenDeleteTransaction(txBody)));
    case proto::TransactionBody::kTokenDissociate:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenDissociateTransaction(txBody)));
    case proto::TransactionBody::kTokenFeeScheduleUpdate:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenFeeScheduleUpdateTransaction(txBody)));
    case proto::TransactionBody::kTokenFreeze:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenFreezeTransaction(txBody)));
    case proto::TransactionBody::kTokenGrantKyc:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenGrantKycTransaction(txBody)));
    case proto::TransactionBody::kTokenMint:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenMintTransaction(txBody)));
    case proto::TransactionBody::kTokenPause:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenPauseTransaction(txBody)));
    case proto::TransactionBody::kTokenRevokeKyc:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenRevokeKycTransaction(txBody)));
    case proto::TransactionBody::kTokenUnfreeze:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenUnfreezeTransaction(txBody)));
    case proto::TransactionBody::kTokenUnpause:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenUnpauseTransaction(txBody)));
    case proto::TransactionBody::kTokenUpdate:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenUpdateTransaction(txBody)));
    case proto::TransactionBody::kTokenWipe:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TokenWipeTransaction(txBody)));
    case proto::TransactionBody::kConsensusCreateTopic:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TopicCreateTransaction(txBody)));
    case proto::TransactionBody::kConsensusDeleteTopic:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TopicDeleteTransaction(txBody)));
    case proto::TransactionBody::kConsensusSubmitMessage:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TopicMessageSubmitTransaction(txBody)));
    case proto::TransactionBody::kConsensusUpdateTopic:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TopicUpdateTransaction(txBody)));
    case proto::TransactionBody::kCryptoTransfer:
      return WrappedTransaction(WrappedTransaction::AnyPossibleTransaction(TransferTransaction(txBody)));
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

  proto::TransactionBody body = generateTransactionBody(nullptr);
  addToBody(body);

  proto::SchedulableTransactionBody schedulableTransactionBody;
  schedulableTransactionBody.set_transactionfee(body.transactionfee());
  schedulableTransactionBody.set_memo(body.memo());

  if (body.has_cryptoapproveallowance())
  {
    schedulableTransactionBody.set_allocated_cryptoapproveallowance(body.release_cryptoapproveallowance());
  }
  else if (body.has_cryptodeleteallowance())
  {
    schedulableTransactionBody.set_allocated_cryptodeleteallowance(body.release_cryptodeleteallowance());
  }
  else if (body.has_cryptocreateaccount())
  {
    schedulableTransactionBody.set_allocated_cryptocreateaccount(body.release_cryptocreateaccount());
  }
  else if (body.has_cryptodeleteallowance())
  {
    schedulableTransactionBody.set_allocated_cryptodelete(body.release_cryptodelete());
  }
  else if (body.has_cryptoupdateaccount())
  {
    schedulableTransactionBody.set_allocated_cryptoupdateaccount(body.release_cryptoupdateaccount());
  }
  else if (body.has_contractcreateinstance())
  {
    schedulableTransactionBody.set_allocated_contractcreateinstance(body.release_contractcreateinstance());
  }
  else if (body.has_contractdeleteinstance())
  {
    schedulableTransactionBody.set_allocated_contractdeleteinstance(body.release_contractdeleteinstance());
  }
  else if (body.has_contractcall())
  {
    schedulableTransactionBody.set_allocated_contractcall(body.release_contractcall());
  }
  else if (body.has_contractupdateinstance())
  {
    schedulableTransactionBody.set_allocated_contractupdateinstance(body.release_contractupdateinstance());
  }
  else if (body.has_fileappend())
  {
    schedulableTransactionBody.set_allocated_fileappend(body.release_fileappend());
  }
  else if (body.has_filecreate())
  {
    schedulableTransactionBody.set_allocated_filecreate(body.release_filecreate());
  }
  else if (body.has_filedelete())
  {
    schedulableTransactionBody.set_allocated_filedelete(body.release_filedelete());
  }
  else if (body.has_fileupdate())
  {
    schedulableTransactionBody.set_allocated_fileupdate(body.release_fileupdate());
  }
  else if (body.has_tokenassociate())
  {
    schedulableTransactionBody.set_allocated_tokenassociate(body.release_tokenassociate());
  }
  else if (body.has_tokenburn())
  {
    schedulableTransactionBody.set_allocated_tokenburn(body.release_tokenburn());
  }
  else if (body.has_tokencreation())
  {
    schedulableTransactionBody.set_allocated_tokencreation(body.release_tokencreation());
  }
  else if (body.has_tokendeletion())
  {
    schedulableTransactionBody.set_allocated_tokendeletion(body.release_tokendeletion());
  }
  else if (body.has_tokendissociate())
  {
    schedulableTransactionBody.set_allocated_tokendissociate(body.release_tokendissociate());
  }
  else if (body.has_token_fee_schedule_update())
  {
    schedulableTransactionBody.set_allocated_token_fee_schedule_update(body.release_token_fee_schedule_update());
  }
  else if (body.has_tokenfreeze())
  {
    schedulableTransactionBody.set_allocated_tokenfreeze(body.release_tokenfreeze());
  }
  else if (body.has_tokengrantkyc())
  {
    schedulableTransactionBody.set_allocated_tokengrantkyc(body.release_tokengrantkyc());
  }
  else if (body.has_tokenmint())
  {
    schedulableTransactionBody.set_allocated_tokenmint(body.release_tokenmint());
  }
  else if (body.has_token_pause())
  {
    schedulableTransactionBody.set_allocated_token_pause(body.release_token_pause());
  }
  else if (body.has_tokenrevokekyc())
  {
    schedulableTransactionBody.set_allocated_tokenrevokekyc(body.release_tokenrevokekyc());
  }
  else if (body.has_tokenunfreeze())
  {
    schedulableTransactionBody.set_allocated_tokenunfreeze(body.release_tokenunfreeze());
  }
  else if (body.has_token_unpause())
  {
    schedulableTransactionBody.set_allocated_token_unpause(body.release_token_unpause());
  }
  else if (body.has_tokenwipe())
  {
    schedulableTransactionBody.set_allocated_tokenwipe(body.release_tokenwipe());
  }
  else if (body.has_consensuscreatetopic())
  {
    schedulableTransactionBody.set_allocated_consensuscreatetopic(body.release_consensuscreatetopic());
  }
  else if (body.has_consensusdeletetopic())
  {
    schedulableTransactionBody.set_allocated_consensusdeletetopic(body.release_consensusdeletetopic());
  }
  else if (body.has_cryptotransfer())
  {
    schedulableTransactionBody.set_allocated_cryptotransfer(body.release_cryptotransfer());
  }
  else
  {
    throw UnsupportedOperationException("Transaction cannot be scheduled");
  }

  return ScheduleCreateTransaction().setScheduledTransaction(schedulableTransactionBody);
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
proto::TransactionBody Transaction<SdkRequestType>::generateTransactionBody(const Client* client) const
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
  if (!mIsFrozen)
  {
    freezeWith(&client);
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
template class Transaction<ScheduleCreateTransaction>;
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

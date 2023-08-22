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
#include "TransferTransaction.h"
#include "TransactionResponse.h"
#include "impl/Node.h"

#include <proto/crypto_transfer.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction.pb.h>

namespace Hedera
{

//-----
TransferTransaction::TransferTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TransferTransaction>(transactionBody)
{
  if (!transactionBody.has_cryptotransfer())
  {
    throw std::invalid_argument("Transaction body doesn't contain CryptoTransfer data");
  }

  const proto::CryptoTransferTransactionBody& body = transactionBody.cryptotransfer();

  for (int i = 0; i < body.transfers().accountamounts_size(); ++i)
  {
    const proto::AccountAmount& accountAmount = body.transfers().accountamounts(i);
    mHbarTransfers.push_back(HbarTransfer()
                               .setAccountId(AccountId::fromProtobuf(accountAmount.accountid()))
                               .setAmount(Hbar(accountAmount.amount(), HbarUnit::TINYBAR()))
                               .setApproved(accountAmount.is_approval()));
  }

  for (int i = 0; i < body.tokentransfers_size(); ++i)
  {
    const proto::TokenTransferList& transfer = body.tokentransfers(i);
    const TokenId tokenId = TokenId::fromProtobuf(transfer.token());

    for (int j = 0; j < transfer.transfers_size(); ++j)
    {
      const proto::AccountAmount& accountAmount = transfer.transfers(j);
      mTokenTransfers.emplace_back(tokenId,
                                   AccountId::fromProtobuf(accountAmount.accountid()),
                                   accountAmount.amount(),
                                   transfer.expected_decimals().value(),
                                   accountAmount.is_approval());
    }

    for (int j = 0; j < transfer.nfttransfers_size(); ++j)
    {
      const proto::NftTransfer& nftTransfer = transfer.nfttransfers(j);
      mNftTransfers.push_back(TokenNftTransfer()
                                .setNftId(NftId(tokenId, static_cast<uint64_t>(nftTransfer.serialnumber())))
                                .setSenderAccountId(AccountId::fromProtobuf(nftTransfer.senderaccountid()))
                                .setReceiverAccountId(AccountId::fromProtobuf(nftTransfer.receiveraccountid()))
                                .setApproval(nftTransfer.is_approval()));
    }
  }
}

//-----
TransferTransaction& TransferTransaction::addHbarTransfer(const AccountId& accountId, const Hbar& amount)
{
  requireNotFrozen();

  doHbarTransfer(HbarTransfer().setAccountId(accountId).setAmount(amount).setApproved(false));
  return *this;
}

//-----
TransferTransaction& TransferTransaction::addTokenTransfer(const TokenId& tokenId,
                                                           const AccountId& accountId,
                                                           const int64_t& amount)
{
  requireNotFrozen();

  doTokenTransfer(TokenTransfer(tokenId, accountId, amount, false));
  return *this;
}

//-----
TransferTransaction& TransferTransaction::addNftTransfer(const NftId& nftId,
                                                         const AccountId& senderAccountId,
                                                         const AccountId& receiverAccountId)
{
  requireNotFrozen();

  doNftTransfer(TokenNftTransfer()
                  .setNftId(nftId)
                  .setSenderAccountId(senderAccountId)
                  .setReceiverAccountId(receiverAccountId)
                  .setApproval(false));
  return *this;
}

//-----
TransferTransaction& TransferTransaction::addTokenTransferWithDecimals(const TokenId& tokenId,
                                                                       const AccountId& accountId,
                                                                       const int64_t& amount,
                                                                       uint32_t decimals)
{
  requireNotFrozen();

  doTokenTransfer(TokenTransfer(tokenId, accountId, amount, decimals, false));
  return *this;
}

//-----
TransferTransaction& TransferTransaction::addApprovedHbarTransfer(const AccountId& accountId, const Hbar& amount)
{
  requireNotFrozen();

  doHbarTransfer(HbarTransfer().setAccountId(accountId).setAmount(amount).setApproved(true));
  return *this;
}

//-----
TransferTransaction& TransferTransaction::addApprovedTokenTransfer(const TokenId& tokenId,
                                                                   const AccountId& accountId,
                                                                   const int64_t& amount)
{
  requireNotFrozen();

  doTokenTransfer(TokenTransfer(tokenId, accountId, amount, true));
  return *this;
}

//-----
TransferTransaction& TransferTransaction::addApprovedNftTransfer(const NftId& nftId,
                                                                 const AccountId& senderAccountId,
                                                                 const AccountId& receiverAccountId)
{
  requireNotFrozen();

  doNftTransfer(TokenNftTransfer()
                  .setNftId(nftId)
                  .setSenderAccountId(senderAccountId)
                  .setReceiverAccountId(receiverAccountId)
                  .setApproval(true));
  return *this;
}

//-----
TransferTransaction& TransferTransaction::addApprovedTokenTransferWithDecimals(const TokenId& tokenId,
                                                                               const AccountId& accountId,
                                                                               const int64_t& amount,
                                                                               uint32_t decimals)
{
  requireNotFrozen();

  doTokenTransfer(TokenTransfer(tokenId, accountId, amount, decimals, true));
  return *this;
}

//-----
std::unordered_map<AccountId, Hbar> TransferTransaction::getHbarTransfers() const
{
  std::unordered_map<AccountId, Hbar> hbarTransfers;

  for (const HbarTransfer& transfer : mHbarTransfers)
  {
    hbarTransfers[transfer.getAccountId()] += transfer.getAmount();
  }

  return hbarTransfers;
}

//-----
std::unordered_map<TokenId, std::unordered_map<AccountId, int64_t>> TransferTransaction::getTokenTransfers() const
{
  std::unordered_map<TokenId, std::unordered_map<AccountId, int64_t>> tokenTransfers;

  for (const TokenTransfer& transfer : mTokenTransfers)
  {
    tokenTransfers[transfer.mTokenId][transfer.mAccountId] += transfer.mAmount;
  }

  return tokenTransfers;
}

//-----
std::unordered_map<TokenId, std::vector<TokenNftTransfer>> TransferTransaction::getNftTransfers() const
{
  std::unordered_map<TokenId, std::vector<TokenNftTransfer>> nftTransfers;

  for (const TokenNftTransfer& transfer : mNftTransfers)
  {
    nftTransfers[transfer.getNftId().getTokenId()].push_back(transfer);
  }

  return nftTransfers;
}

//-----
std::unordered_map<TokenId, uint32_t> TransferTransaction::getTokenIdDecimals() const
{
  std::unordered_map<TokenId, uint32_t> decimals;

  for (const TokenTransfer& transfer : mTokenTransfers)
  {
    if (transfer.mExpectedDecimals != 0U)
    {
      decimals[transfer.mTokenId] = transfer.mExpectedDecimals;
    }
  }

  return decimals;
}

//-----
proto::Transaction TransferTransaction::makeRequest(const Client& client, const std::shared_ptr<internal::Node>&) const
{
  return signTransaction(generateTransactionBody(&client), client);
}

//-----
grpc::Status TransferTransaction::submitRequest(const Client& client,
                                                const std::chrono::system_clock::time_point& deadline,
                                                const std::shared_ptr<internal::Node>& node,
                                                proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kCryptoTransfer, makeRequest(client, node), deadline, response);
}

//-----
void TransferTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_cryptotransfer(build());
}

//-----
proto::CryptoTransferTransactionBody* TransferTransaction::build() const
{
  auto body = std::make_unique<proto::CryptoTransferTransactionBody>();

  for (const HbarTransfer& transfer : mHbarTransfers)
  {
    *body->mutable_transfers()->add_accountamounts() = *transfer.toProtobuf();
  }

  for (const TokenTransfer& transfer : mTokenTransfers)
  {
    // If this token already has a transfer, grab that token's list. Otherwise, add this token
    proto::TokenTransferList* list = nullptr;
    for (int i = 0; i < body->mutable_tokentransfers()->size(); ++i)
    {
      if (TokenId::fromProtobuf(body->mutable_tokentransfers(i)->token()) == transfer.mTokenId)
      {
        list = body->mutable_tokentransfers(i);
      }
    }

    if (!list)
    {
      list = body->add_tokentransfers();
      list->set_allocated_token(transfer.mTokenId.toProtobuf().release());
    }

    proto::AccountAmount* amount = list->add_transfers();
    amount->set_allocated_accountid(transfer.mAccountId.toProtobuf().release());
    amount->set_amount(transfer.mAmount);
    amount->set_is_approval(transfer.mIsApproval);

    // Shouldn't ever overwrite a different value here because it is checked when the transfer is added to this
    // TransferTransaction.
    list->mutable_expected_decimals()->set_value(transfer.mExpectedDecimals);
  }

  for (const TokenNftTransfer& transfer : mNftTransfers)
  {
    // If this token already has a transfer, grab that token's list. Otherwise, add this token
    proto::TokenTransferList* list = nullptr;
    for (int i = 0; i < body->mutable_tokentransfers()->size(); ++i)
    {
      if (TokenId::fromProtobuf(body->mutable_tokentransfers(i)->token()) == transfer.getNftId().getTokenId())
      {
        list = body->mutable_tokentransfers(i);
      }
    }

    if (!list)
    {
      list = body->add_tokentransfers();
      list->set_allocated_token(transfer.getNftId().getTokenId().toProtobuf().release());
    }

    *list->add_nfttransfers() = *transfer.toProtobuf();
  }

  return body.release();
}

//----
void TransferTransaction::doHbarTransfer(const HbarTransfer& transfer)
{
  // If a transfer has already been added for an account, just update the amount if the approval status is the same
  for (auto transferIter = mHbarTransfers.begin(); transferIter != mHbarTransfers.end(); ++transferIter)
  {
    if (transferIter->getAccountId() == transfer.getAccountId() &&
        transferIter->getApproval() == transfer.getApproval())
    {
      if (const auto newValue =
            Hbar(transferIter->getAmount().toTinybars() + transfer.getAmount().toTinybars(), HbarUnit::TINYBAR());
          newValue.toTinybars() == 0LL)
      {
        mHbarTransfers.erase(transferIter);
      }
      else
      {
        transferIter->setAmount(newValue);
      }

      return;
    }
  }

  mHbarTransfers.push_back(transfer);
}

//-----
void TransferTransaction::doTokenTransfer(const TokenTransfer& transfer)
{
  for (auto transferIter = mTokenTransfers.begin(); transferIter != mTokenTransfers.end(); ++transferIter)
  {
    if (transferIter->mTokenId == transfer.mTokenId && transferIter->mAccountId == transfer.mAccountId &&
        transferIter->mIsApproval == transfer.mIsApproval)
    {
      if (transferIter->mExpectedDecimals != transfer.mExpectedDecimals)
      {
        throw std::invalid_argument("Expected decimals for token do not match previously set decimals");
      }
      else if (const int64_t newAmount = transferIter->mAmount + transfer.mAmount; newAmount == 0LL)
      {
        mTokenTransfers.erase(transferIter);
      }
      else
      {
        transferIter->mAmount = newAmount;
      }

      return;
    }
  }

  mTokenTransfers.push_back(transfer);
}

//-----
void TransferTransaction::doNftTransfer(const TokenNftTransfer& transfer)
{
  for (auto transferIter = mNftTransfers.begin(); transferIter != mNftTransfers.end(); ++transferIter)
  {
    if (transferIter->getNftId().getSerialNum() == transfer.getNftId().getSerialNum() &&
        transferIter->getSenderAccountId() == transfer.getReceiverAccountId() &&
        transferIter->getReceiverAccountId() == transfer.getSenderAccountId() &&
        transferIter->getApproval() == transfer.getApproval())
    {
      mNftTransfers.erase(transferIter);
      return;
    }
  }

  mNftTransfers.push_back(transfer);
}

} // namespace Hedera

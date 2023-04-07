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
#include "AccountCreateTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "HbarTransfer.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <proto/transaction_record.pb.h>

using json = nlohmann::json;
using namespace Hedera;

class TransferTransactionIntegrationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountIdTo() const { return mAccountIdTo; }
  [[nodiscard]] inline const AccountId& getTestAccountIdFrom() const { return mAccountIdFrom; }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }

  void SetUp() override
  {
    const auto accountId = AccountId::fromString("0.0.3");
    const std::string_view accountIdStr = "0.0.3";
    const std::string_view networkTag = "network";
    const std::string_view operatorTag = "operator";
    const std::string_view accountIdTag = "accountId";
    const std::string_view privateKeyTag = "privateKey";

    const std::string testPathToJSON = std::filesystem::current_path() / "local_node.json";
    const std::unique_ptr<PrivateKey> testPrivateKey = ED25519PrivateKey::generatePrivateKey();
    const std::shared_ptr<PublicKey> testPublicKey = testPrivateKey->getPublicKey();

    AccountId operatorAccountId;
    std::string operatorAccountPrivateKey;
    std::ifstream testInputFile(testPathToJSON, std::ios::in);
    std::string nodeAddressString;
    json jsonData = json::parse(testInputFile);

    if (jsonData[networkTag][accountIdStr].is_string())
    {
      nodeAddressString = jsonData[networkTag][accountIdStr];
    }

    if (jsonData[operatorTag][accountIdTag].is_string() && jsonData[operatorTag][privateKeyTag].is_string())
    {
      std::string operatorAccountIdStr = jsonData[operatorTag][accountIdTag];

      operatorAccountId = AccountId::fromString(operatorAccountIdStr);
      operatorAccountPrivateKey = jsonData[operatorTag][privateKeyTag];
    }

    testInputFile.close();

    std::unordered_map<std::string, AccountId> networkMap;
    networkMap.insert(std::pair<std::string, AccountId>(nodeAddressString, accountId));

    mClient = Client::forNetwork(networkMap);
    mClient.setOperator(operatorAccountId, ED25519PrivateKey::fromString(operatorAccountPrivateKey));
  }

private:
  const AccountId mAccountIdFrom = AccountId::fromString("0.0.1024");
  const AccountId mAccountIdTo = AccountId::fromString("0.0.1032");

  Client mClient;
};

// Tests invoking of method execute() from TransferTransaction.
TEST_F(TransferTransactionIntegrationTest, ExecuteRequestToTestnetNode)
{
  // Given
  const auto accountIdFrom = getTestAccountIdFrom();
  const auto accountIdTo = getTestAccountIdTo();
  const int64_t transferAmount = 10LL;
  const std::string txHash = "txHash";
  const std::string txMemo = "txMemo";
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  const uint64_t txFee = 10ULL;
  const auto tokenId = TokenId(10ULL);
  const auto nftId = NftId(TokenId(20ULL), 1000ULL);

  proto::TransactionRecord protoTransactionRecord;
  protoTransactionRecord.mutable_receipt()->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  protoTransactionRecord.set_allocated_transactionhash(new std::string(txHash));
  protoTransactionRecord.set_allocated_consensustimestamp(internal::TimestampConverter::toProtobuf(now));
  protoTransactionRecord.set_allocated_transactionid(TransactionId::generate(accountIdFrom).toProtobuf().release());
  protoTransactionRecord.set_allocated_memo(new std::string(txMemo));
  protoTransactionRecord.set_transactionfee(txFee);

  proto::AccountAmount* aa = protoTransactionRecord.mutable_transferlist()->add_accountamounts();
  aa->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  aa->set_amount(-transferAmount);

  aa = protoTransactionRecord.mutable_transferlist()->add_accountamounts();
  aa->set_allocated_accountid(accountIdTo.toProtobuf().release());
  aa->set_amount(transferAmount);

  proto::TokenTransferList* list = protoTransactionRecord.add_tokentransferlists();
  list->set_allocated_token(tokenId.toProtobuf().release());

  aa = list->add_transfers();
  aa->set_allocated_accountid(accountIdTo.toProtobuf().release());
  aa->set_amount(transferAmount);

  aa = list->add_transfers();
  aa->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  aa->set_amount(-transferAmount);

  list = protoTransactionRecord.add_tokentransferlists();
  list->set_allocated_token(nftId.getTokenId().toProtobuf().release());

  proto::NftTransfer* nft = list->add_nfttransfers();
  nft->set_serialnumber(static_cast<int64_t>(nftId.getSerialNum()));
  nft->set_allocated_senderaccountid(accountIdFrom.toProtobuf().release());
  nft->set_allocated_receiveraccountid(accountIdTo.toProtobuf().release());

  // When
  TransactionRecord txRecord = TransactionRecord::fromProtobuf(protoTransactionRecord);

  // Then
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_TRUE(txRecord.getReceipt()->getAccountId());
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());
  EXPECT_EQ(*txRecord.getReceipt()->getAccountId(), accountIdFrom);
  EXPECT_EQ(txRecord.getTransactionHash(), txHash);
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());

  EXPECT_GE(txRecord.getConsensusTimestamp()->time_since_epoch().count(), now.time_since_epoch().count());

  EXPECT_TRUE(txRecord.getTransactionId().has_value());
  EXPECT_EQ(txRecord.getTransactionId()->getAccountId(), accountIdFrom);
  EXPECT_GE(txRecord.getTransactionId()->getValidTransactionTime(), now);

  EXPECT_EQ(txRecord.getTransactionMemo(), txMemo);

  EXPECT_EQ(txRecord.getTransactionFee(), txFee);

  EXPECT_EQ(txRecord.getHbarTransferList().size(), 2);
  EXPECT_EQ(txRecord.getHbarTransferList().at(0).getAccountId(), accountIdFrom);
  EXPECT_EQ(txRecord.getHbarTransferList().at(0).getAmount().toTinybars(), -transferAmount);
  EXPECT_EQ(txRecord.getHbarTransferList().at(1).getAccountId(), accountIdTo);
  EXPECT_EQ(txRecord.getHbarTransferList().at(1).getAmount().toTinybars(), transferAmount);

  EXPECT_EQ(txRecord.getTokenTransferList().size(), 2);
  EXPECT_EQ(txRecord.getTokenTransferList().at(0).getTokenId(), tokenId);
  EXPECT_EQ(txRecord.getTokenTransferList().at(0).getAccountId(), accountIdTo);
  EXPECT_EQ(txRecord.getTokenTransferList().at(0).getAmount(), transferAmount);
  EXPECT_EQ(txRecord.getTokenTransferList().at(1).getTokenId(), tokenId);
  EXPECT_EQ(txRecord.getTokenTransferList().at(1).getAccountId(), accountIdFrom);
  EXPECT_EQ(txRecord.getTokenTransferList().at(1).getAmount(), -transferAmount);

  EXPECT_EQ(txRecord.getNftTransferList().size(), 1);
  EXPECT_EQ(txRecord.getNftTransferList().at(0).getNftId(), nftId);
  EXPECT_EQ(txRecord.getNftTransferList().at(0).getSenderAccountId(), accountIdFrom);
  EXPECT_EQ(txRecord.getNftTransferList().at(0).getReceiverAccountId(), accountIdTo);

  EXPECT_FALSE(txRecord.getEvmAddress().has_value());
}

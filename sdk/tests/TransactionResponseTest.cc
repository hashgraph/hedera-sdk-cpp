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
#include "TransactionResponse.h"

#include <gtest/gtest.h>
#include <proto/transaction_response.pb.h>

using namespace Hedera;

class TransactionResponseTest : public ::testing::Test
{
};

// Tests deserialization of Hedera::TransactionResponse -> proto::TransactionResponse.
TEST_F(TransactionResponseTest, ProtobufTransactionResponse)
{
  const uint64_t cost = 10ULL;
  const proto::ResponseCodeEnum response = proto::ResponseCodeEnum::AUTHORIZATION_FAILED;
  proto::TransactionResponse protoTransactionResponse;
  protoTransactionResponse.set_cost(cost);
  protoTransactionResponse.set_nodetransactionprecheckcode(response);

  TransactionResponse txResponse = TransactionResponse::fromProtobuf(protoTransactionResponse);
  EXPECT_EQ(txResponse.getCost(), cost);
  EXPECT_FALSE(txResponse.getValidateStatus());

  protoTransactionResponse.set_nodetransactionprecheckcode(proto::ResponseCodeEnum::OK);
  txResponse = TransactionResponse::fromProtobuf(protoTransactionResponse);
  EXPECT_TRUE(txResponse.getValidateStatus());
}

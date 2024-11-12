/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "ED25519PrivateKey.h"
#include "NodeDeleteTransaction.h"

#include <gtest/gtest.h>
#include <transaction_body.pb.h>

using namespace Hiero;

class NodeDeleteTransactionUnitTests : public ::testing::Test
{
protected:
  NodeDeleteTransaction transaction;
};

//-----
TEST_F(NodeDeleteTransactionUnitTests, ConstructNodeDeleteTransactionFromTransactionBodyProtobuf)
{
  // Given
  proto::TransactionBody transactionBody;
  aproto::NodeDeleteTransactionBody* body = transactionBody.mutable_nodedelete();

  body->set_node_id(1);

  // When
  NodeDeleteTransaction nodeDeleteTransaction(transactionBody);

  // Then
  ASSERT_EQ(nodeDeleteTransaction.getNodeId(), 1);
}

//-----
TEST_F(NodeDeleteTransactionUnitTests, SetAndGetNodeId)
{
  // Given
  uint64_t nodeId = 3;

  // When
  transaction.setNodeId(nodeId);

  // Then
  ASSERT_EQ(transaction.getNodeId(), nodeId);
}
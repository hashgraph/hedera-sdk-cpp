/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "BaseIntegrationTest.h"
#include "TokenRejectTransaction.h"

#include <gtest/gtest.h>

using namespace Hedera;

class TokenRejectTransactionIntegrationTests : public BaseIntegrationTest
{
protected:
private:
};

//-----
TEST_F(TokenRejectTransactionIntegrationTests, CanExecuteForFT)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, CanExecuteForNFT)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, CanExecuteForFTAndNFT)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, ReceiverSigRequired)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, TokenFrozen)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, TokenPaused)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, RemovesAllowance)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWhenRejectingNFTWithTokenID)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWithTokenReferenceRepeated)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWhenOwnerHasNoBalance)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsTreasuryRejects)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWithInvalidToken)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWithReferenceSizeExceeded)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWithInvalidSignature)
{
  // Given
  // When
  // Then
}

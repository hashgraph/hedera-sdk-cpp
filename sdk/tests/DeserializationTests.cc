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
#include "AccountBalance.h"
#include "AccountId.h"
#include "ECDSAPrivateKey.h"
#include "ECDSAPublicKey.h"
#include "ExchangeRate.h"
#include "ExchangeRateSet.h"
#include "PublicKey.h"
#include "Status.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"

#include "impl/TimestampConverter.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>
#include <proto/exchange_rate.pb.h>
#include <proto/transaction_receipt.pb.h>
#include <proto/transaction_record.pb.h>
#include <proto/transaction_response.pb.h>
#include <proto/crypto_get_account_balance.pb.h>

using namespace Hedera;

class DeserializationTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const uint64_t getTestShardNum() const { return mShardNum; }
  [[nodiscard]] inline const uint64_t getTestRealmNum() const { return mRealmNum; }
  [[nodiscard]] inline const uint64_t getTestContractNum() const { return mContractNum; }
  [[nodiscard]] inline const int32_t  getTestCents() const { return mCents; }
  [[nodiscard]] inline const int32_t  getTestHbar() const { return mHbar; }
  [[nodiscard]] inline const uint64_t getTestSeconds() const { return mSeconds; }
  [[nodiscard]] inline const Hbar     getTestBalance() const { return mBalance; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const AccountId& getTestAccountIdFrom() const { return mAccountIdFrom; }
  [[nodiscard]] inline const AccountId& getTestAccountIdTo() const { return mAccountIdTo; }
  [[nodiscard]] inline const std::shared_ptr<ECDSAPublicKey>& getTestECDSAPublicKey() const { return mPublicKeyFromString; }

private:
  const uint64_t mShardNum = 1;
  const uint64_t mRealmNum = 2;
  const uint64_t mContractNum = 3;
  const int32_t mCents = 2;
  const int32_t mHbar = 1;
  const uint64_t mSeconds = 100ULL;
  const Hbar mBalance = Hbar(100LL);
  const AccountId mAccountId = AccountId(0ULL, 0ULL, 10ULL);
  const AccountId mAccountIdFrom = AccountId(4ULL);
  const AccountId mAccountIdTo = AccountId(3ULL);
  const std::unique_ptr<ECDSAPrivateKey> mPrivateKey = ECDSAPrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> mPublicKeyFromPrivate = mPrivateKey->getPublicKey();
  const std::shared_ptr<ECDSAPublicKey> mPublicKeyFromString = ECDSAPublicKey::fromString(mPublicKeyFromPrivate->toString());
};

// Tests deserialization of Hedera::PublicKey object from a proto::Key object.
TEST_F(DeserializationTests, PublicKeyFromProtobufTest)
{
  // Given
  const std::shared_ptr<ECDSAPublicKey> testECDSAPublicKey = getTestECDSAPublicKey();
  const std::string testECDSAPublicKeyAsString = testECDSAPublicKey.get()->toString();
  const std::unique_ptr<proto::Key> testProtobufECDSAPublicKey = testECDSAPublicKey.get()->toProtobuf();

  // When
  std::shared_ptr<PublicKey> testPublicKey = PublicKey::fromProtobuf(*testProtobufECDSAPublicKey.get());
  
  // Then
  EXPECT_NE(testPublicKey.get(), nullptr);
  EXPECT_EQ(testECDSAPublicKeyAsString, testPublicKey.get()->toString());
}
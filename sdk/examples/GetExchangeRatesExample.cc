/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "FileContentsQuery.h"
#include "FileId.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"

#include <dotenv.h>
#include <iostream>

using namespace Hedera;

int main(int argc, char** argv)
{
  dotenv::init();
  const AccountId operatorAccountId = AccountId::fromString(std::getenv("OPERATOR_ID"));
  const std::shared_ptr<PrivateKey> operatorPrivateKey = ED25519PrivateKey::fromString(std::getenv("OPERATOR_KEY"));

  // Get a client for the Hedera testnet, and set the operator account ID and key such that all generated transactions
  // will be paid for by this account and be signed by this key.
  Client client = Client::forTestnet();
  client.setOperator(operatorAccountId, operatorPrivateKey);

  // Get the contents of the exchange rate file.
  FileContents fileContents = FileContentsQuery().setFileId(FileId::EXCHANGE_RATES).execute(client);

  // Convert the contents to an ExchangeRates object.
  const ExchangeRates exchangeRates = ExchangeRates::fromBytes(fileContents);

  std::cout << "Current numerator:       " << exchangeRates.mCurrentRate.mCents << std::endl;
  std::cout << "Current denominator:     " << exchangeRates.mCurrentRate.mHbars << std::endl;
  std::cout << "Current expiration time: " << exchangeRates.mCurrentRate.mExpirationTime.time_since_epoch().count()
            << std::endl;
  std::cout << "Current exchange rate:   " << exchangeRates.mCurrentRate.mExchangeRateInCents << std::endl << std::endl;

  std::cout << "Next numerator:       " << exchangeRates.mNextRate.mCents << std::endl;
  std::cout << "Next denominator:     " << exchangeRates.mNextRate.mHbars << std::endl;
  std::cout << "Next expiration time: " << exchangeRates.mNextRate.mExpirationTime.time_since_epoch().count()
            << std::endl;
  std::cout << "Next exchange rate:   " << exchangeRates.mNextRate.mExchangeRateInCents << std::endl;

  return 0;
}

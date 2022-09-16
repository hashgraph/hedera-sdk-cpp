/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#ifndef ACCOUNT_DELETE_TRANSACTION_H_
#define ACCOUNT_DELETE_TRANSACTION_H_

/**
 * Library includes
 */
#include "AccountId.h"
#include "Transaction.h"
#include "helper/InitType.h"

/**
 * STL includes
 */
#include <unordered_map>

/**
 * Hedera forward declarations
 */
namespace Hedera
{
class Client;
class TransactionId;
} // namespace Hedera

/**
 * Protobuf forward declarations
 */
namespace proto
{
class TransactionBody;
} // namespace proto

namespace Hedera
{
/**
 * Marks an account as deleted, moving all its current Hbars to another account.
 *
 * It will remain in the ledger, marked as deleted, until it expires.
 * Transfers into a deleted account fail. But a deleted account can still have
 * its expiration extended in the normal way.
 */
class AccountDeleteTransaction : public Transaction<AccountDeleteTransaction>
{
public:
  /**
   * Constructor.
   */
  AccountDeleteTransaction();

  /**
   * Construct from a map of TransactionId's to their corresponding
   * AccountId's and Transactions.
   *
   * @param transactions Compound list of transaction ID's list of (AccountId,
   *                     TransactionBody) records.
   */
  explicit AccountDeleteTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Constructor from a TransactionBody object.
   *
   * @param transaction The protobuf transaction body.
   */
  explicit AccountDeleteTransaction(const proto::TransactionBody& transaction);

  /**
   * Derived from Transaction. Validate the checksums of the account IDs.
   *
   * @param client The client with which to validate.
   */
  virtual void validateChecksums(const Client& client) const override;

  /**
   * Sets the account ID of the account to delete.
   *
   * @param accountId The account ID of the account to delete.
   * @return          Reference to this AccountDeleteTransaction object.
   */
  AccountDeleteTransaction& setAccountId(const AccountId& accountId);

  /**
   * Sets the account ID which will receive all remaining Hbars.
   *
   * @param accountId The account ID of the account to receive the Hbars.
   * @return          Reference to this AccountDeleteTransaction object.
   */
  AccountDeleteTransaction& setTransferAccountId(const AccountId& accountId);

  /**
   * Extract the account id of the account to delete.
   *
   * @return The account id of the account to delete.
   */
  inline InitType<AccountId> getAccountId() const { return mAccountId; }

  /**
   * Extract the receiving account id.
   *
   * @return The account ID of the account to receive the Hbars.
   */
  inline InitType<AccountId> getTransferAccountId() const
  {
    return mTransferAccountId;
  }

private:
  /**
   * Initialize from the transaction body.
   */
  void initFromTransactionBody();

  /**
   * The account ID of the account to delete.
   */
  InitType<AccountId> mAccountId;

  /**
   * The account ID of the account to receive the Hbars.
   *
   */
  InitType<AccountId> mTransferAccountId;
};

} // namespace Hedera

#endif // ACCOUNT_DELETE_TRANSACTION_H_
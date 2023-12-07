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
#ifndef HEDERA_SDK_CPP_TOKEN_UNLOCK_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_UNLOCK_TRANSACTION_H_

#include "AccountId.h"
#include "TokenId.h"
#include "Transaction.h"

namespace proto
{
class TokenUnlockTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Unlock a certain amount of tokens in an account. The TokenID must either refer to a FUNGIBLE_COMMON token type, or a
 * partition of such a token type.
 */
class TokenUnlockTransaction : public Transaction<TokenUnlockTransaction>
{
public:
  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a TokenUnlock transaction.
   */
  explicit TokenUnlockTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Construct from a map of TransactionIds to node account IDs and their respective Transaction protobuf objects.
   *
   * @param transactions The map of TransactionIds to node account IDs and their respective Transaction protobuf
   *                     objects.
   */
  explicit TokenUnlockTransaction(const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions);

  /**
   * Set the ID of the account which contains the tokens to unlock.
   *
   * @param accountId The ID of the account which contains the tokens to unlock.
   * @return A reference to this TokenUnlockTransaction with the newly-set account ID.
   */
  TokenUnlockTransaction& setAccountId(const AccountId& accountId);

  /**
   * Set the ID of the token or partition to unlock.
   *
   * @param tokenId The ID of the token or partition to unlock.
   * @return A reference to this TokenUnlockTransaction with the newly-set token ID.
   */
  TokenUnlockTransaction& setTokenId(const TokenId& tokenId);

  /**
   * Set the amount of the FUNGIBLE_COMMON token to unlock. This is mutually exclusive with mSerialNumber, and will
   * reset the value of the mSerialNumber if it is set.
   *
   * @param amount The amount of the FUNGIBLE_COMMON token to unlock.
   * @return A reference to this TokenUnlockTransaction with the newly-set amount.
   */
  TokenUnlockTransaction& setAmount(uint64_t amount);

  /**
   * Set the serial number of the NON_FUNGIBLE_UNIQUE token to unlock. This is mutually exclusive with mAmount, and will
   * reset the value of the mAmount if it is set.
   *
   * @param serialNumber The serial number of the NON_FUNGIBLE_UNIQUE token to unlock.
   * @return A reference to this TokenUnlockTransaction with the newly-set amount.
   */
  TokenUnlockTransaction& setSerialNumber(uint64_t serialNumber);

  /**
   * Get the ID of the account which contains the tokens to unlock.
   *
   * @return The ID of the account which contains the tokens to unlock.
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAccountId; }

  /**
   * Get the ID of the token or partition to unlock.
   *
   * @return The ID of the token or partition to unlock.
   */
  [[nodiscard]] inline TokenId getTokenId() const { return mTokenId; }

  /**
   * Get the amount of the FUNGIBLE_COMMON token to unlock.
   *
   * @return The amount of the FUNGIBLE_COMMON token to unlock. Returns uninitialized if a value has not been set, or if
   * a serial number has been set most recently.
   */
  [[nodiscard]] inline std::optional<uint64_t> getAmount() const { return mAmount; }

  /**
   * Get the serial number of the NON_FUNGIBLE_UNIQUE token to unlock.
   *
   * @return The serial number of the NON_FUNGIBLE_UNIQUE token to unlock. Returns uninitialized if a value has not been
   *         set, or if an amount has been set most recently.
   */
  [[nodiscard]] inline std::optional<uint64_t> getSerialNumber() const { return mSerialNumber; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Submit a Transaction protobuf object which contains this TokenUnlockTransaction's data to
   * a Node.
   *
   * @param request  The Transaction protobuf object to submit.
   * @param node     The Node to which to submit the request.
   * @param deadline The deadline for submitting the request.
   * @param response Pointer to the ProtoResponseType object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const proto::Transaction& request,
                                           const std::shared_ptr<internal::Node>& node,
                                           const std::chrono::system_clock::time_point& deadline,
                                           proto::TransactionResponse* response) const override;

  /**
   * Derived from Transaction. Verify that all the checksums in this TokenUnlockTransaction are valid.
   *
   * @param client The Client that should be used to validate the checksums.
   * @throws BadEntityException This TokenUnlockTransaction's checksums are not valid.
   */
  void validateChecksums(const Client& client) const override;

  /**
   * Derived from Transaction. Build and add the TokenUnlockTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Initialize this TokenUnlockTransaction from its source TransactionBody protobuf object.
   */
  void initFromSourceTransactionBody();

  /**
   * Build a TokenUnlockTransactionBody protobuf object from this TokenUnlockTransaction object.
   *
   * @return A pointer to a TokenUnlockTransactionBody protobuf object filled with this TokenUnlockTransaction object's
   *         data.
   */
  [[nodiscard]] proto::TokenUnlockTransactionBody* build() const;

  /**
   * The ID of the account which contains the tokens to unlock.
   */
  AccountId mAccountId;

  /**
   * The ID of the token or partition to unlock.
   */
  TokenId mTokenId;

  /**
   * The amount of the FUNGIBLE_COMMON token to unlock.
   */
  std::optional<uint64_t> mAmount;

  /**
   * The serial number of the NON_FUNGIBLE_UNIQUE token to unlock.
   */
  std::optional<uint64_t> mSerialNumber;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_UNLOCK_TRANSACTION_H_

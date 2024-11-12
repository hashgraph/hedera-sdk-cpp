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
#ifndef HIERO_SDK_CPP_TOKEN_REJECT_FLOW_H_
#define HIERO_SDK_CPP_TOKEN_REJECT_FLOW_H_

#include "AccountId.h"
#include "NftId.h"
#include "TokenId.h"
#include "TransactionResponse.h"

namespace Hiero
{
class Client;
class PrivateKey;
}

namespace Hiero
{
/**
 * Reject undesired token(s) and dissociate in a single flow.
 */
class TokenRejectFlow
{
public:
  /**
   * Execute the Transactions in this flow (TokenRejectTransaction and TokenDissociateTransaction).
   *
   * @param client The Client to use to submit these Transactions.
   * @return The TransactionResponse object of the flow.
   * @throws MaxAttemptsExceededException If this Executable attempts to execute past the number of allowable attempts.
   * @throws PrecheckStatusException      If this Executable fails its pre-check.
   * @throws UninitializedException       If the input Client has not yet been initialized.
   */
  TransactionResponse execute(const Client& client);

  /**
   * Execute the Transactions in this flow (TokenRejectTransaction and TokenDissociateTransaction) with timeout.
   *
   * @param client  The Client to use to submit these Transactions.
   * @param timeout The desired timeout for the execution of these Transactions.
   * @return The TransactionResponse object of the flow.
   * @throws MaxAttemptsExceededException If this Executable attempts to execute past the number of allowable attempts.
   * @throws PrecheckStatusException      If this Executable fails its pre-check.
   * @throws UninitializedException       If the input Client has not yet been initialized.
   */
  TransactionResponse execute(const Client& client, const std::chrono::system_clock::duration& timeout);

  /**
   * Freeze the TokenRejectTransaction with a Client. The Client's operator will be used to generate a transaction
   * ID, and the client's network will be used to generate a list of node account IDs.
   *
   * NOTE: Since Client's can't be copied, this TokenRejectFlow will store a pointer to the input Client. It is the
   * responsibility of the user to make sure that the Client does not go out of scope or get destroyed until this
   * TokenRejectFlow is done executing, otherwise this will crash upon execution.
   *
   * @param client The Client with which to freeze the TokenRejectTransaction.
   * @return A reference to this TokenRejectFlow object with the newly-set freeze Client.
   * @throws UninitializedException If Client operator has not been initialized.
   */
  TokenRejectFlow& freezeWith(const Client* client);

  /**
   * Set the PrivateKey with which the TokenRejectTransaction will be signed.
   *
   * @param key The PrivateKey with which to sign the TokenRejectTransaction.
   * @return A reference to this TokenRejectFlow object with the newly-set signing PrivateKey.
   */
  TokenRejectFlow& sign(const std::shared_ptr<PrivateKey>& key);

  /**
   * Set the PublicKey and signer function with which the TokenRejectTransaction will be signed.
   *
   * @param key    The PublicKey with which to sign the TokenRejectTransaction.
   * @param signer The callback function to use to sign the TokenRejectTransaction.
   * @return A reference to this TokenRejectFlow object with the newly-set public key and signer function.
   */
  TokenRejectFlow& signWith(const std::shared_ptr<PublicKey>& key,
                            const std::function<std::vector<std::byte>(const std::vector<std::byte>&)>& signer);

  /**
   * Set the Client operator with which the TokenRejectTransaction will be signed.
   *
   * @param client The Client operator to sign the TokenRejectTransaction.
   * @return A reference to this TokenRejectFlow object with the newly-set signing operator.
   * @throws UninitializedException If the Client operator has not yet been set.
   */
  TokenRejectFlow& signWithOperator(const Client& client);

  /**
   * Get the list of account IDs for nodes with which execution will be attempted.
   *
   * @return The list of account IDs of nodes this TokenRejectFlow would attempt request submission.
   */
  [[nodiscard]] inline std::vector<AccountId> getNodeAccountIds() const { return mNodeAccountIds; }

  /**
   * Get the account holding tokens to be rejected.
   *
   * @return Optional containing the account Id of the owner.
   */
  [[nodiscard]] std::optional<AccountId> getOwner() const { return mOwner; };

  /**
   * Get the list of fungible tokens to be rejected.
   *
   * @return A vector of TokenId objects.
   */
  [[nodiscard]] const std::vector<TokenId>& getFts() const { return mFts; };

  /**
   * Get the list of non-fungible tokens to be rejected.
   *
   * @return A vector of NftId objects.
   */
  [[nodiscard]] const std::vector<NftId>& getNfts() const { return mNfts; };

  /**
   * Set the desired account IDs of nodes to which this transaction will be submitted.
   *
   * @param nodeAccountIds The desired list of account IDs of nodes to submit this request.
   * @return A reference to this TokenRejectFlow object with the newly-set node account IDs.
   */
  TokenRejectFlow& setNodeAccountIds(const std::vector<AccountId>& nodeAccountIds);

  /**
   * Set a new account holding tokens to be rejected.
   *
   * @param owner Account Id of the account.
   * @return A reference to this TokenRejectFlow with the newly-set owner.
   */
  TokenRejectFlow& setOwner(const AccountId& owner);

  /**
   * Set a new fungible tokens list of tokens to be rejected.
   *
   * @param fts List of token ids.
   * @return A reference to this TokenRejectFlow with the newly-set tokens list.
   */
  TokenRejectFlow& setFts(const std::vector<TokenId>& fts);

  /**
   * Set a new non-fungible tokens list of tokens to be rejected.
   *
   * @param nfts List of nft ids.
   * @return A reference to this TokenRejectFlow with the newly-set nfts list.
   */
  TokenRejectFlow& setNfts(const std::vector<NftId>& nfts);

private:
  /**
   * The Client with which to freeze the TokenRejectTransaction.
   */
  const Client* mFreezeWithClient = nullptr;

  /**
   * The PrivateKey with which to sign the TokenRejectTransaction.
   */
  std::shared_ptr<PrivateKey> mPrivateKey = nullptr;

  /**
   * The PublicKey associated with the signer function to sign the TokenRejectTransaction.
   */
  std::shared_ptr<PublicKey> mPublicKey = nullptr;

  /**
   * The signer function to use to sign the TokenRejectTransaction.
   */
  std::optional<std::function<std::vector<std::byte>(const std::vector<std::byte>&)>> mSigner;

  /**
   * The list of account IDs of the nodes with which execution should be attempted.
   */
  std::vector<AccountId> mNodeAccountIds;

  /**
   * An account holding the tokens to be rejected.
   * If set, this account MUST sign this transaction.
   * If not set, the payer for this transaction SHALL be the account rejecting tokens.
   */
  std::optional<AccountId> mOwner;

  /**
   * On success each rejected token serial number or balance SHALL be transferred from
   * the requesting account to the treasury account for that token type.
   * After rejection the requesting account SHALL continue to be associated with the token.
   * if dissociation is desired then a separate TokenDissociate transaction MUST be submitted to remove the association.
   *
   * A list of one or more fungible token rejections.
   */
  std::vector<TokenId> mFts;

  /**
   * A list of one or more non-fungible token rejections.
   */
  std::vector<NftId> mNfts;
};

} // namespace Hiero

#endif // HIERO_SDK_CPP_TOKEN_REJECT_FLOW_H_

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
#include "TokenRejectFlow.h"
#include "AccountId.h"
#include "Client.h"
#include "NftId.h"
#include "TokenDissociateTransaction.h"
#include "TokenId.h"
#include "TokenRejectTransaction.h"
#include "TransactionReceipt.h"
#include "exceptions/UninitializedException.h"

namespace Hiero
{
//-----
TransactionResponse TokenRejectFlow::execute(const Client& client)
{
  return execute(client, client.getRequestTimeout());
}

//-----
TransactionResponse TokenRejectFlow::execute(const Client& client, const std::chrono::system_clock::duration& timeout)
{
  TokenRejectTransaction tokenRejectTransaction;

  if (mOwner.has_value())
  {
    tokenRejectTransaction.setOwner(mOwner.value());
  }

  tokenRejectTransaction.setFts(mFts).setNfts(mNfts);

  if (!mNodeAccountIds.empty())
  {
    tokenRejectTransaction.setNodeAccountIds(mNodeAccountIds);
  }

  if (mFreezeWithClient != nullptr)
  {
    tokenRejectTransaction.freezeWith(mFreezeWithClient);
  }

  if (mPrivateKey)
  {
    tokenRejectTransaction.sign(mPrivateKey);
  }
  else if (mPublicKey && mSigner.has_value())
  {
    tokenRejectTransaction.signWith(mPublicKey, mSigner.value());
  }

  // Submit the TokenRejectTransaction.
  TransactionResponse txResponse = tokenRejectTransaction.execute(client, timeout);

  // Make sure the transaction reaches consensus.
  TransactionReceipt txReceipt = txResponse.getReceipt(client, timeout);

  TokenDissociateTransaction tokenDissociateTransaction;

  if (mOwner.has_value())
  {
    tokenDissociateTransaction.setAccountId(mOwner.value());
  }

  // build dissociate vector from rejected nfts
  std::vector<TokenId> toDissociate;
  std::for_each(
    mNfts.cbegin(), mNfts.cend(), [&toDissociate](const NftId& nftId) { toDissociate.push_back(nftId.mTokenId); });

  // Make sure the transaction reaches consensus.
  txReceipt = tokenDissociateTransaction.setTokenIds(toDissociate).execute(client, timeout).getReceipt(client, timeout);

  return txResponse;
}

//-----
TokenRejectFlow& TokenRejectFlow::freezeWith(const Client* client)
{
  mFreezeWithClient = client;
  return *this;
}

//-----
TokenRejectFlow& TokenRejectFlow::sign(const std::shared_ptr<PrivateKey>& key)
{
  mPrivateKey = key;
  mPublicKey = nullptr;
  mSigner.reset();
  return *this;
}

//-----
TokenRejectFlow& TokenRejectFlow::signWith(
  const std::shared_ptr<PublicKey>& key,
  const std::function<std::vector<std::byte>(const std::vector<std::byte>&)>& signer)
{
  mPrivateKey = nullptr;
  mPublicKey = key;
  mSigner = signer;
  return *this;
}

//-----
TokenRejectFlow& TokenRejectFlow::signWithOperator(const Client& client)
{
  if (!client.getOperatorPublicKey())
  {
    throw UninitializedException("Client operator has not yet been set");
  }

  mPrivateKey = nullptr;
  mPublicKey = client.getOperatorPublicKey();
  mSigner = client.getOperatorSigner();
  return *this;
}

//-----
TokenRejectFlow& TokenRejectFlow::setNodeAccountIds(const std::vector<AccountId>& nodeAccountIds)
{
  mNodeAccountIds = nodeAccountIds;
  return *this;
}

//-----
TokenRejectFlow& TokenRejectFlow::setOwner(const AccountId& owner)
{
  mOwner = owner;
  return *this;
}

//-----
TokenRejectFlow& TokenRejectFlow::setFts(const std::vector<TokenId>& fts)
{
  mFts = fts;
  return *this;
}

//-----
TokenRejectFlow& TokenRejectFlow::setNfts(const std::vector<NftId>& nfts)
{
  mNfts = nfts;
  return *this;
}

} // namespace Hiero
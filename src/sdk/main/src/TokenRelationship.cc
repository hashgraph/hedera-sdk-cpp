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

#include "TokenRelationship.h"

#include <proto/crypto_get_info.pb.h>

using namespace Hedera;

//-----
// TokenRelationship::TokenRelationship()
//   : mTokenId(TokenId())
//   , mSymbol("")
//   , mBalance(0)
//   , mDecimals(0)
//   , mKycStatus()
//   , mFreezeStatus()
//   , mAutomaticAssociation(false)
// {
//   setKycStatus(0);
//   setFreezeStatus(0);
// }

//-----
TokenRelationship::TokenRelationship(const TokenId& tokenId,
                                     const std::string& symbol,
                                     uint64_t balance,
                                     uint32_t decimals,
                                     int kycStatus,
                                     int freezeStatus,
                                     bool automaticAssociation)
  : mTokenId(tokenId)
  , mSymbol(symbol)
  , mBalance(balance)
  , mDecimals(decimals)
  , mKycStatus()
  , mFreezeStatus()
  , mAutomaticAssociation(automaticAssociation)
{
  setKycStatus(kycStatus);
  setFreezeStatus(freezeStatus);
}

//-----
TokenRelationship TokenRelationship::fromProtobuf(const proto::TokenRelationship& proto)
{
  TokenRelationship tokenRelationship;
  if (proto.has_tokenid())
  {
    tokenRelationship.mTokenId = TokenId::fromProtobuf(proto.tokenid());
  }
  tokenRelationship.mSymbol = proto.symbol();
  tokenRelationship.mBalance = proto.balance();
  tokenRelationship.mDecimals = proto.decimals();
  tokenRelationship.setKycStatus(proto.kycstatus());
  tokenRelationship.setFreezeStatus(proto.freezestatus());
  tokenRelationship.mAutomaticAssociation = proto.automatic_association();

  return tokenRelationship;
}

//-----
std::unique_ptr<proto::TokenRelationship> TokenRelationship::toProtobuf() const
{
  auto proto = std::make_unique<proto::TokenRelationship>();
  proto->set_allocated_tokenid(mTokenId.toProtobuf().release());
  proto->set_symbol(mSymbol);
  proto->set_balance(mBalance);
  proto->set_decimals(mDecimals);
  proto->set_kycstatus(getKycStatus());
  proto->set_freezestatus(getFreezeStatus());
}

//-----
std::string TokenRelationship::toString() const
{
  return "TokenRelationship {"
         "\n  tokenId: " +
         mTokenId.toString() + "\n  symbol: " + mSymbol + "\n  balance: " + std::to_string(mBalance) +
         "\n  decimals: " + std::to_string(mDecimals) +
         "\n  kycStatus: " + (mKycStatus.has_value() ? std::to_string(mKycStatus.value()) : "null") +
         "\n  freezeStatus: " + (mFreezeStatus.has_value() ? std::to_string(mFreezeStatus.value()) : "null") +
         "\n  automaticAssociation: " + (mAutomaticAssociation ? "true" : "false") + "\n}";
}

//-----
void TokenRelationship::setKycStatus(int kycStatus)
{
  if (kycStatus == 1)
  {
    mKycStatus = true;
  }
  else if (kycStatus == 2)
  {
    mKycStatus = false;
  }
  else if (kycStatus == 0)
  {
    mKycStatus = std::nullopt;
  }
  else
  {
    throw std::invalid_argument("Invalid KYC status string");
  }
}

//-----
void TokenRelationship::setFreezeStatus(int freezeStatus)
{
  if (freezeStatus == 1)
  {
    mFreezeStatus = true;
  }
  else if (freezeStatus == 2)
  {
    mFreezeStatus = false;
  }
  else if (freezeStatus == 0)
  {
    mFreezeStatus = std::nullopt;
  }
  else
  {
    throw std::invalid_argument("Invalid freeze status string");
  }
}

//-----
proto::TokenKycStatus TokenRelationship::getKycStatus() const
{
  if (!mKycStatus.has_value())
    return proto::TokenKycStatus::KycNotApplicable;
  if (mKycStatus.value() == true)
    return proto::TokenKycStatus::Granted;
  else
    return proto::TokenKycStatus::Revoked;
}

//-----
proto::TokenFreezeStatus TokenRelationship::getFreezeStatus() const
{
  if (!mFreezeStatus.has_value())
    return proto::TokenFreezeStatus::FreezeNotApplicable;
  if (mFreezeStatus.value() == true)
    return proto::TokenFreezeStatus::Frozen;
  else
    return proto::TokenFreezeStatus::Unfrozen;
}
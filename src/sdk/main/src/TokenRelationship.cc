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

#include "TokenRelationship.h"

using namespace Hedera;

//-----
TokenRelationship::TokenRelationship(const TokenId& tokenId,
                                     const std::string& symbol,
                                     uint64_t balance,
                                     std::string_view kycStatus,
                                     std::string_view freezeStatus,
                                     bool automaticAssociation)
  : mTokenId(tokenId)
  , mSymbol(symbol)
  , mBalance(balance)
  , mKycStatus()
  , mFreezeStatus()
  , mAutomaticAssociation(automaticAssociation)
{
  setKycStatus(kycStatus);
  setFreezeStatus(freezeStatus);
}

//-----
std::string TokenRelationship::toString() const
{
  return "TokenRelationship {"
         "\n  tokenId: " +
         mTokenId.toString() + "\n  symbol: " + mSymbol + "\n  balance: " + std::to_string(mBalance) +
         "\n  kycStatus: " + (mKycStatus.has_value() ? std::to_string(mKycStatus.value()) : "null") +
         "\n  freezeStatus: " + (mFreezeStatus.has_value() ? std::to_string(mFreezeStatus.value()) : "null") +
         "\n  automaticAssociation: " + (mAutomaticAssociation ? "true" : "false") + "\n}";
}

//-----
void TokenRelationship::setKycStatus(std::string_view kycStatusString)
{
  if (kycStatusString == "GRANTED")
  {
    mKycStatus = true;
  }
  else if (kycStatusString == "REVOKED")
  {
    mKycStatus = false;
  }
  else if (kycStatusString == "NOT_APPLICABLE")
  {
    mKycStatus = std::nullopt;
  }
  else
  {
    throw std::invalid_argument("Invalid KYC status string");
  }
}

//-----
void TokenRelationship::setFreezeStatus(std::string_view freezeStatus)
{
  if (freezeStatus == "FROZEN")
  {
    mFreezeStatus = true;
  }
  else if (freezeStatus == "UNFROZEN")
  {
    mFreezeStatus = false;
  }
  else if (freezeStatus == "NOT_APPLICABLE")
  {
    mFreezeStatus = std::nullopt;
  }
  else
  {
    throw std::invalid_argument("Invalid freeze status string");
  }
}
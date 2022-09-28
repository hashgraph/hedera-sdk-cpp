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
#ifndef NFT_ALLOWANCE_H_
#define NFT_ALLOWANCE_H_

#include <vector>

namespace proto
{
class NftAllowance;
}

namespace Hedera
{
template<typename T>
class InitType;

class AccountId;
class Client;
class TokenId;
}

namespace Hedera
{
/**
 * Class to encapsulate the NFT methods for token allowances.
 */
class NftAllowance
{
public:
  NftAllowance();

  NftAllowance(const InitType<TokenId>& tokenId,
               const InitType<AccountId>& ownerAccountId,
               const InitType<AccountId>& spenderAccountId,
               const std::vector<int64_t>& serialNumbers,
               bool allSerials)
  {
    (void)tokenId;
    (void)ownerAccountId;
    (void)spenderAccountId;
    (void)serialNumbers;
    (void)allSerials;
  }

  static NftAllowance fromProtobuf(const proto::NftAllowance& proto)
  {
    (void)proto;
    return NftAllowance();
  }

  void validateChecksums(const Client& client) const { (void)client; }

  std::vector<int64_t> mSerialNumbers;
  bool allSerials;
};

} // namespace Hedera

#endif // NFT_ALLOWANCE_H_
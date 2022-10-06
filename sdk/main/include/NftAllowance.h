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

/**
 * STL includes
 */
#include <vector>

/**
 * Protobuf forward declarations
 */
namespace proto
{
class NftAllowance;
} // namespace proto

namespace Hedera
{
template<typename T>
class InitType;

class AccountId;
class TokenId;
} // namespace Hedera

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
               const std::vector<long long>& serialNumbers,
               bool allSerials)
  {
  }

  static NftAllowance fromProtobuf(const ::proto::NftAllowance& proto)
  {
    return NftAllowance();
  }

  std::vector<long long> mSerialNumbers;
  bool allSerials;
};

} // namespace Hedera

#endif // NFT_ALLOWANCE_H_
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
#ifndef TOKEN_ALLOWANCE_H_
#define TOKEN_ALLOWANCE_H_

/**
 * Hedera forward declarations
 */
namespace Hedera
{
class AccountId;
class TokenId;
} // namespace Hedera

namespace Hedera
{
/**
 * An approved allowance of token transfers for a spender.
 */
class TokenAllowance
{
public:
  TokenAllowance() {}

  TokenAllowance(const TokenId& tokenId,
                 const InitType<AccountId>& ownerAccountId,
                 const AccountId& spenderAccountId,
                 const long long& amount)
  {
  }

  static TokenAllowance fromProtobuf(const ::proto::TokenAllowance& proto)
  {
    return TokenAllowance();
  }
};

} // namespace Hedera

#endif // TOKEN_ALLOWANCE_H_
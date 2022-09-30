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

namespace Hedera
{
class AccountId;
class Client;
class TokenId;
}

namespace proto
{
class TokenAllowance;
}

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
                 const int64_t& amount)
  {
    (void)tokenId;
    (void)ownerAccountId;
    (void)spenderAccountId;
    (void)amount;
  }

  static TokenAllowance fromProtobuf(const proto::TokenAllowance& proto)
  {
    (void)proto;
    return TokenAllowance();
  }

  void validateChecksums(const Client& client) const { (void)client; }
};

} // namespace Hedera

#endif // TOKEN_ALLOWANCE_H_
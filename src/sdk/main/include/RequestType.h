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
#ifndef HEDERA_SDK_CPP_REQUEST_TYPE_H_
#define HEDERA_SDK_CPP_REQUEST_TYPE_H_

#include <string>
#include <unordered_map>

namespace proto
{
enum HederaFunctionality : int;
}

namespace Hedera
{
/**
 * Enumeration of all queries and transactions supported by Hedera Hashgraph.
 */
enum class RequestType
{
  NONE,
  CRYPTO_TRANSFER,
  CRYPTO_UPDATE,
  CRYPTO_DELETE,
  CRYPTO_ADD_LIVE_HASH,
  CRYPTO_DELETE_LIVE_HASH,
  CONTRACT_CALL,
  CONTRACT_CREATE,
  CONTRACT_UPDATE,
  FILE_CREATE,
  FILE_APPEND,
  FILE_UPDATE,
  FILE_DELETE,
  CRYPTO_GET_ACCOUNT_BALANCE,
  CRYPTO_GET_ACCOUNT_RECORDS,
  CRYPTO_GET_INFO,
  CONTRACT_CALL_LOCAL,
  CONTRACT_GET_INFO,
  CONTRACT_GET_BYTECODE,
  GET_BY_SOLIDITY_ID,
  GET_BY_KEY,
  CRYPTO_GET_LIVE_HASH,
  CRYPTO_GET_STAKERS,
  FILE_GET_CONTENTS,
  FILE_GET_INFO,
  TRANSACTION_GET_RECORD,
  CONTRACT_GET_RECORDS,
  CRYPTO_CREATE,
  SYSTEM_DELETE,
  SYSTEM_UNDELETE,
  CONTRACT_DELETE,
  FREEZE,
  CREATE_TRANSACTION_RECORD,
  CRYPTO_ACCOUNT_AUTO_RENEW,
  CONTRACT_AUTO_RENEW,
  GET_VERSION_INFO,
  TRANSACTION_GET_RECEIPT,
  CONSENSUS_CREATE_TOPIC,
  CONSENSUS_UPDATE_TOPIC,
  CONSENSUS_DELETE_TOPIC,
  CONSENSUS_GET_TOPIC_INFO,
  CONSENSUS_SUBMIT_MESSAGE,
  UNCHECKED_SUBMIT,
  TOKEN_CREATE,
  TOKEN_GET_INFO,
  TOKEN_FREEZE_ACCOUNT,
  TOKEN_UNFREEZE_ACCOUNT,
  TOKEN_GRANT_KYC_TO_ACCOUNT,
  TOKEN_REVOKE_KYC_FROM_ACCOUNT,
  TOKEN_DELETE,
  TOKEN_UPDATE,
  TOKEN_MINT,
  TOKEN_BURN,
  TOKEN_ACCOUNT_WIPE,
  TOKEN_ASSOCIATE_TO_ACCOUNT,
  TOKEN_DISSOCIATE_FROM_ACCOUNT,
  SCHEDULE_CREATE,
  SCHEDULE_DELETE,
  SCHEDULE_SIGN,
  SCHEDULE_GET_INFO,
  TOKEN_GET_ACCOUNT_NFT_INFOS,
  TOKEN_GET_NFT_INFO,
  TOKEN_GET_NFT_INFOS,
  TOKEN_FEE_SCHEDULE_UPDATE,
  NETWORK_GET_EXECUTION_TIME,
  TOKEN_PAUSE,
  TOKEN_UNPAUSE,
  CRYPTO_APPROVE_ALLOWANCE,
  CRYPTO_DELETE_ALLOWANCE,
  GET_ACCOUNT_DETAILS,
  ETHEREUM_TRANSACTION,
  NODE_STAKE_UPDATE,
  UTIL_PRNG,
  TOKEN_UPDATE_NFTS
};

/**
 * Map of protobuf HederaFunctionality to their corresponding RequestTypes.
 */
extern const std::unordered_map<proto::HederaFunctionality, RequestType> gProtobufHederaFunctionalityToRequestType;

/**
 * Map of RequestTypes to their corresponding protobuf HederaFunctionality.
 */
extern const std::unordered_map<RequestType, proto::HederaFunctionality> gRequestTypeToProtobufHederaFunctionality;

/**
 * Map of RequestTypes to its corresponding string.
 */
[[maybe_unused]] extern const std::unordered_map<RequestType, std::string> gRequestTypeToString;

} // namespace Hedera

#endif // HEDERA_SDK_CPP_REQUEST_TYPE_H_

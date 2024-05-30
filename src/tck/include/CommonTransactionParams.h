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
#ifndef HEDERA_TCK_CPP_COMMON_TRANSACTION_PARAMS_H_
#define HEDERA_TCK_CPP_COMMON_TRANSACTION_PARAMS_H_

#include "Client.h"
#include "Hbar.h"
#include "JsonRpcException.h"
#include "PrivateKey.h"
#include "Transaction.h"
#include "TransactionId.h"

#include <chrono>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

namespace Hedera::TCK
{
/**
 * Struct the contains the common parameters of a Transaction.
 */
struct CommonTransactionParams
{
  /**
   * Function that fills a Transaction with the parameters present in this CommonTransactionParams.
   *
   * @tparam T The type of Transaction being filled out.
   * @param transaction The Transaction to fill out.
   * @param client      The Client being used to submit this Transaction.
   */
  template<typename T>
  void fillOutTransaction(Transaction<T>& transaction, const Client& client) const
  {
    if (mTransactionId.has_value())
    {
      transaction.setTransactionId(mTransactionId.value());
    }

    if (mMaxTransactionFee.has_value())
    {
      transaction.setMaxTransactionFee(Hbar::fromTinybars(mMaxTransactionFee.value()));
    }

    if (mValidTransactionDuration.has_value())
    {
      transaction.setValidTransactionDuration(mValidTransactionDuration.value());
    }

    if (mMemo.has_value())
    {
      transaction.setTransactionMemo(mMemo.value());
    }

    if (mRegenerateTransactionId.has_value())
    {
      transaction.setRegenerateTransactionIdPolicy(mRegenerateTransactionId.value());
    }

    if (mSigners.has_value())
    {
      transaction.freezeWith(&client);
      std::for_each(mSigners->cbegin(),
                    mSigners->cend(),
                    [&transaction](const std::shared_ptr<PrivateKey>& key) { transaction.sign(key); });
    }
  }

  /**
   * The ID of the transaction.
   */
  std::optional<TransactionId> mTransactionId;

  /**
   * The maximum amount willing to be paid to execute the transaction.
   */
  std::optional<int64_t> mMaxTransactionFee;

  /**
   * The length of time for which the transaction is valid.
   */
  std::optional<std::chrono::seconds> mValidTransactionDuration;

  /**
   * The memo of the transaction.
   */
  std::optional<std::string> mMemo;

  /**
   * If expired, should the transaction regenerate it's transaction ID?
   */
  std::optional<bool> mRegenerateTransactionId;

  /**
   * The list of DER-encoded hex strings of private keys to sign the transaction.
   */
  std::optional<std::vector<std::shared_ptr<PrivateKey>>> mSigners;
};

} // namespace Hedera::TCK

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert CommonTransactionParams arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<Hedera::TCK::CommonTransactionParams>
{
  /**
   * Convert a CommonTransactionParams to a JSON object.
   *
   * @param jsonTo   The JSON object to fill with the CommonTransactionParams.
   * @param txParams The CommonTransactionParams with which to fill the JSON object.
   */
  static void to_json(json& jsonTo, const Hedera::TCK::CommonTransactionParams& txParams)
  {
    if (txParams.mTransactionId.has_value())
    {
      jsonTo["transactionId"] = txParams.mTransactionId->toString();
    }

    if (txParams.mMaxTransactionFee.has_value())
    {
      jsonTo["maxTransactionFee"] = txParams.mMaxTransactionFee.value();
    }

    if (txParams.mValidTransactionDuration.has_value())
    {
      jsonTo["validTransactionDuration"] = txParams.mValidTransactionDuration->count();
    }

    if (txParams.mMemo.has_value())
    {
      jsonTo["memo"] = txParams.mMemo.value();
    }

    if (txParams.mRegenerateTransactionId.has_value())
    {
      jsonTo["regenerateTransactionId"] = txParams.mRegenerateTransactionId.value();
    }

    if (txParams.mSigners.has_value())
    {
      std::for_each(txParams.mSigners->cbegin(),
                    txParams.mSigners->cend(),
                    [&jsonTo](const std::shared_ptr<Hedera::PrivateKey>& key)
                    { jsonTo["signers"].push_back(key->toStringDer()); });
    }
  }

  /**
   * Convert a JSON object to a CommonTransactionParams.
   *
   * @param jsonFrom The JSON object with which to fill the CommonTransactionParams.
   * @param txParams The CommonTransactionParams to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, Hedera::TCK::CommonTransactionParams& txParams)
  {
    if (jsonFrom.contains("transactionId"))
    {
      if (!jsonFrom["transactionId"].is_string())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: transactionId should be a string");
      }

      txParams.mTransactionId = Hedera::TransactionId::fromString(jsonFrom["transactionId"].get<std::string>());
    }

    if (jsonFrom.contains("maxTransactionFee"))
    {
      if (!jsonFrom["maxTransactionFee"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: maxTransactionFee should be an integer");
      }

      txParams.mMaxTransactionFee = jsonFrom["maxTransactionFee"];
    }

    if (jsonFrom.contains("validTransactionDuration"))
    {
      if (!jsonFrom["validTransactionDuration"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: validTransactionDuration should be an integer");
      }

      txParams.mValidTransactionDuration = std::chrono::seconds(jsonFrom["validTransactionDuration"]);
    }

    if (jsonFrom.contains("memo"))
    {
      if (!jsonFrom["memo"].is_string())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: memo should be a string");
      }

      txParams.mMemo = jsonFrom["memo"];
    }

    if (jsonFrom.contains("regenerateTransactionId"))
    {
      if (!jsonFrom["regenerateTransactionId"].is_boolean())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: regenerateTransactionId should be a boolean");
      }

      txParams.mRegenerateTransactionId = jsonFrom["regenerateTransactionId"];
    }

    if (jsonFrom.contains("signers"))
    {
      if (!jsonFrom["signers"].is_array())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: signers should be a list");
      }

      txParams.mSigners = std::vector<std::shared_ptr<Hedera::PrivateKey>>();
      txParams.mSigners->reserve(jsonFrom["signers"].size());
      std::for_each(jsonFrom["signers"].cbegin(),
                    jsonFrom["signers"].cend(),
                    [&txParams](const json& jsonKey)
                    {
                      if (!jsonKey.is_string())
                      {
                        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                                            "invalid parameters: key in signers list is not a string");
                      }

                      txParams.mSigners->push_back(Hedera::PrivateKey::fromStringDer(jsonKey.get<std::string>()));
                    });
    }
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_COMMON_TRANSACTION_PARAMS_H_

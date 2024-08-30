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
#ifndef HEDERA_TCK_CPP_CUSTOM_FEE_SERIALIZER_H_
#define HEDERA_TCK_CPP_CUSTOM_FEE_SERIALIZER_H_

#include "AccountId.h"
#include "CustomFee.h"
#include "CustomFeeBase.h"
#include "CustomFixedFee.h"
#include "CustomFractionalFee.h"
#include "CustomRoyaltyFee.h"
#include "JsonRpcException.h"

#include <nlohmann/json.hpp>

namespace nlohmann
{
/**
 * JSON serializer template specialization required to convert CustomFee arguments properly.
 */
template<>
struct [[maybe_unused]] adl_serializer<std::shared_ptr<Hedera::CustomFee>>
{
  /**
   * Convert a CustomFee to a JSON object.
   *
   * @param jsonTo The JSON object to fill with the CustomFee.
   * @param fee    The CustomFee with which to fill the JSON object.
   */
  static void to_json(json& jsonTo, const std::shared_ptr<Hedera::CustomFee>& fee)
  {
    jsonTo["feeCollectorAccountId"] = fee->getFeeCollectorAccountId().toString();
    jsonTo["feeCollectorsExempt"] = fee->getAllCollectorsAreExempt();

    if (const std::shared_ptr<Hedera::CustomFixedFee> fixedFee = std::dynamic_pointer_cast<Hedera::CustomFixedFee>(fee);
        fixedFee)
    {
      jsonTo["fixedFee"] = {
        {"amount", fixedFee->getAmount()}
      };
      if (fixedFee->getDenominatingTokenId().has_value())
      {
        jsonTo["fixedFee"]["denominatingTokenId"] = fixedFee->getDenominatingTokenId()->toString();
      }
    }

    else if (const std::shared_ptr<Hedera::CustomFractionalFee> fractionalFee =
               std::dynamic_pointer_cast<Hedera::CustomFractionalFee>(fee);
             fractionalFee)
    {
      std::string assessmentMethod = Hedera::gFeeAssessmentMethodToString.at(fractionalFee->getAssessmentMethod());
      std::transform(assessmentMethod.begin(),
                     assessmentMethod.end(),
                     assessmentMethod.begin(),
                     [](unsigned char c) { return std::tolower(c); });
      jsonTo["fractionalFee"] = {
        {"numerator",         fractionalFee->getNumerator()    },
        { "denominator",      fractionalFee->getDenominator()  },
        { "minimumAmount",    fractionalFee->getMinimumAmount()},
        { "maximumAmount",    fractionalFee->getMaximumAmount()},
        { "assessmentMethod", assessmentMethod                 }
      };
    }

    else
    {
      const std::shared_ptr<Hedera::CustomRoyaltyFee> royaltyFee =
        std::dynamic_pointer_cast<Hedera::CustomRoyaltyFee>(fee);

      jsonTo["royaltyFee"] = {
        {"numerator",    royaltyFee->getNumerator()  },
        { "denominator", royaltyFee->getDenominator()}
      };

      if (royaltyFee->getFallbackFee().has_value())
      {
        jsonTo["royaltyFee"]["fallbackFee"] = {
          {"amount", royaltyFee->getFallbackFee()->getAmount()}
        };
        if (royaltyFee->getFallbackFee()->getDenominatingTokenId().has_value())
        {
          jsonTo["royaltyFee"]["fallbackFee"]["denominatingTokenId"] =
            royaltyFee->getFallbackFee()->getDenominatingTokenId()->toString();
        }
      }
    }
  }

  /**
   * Convert a JSON object to a CustomFee.
   *
   * @param jsonFrom The JSON object with which to fill the CustomFee.
   * @param fee      The CustomFee to fill with the JSON object.
   */
  static void from_json(const json& jsonFrom, std::shared_ptr<Hedera::CustomFee>& fee)
  {
    if (!jsonFrom.contains("feeCollectorAccountId"))
    {
      throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                          "invalid parameters: feeCollectorAccountId is REQUIRED.");
    }

    if (!jsonFrom["feeCollectorAccountId"].is_string())
    {
      throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                          "invalid parameters: feeCollectorAccountId MUST be a string.");
    }

    const Hedera::AccountId feeCollectorAccountId =
      Hedera::AccountId::fromString(jsonFrom["feeCollectorAccountId"].get<std::string>());

    if (!jsonFrom.contains("feeCollectorsExempt"))
    {
      throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                          "invalid parameters: feeCollectorsExempt is REQUIRED.");
    }

    if (!jsonFrom["feeCollectorsExempt"].is_boolean())
    {
      throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                          "invalid parameters: feeCollectorsExempt MUST be a boolean.");
    }

    const bool feeCollectorsExempt = jsonFrom["feeCollectorsExempt"].get<bool>();

    if (jsonFrom.contains("fixedFee"))
    {
      auto fixedFee = std::make_shared<Hedera::CustomFixedFee>();
      fixedFee->setFeeCollectorAccountId(feeCollectorAccountId);
      fixedFee->setAllCollectorsAreExempt(feeCollectorsExempt);

      if (!jsonFrom["fixedFee"].contains("amount"))
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: amount is REQUIRED for fixedFee fee types.");
      }

      if (!jsonFrom["fixedFee"]["amount"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: amount MUST be an int64.");
      }

      fixedFee->setAmount(jsonFrom["fixedFee"]["amount"].get<int64_t>());

      if (jsonFrom["fixedFee"].contains("denominatingTokenId"))
      {
        if (!jsonFrom["fixedFee"]["denominatingTokenId"].is_string())
        {
          throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                              "invalid parameters: denominatingTokenId MUST be a string.");
        }

        fixedFee->setDenominatingTokenId(
          Hedera::TokenId::fromString(jsonFrom["fixedFee"]["denominatingTokenId"].get<std::string>()));
      }

      fee = fixedFee;
    }

    else if (jsonFrom.contains("fractionalFee"))
    {
      auto fractionalFee = std::make_shared<Hedera::CustomFractionalFee>();
      fractionalFee->setFeeCollectorAccountId(feeCollectorAccountId);
      fractionalFee->setAllCollectorsAreExempt(feeCollectorsExempt);

      if (!jsonFrom["fractionalFee"].contains("numerator"))
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: numerator is REQUIRED for fractionalFee fee types.");
      }

      if (!jsonFrom["fractionalFee"]["numerator"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: numerator MUST be an int64.");
      }

      fractionalFee->setNumerator(jsonFrom["fractionalFee"]["numerator"].get<int64_t>());

      if (!jsonFrom["fractionalFee"].contains("denominator"))
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: denominator is REQUIRED for fractionalFee fee types.");
      }

      if (!jsonFrom["fractionalFee"]["denominator"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: denominator MUST be an int64.");
      }

      fractionalFee->setDenominator(jsonFrom["fractionalFee"]["denominator"].get<int64_t>());

      if (!jsonFrom["fractionalFee"].contains("minimumAmount"))
      {
        throw Hedera::TCK::JsonRpcException(
          Hedera::TCK::JsonErrorType::INVALID_PARAMS,
          "invalid parameters: minimumAmount is REQUIRED for fractionalFee fee types.");
      }

      if (!jsonFrom["fractionalFee"]["minimumAmount"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: minimumAmount MUST be an int64.");
      }

      fractionalFee->setMinimumAmount(jsonFrom["fractionalFee"]["minimumAmount"].get<int64_t>());

      if (!jsonFrom["fractionalFee"].contains("maximumAmount"))
      {
        throw Hedera::TCK::JsonRpcException(
          Hedera::TCK::JsonErrorType::INVALID_PARAMS,
          "invalid parameters: maximumAmount is REQUIRED for fractionalFee fee types.");
      }

      if (!jsonFrom["fractionalFee"]["maximumAmount"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: maximumAmount MUST be an int64.");
      }

      fractionalFee->setMaximumAmount(jsonFrom["fractionalFee"]["maximumAmount"].get<int64_t>());

      if (!jsonFrom["fractionalFee"].contains("assessmentMethod"))
      {
        throw Hedera::TCK::JsonRpcException(
          Hedera::TCK::JsonErrorType::INVALID_PARAMS,
          "invalid parameters: assessmentMethod is REQUIRED for fractionalFee fee types.");
      }

      if (!jsonFrom["fractionalFee"]["assessmentMethod"].is_string())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: assessmentMethod MUST be a string.");
      }

      std::string assessmentMethod = jsonFrom["fractionalFee"]["assessmentMethod"].get<std::string>();
      std::transform(assessmentMethod.begin(),
                     assessmentMethod.end(),
                     assessmentMethod.begin(),
                     [](unsigned char c) { return std::toupper(c); });
      fractionalFee->setAssessmentMethod(Hedera::gStringToFeeAssessmentMethod.at(assessmentMethod));

      fee = fractionalFee;
    }

    else if (jsonFrom.contains("royaltyFee"))
    {
      auto royaltyFee = std::make_shared<Hedera::CustomRoyaltyFee>();
      royaltyFee->setFeeCollectorAccountId(feeCollectorAccountId);
      royaltyFee->setAllCollectorsAreExempt(feeCollectorsExempt);

      if (!jsonFrom["royaltyFee"].contains("numerator"))
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: numerator is REQUIRED for royaltyFee fee types.");
      }

      if (!jsonFrom["royaltyFee"]["numerator"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: numerator MUST be an int64.");
      }

      royaltyFee->setNumerator(jsonFrom["royaltyFee"]["numerator"].get<int64_t>());

      if (!jsonFrom["royaltyFee"].contains("denominator"))
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: denominator is REQUIRED for royaltyFee fee types.");
      }

      if (!jsonFrom["royaltyFee"]["denominator"].is_number_integer())
      {
        throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                            "invalid parameters: denominator MUST be an int64.");
      }

      royaltyFee->setDenominator(jsonFrom["royaltyFee"]["denominator"].get<int64_t>());

      if (jsonFrom.contains("fallbackFee"))
      {
        Hedera::CustomFixedFee fallbackFee;

        if (!jsonFrom["royaltyFee"]["fallbackFee"].contains("amount"))
        {
          throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                              "invalid parameters: amount is REQUIRED for a fallback fee.");
        }

        if (!jsonFrom["royaltyFee"]["fallbackFee"]["amount"].is_number_integer())
        {
          throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                              "invalid parameters: amount MUST be an int64.");
        }

        fallbackFee.setAmount(jsonFrom["royaltyFee"]["fallbackFee"]["amount"].get<int64_t>());

        if (jsonFrom["royaltyFee"]["fallbackFee"].contains("denominatingTokenId"))
        {
          if (!jsonFrom["royaltyFee"]["fallbackFee"]["denominatingTokenId"].is_number_integer())
          {
            throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                                "invalid parameters: denominatingTokenId MUST be a string.");
          }

          fallbackFee.setDenominatingTokenId(Hedera::TokenId::fromString(
            jsonFrom["royaltyFee"]["fallbackFee"]["denominatingTokenId"].get<std::string>()));
        }

        royaltyFee->setFallbackFee(fallbackFee);
      }

      fee = royaltyFee;
    }

    else
    {
      throw Hedera::TCK::JsonRpcException(Hedera::TCK::JsonErrorType::INVALID_PARAMS,
                                          "invalid parameters: fee MUST contain one of fixedFee, fractionalFee, or "
                                          "royaltyFee.");
    }
  }
};

} // namespace nlohmann

#endif // HEDERA_TCK_CPP_CUSTOM_FEE_PARAMS_H_

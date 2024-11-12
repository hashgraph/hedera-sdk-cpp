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
#ifndef HIERO_SDK_CPP_MAX_QUERY_PAYMENT_EXCEEDED_EXCEPTION_H_
#define HIERO_SDK_CPP_MAX_QUERY_PAYMENT_EXCEEDED_EXCEPTION_H_

#include <exception>
#include <string_view>

namespace Hiero
{
/**
 * Exception that is thrown when the cost to execute a Query is larger than the maximum allowed amount.
 */
class MaxQueryPaymentExceededException : public std::exception
{
public:
  /**
   * Construct with a message.
   *
   * @param msg The error message to further describe this exception.
   */
  explicit MaxQueryPaymentExceededException(std::string_view msg)
    : mError(msg)
  {
  }

  /**
   * Get the descriptor message for this error.
   *
   * @return The descriptor message for this error.
   */
  [[nodiscard]] const char* what() const noexcept override { return mError.data(); };

private:
  /**
   * Descriptive error message.
   */
  std::string_view mError;
};

} // namespace Hiero

#endif // HIERO_SDK_CPP_MAX_QUERY_PAYMENT_EXCEEDED_EXCEPTION_H_

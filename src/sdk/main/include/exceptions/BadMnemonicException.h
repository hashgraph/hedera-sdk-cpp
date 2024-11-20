// SPDX-License-Identifier: Apache-2.0
#ifndef HIERO_SDK_CPP_BAD_MNEMONIC_EXCEPTION_H_
#define HIERO_SDK_CPP_BAD_MNEMONIC_EXCEPTION_H_

#include <exception>
#include <string_view>

namespace Hiero
{
/**
 * Exception that is thrown when a key could not be realized from a given input.
 */
class BadMnemonicException : public std::exception
{
public:
  /**
   * Construct with a message.
   *
   * @param msg The error message to further describe this exception.
   */
  explicit BadMnemonicException(std::string_view msg)
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

#endif // HIERO_SDK_CPP_BAD_MNEMONIC_EXCEPTION_H_

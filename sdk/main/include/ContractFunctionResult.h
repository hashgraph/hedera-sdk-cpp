/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_CONTRACT_FUNCTION_RESULT_H_
#define HEDERA_SDK_CPP_CONTRACT_FUNCTION_RESULT_H_

#include "AccountId.h"
#include "ContractId.h"
#include "ContractLogInfo.h"
#include "EvmAddress.h"
#include "Hbar.h"

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace proto
{
class ContractFunctionResult;
}

namespace Hedera
{
/**
 * Class to represent the result of invoking a contract via ContractCallQuery, ContractExecuteTransaction, or the result
 * of a contract constructor being called by ContractCreateTransaction.
 */
class ContractFunctionResult
{
public:
  /**
   * The error prefix representing an error in contract execution.
   */
  static inline const std::vector<std::byte> ERROR_PREFIX = { std::byte(8),
                                                              std::byte(-61),
                                                              std::byte(121),
                                                              std::byte(-96) };

  /**
   * Construct a ContractFunctionResult object from a ContractFunctionResult protobuf object.
   *
   * @param proto The ContractFunctionResult protobuf object from which to construct a ContractFunctionResult object.
   * @return The constructed ContractFunctionResult object.
   */
  [[nodiscard]] static ContractFunctionResult fromProtobuf(const proto::ContractFunctionResult& proto);

  /**
   * Get the value at the input index as a string.
   *
   * @param index The index at which to retrieve the string.
   * @return The value at the input index as a string.
   */
  [[nodiscard]] std::string getString(int index) const;

  /**
   * Get the value at the input index as a string array.
   *
   * @param index The index at which to retrieve the string array.
   * @return The value at the input index as a string array.
   */
  [[nodiscard]] std::vector<std::string> getStringArray(int index) const;

  /**
   * Get the value at the input index as a byte array.
   *
   * @param index The index at which to retrieve the byte array.
   * @return The value at the input index as a byte array.
   */
  [[nodiscard]] std::vector<std::byte> getByteArray(int index) const;

  /**
   * Get the value at the input index as a Solidity 32-byte value.
   *
   * @param index The index at which to retrieve the 32-byte value.
   * @return The value at the input index as a 32-byte value.
   */
  [[nodiscard]] std::vector<std::byte> getBytes32(int index) const;

  /**
   * Get the value at the input index as a boolean.
   *
   * @param index The index at which to retrieve the boolean value.
   * @return The value at the input index as a bool value.
   */
  [[nodiscard]] bool getBool(int index) const;

  /**
   * Get the value at the input index as a signed 8-bit integer. If the actual value is wider, it will be truncated to
   * the last byte.
   *
   * @param index The index at which to retrieve the signed 8-bit integer value.
   * @return The value at the input index as a signed 8-bit integer value.
   */
  [[nodiscard]] int8_t getInt8(int index) const;

  /**
   * Get the value at the input index as a signed 32-bit integer. If the actual value is wider, it will be truncated to
   * the last 4 bytes.
   *
   * @param index The index at which to retrieve the signed 32-bit integer value.
   * @return The value at the input index as a signed 32-bit integer value.
   */
  [[nodiscard]] int32_t getInt32(int index) const;

  /**
   * Get the value at the input index as a signed 64-bit integer. If the actual value is wider, it will be truncated to
   * the last 8 bytes.
   *
   * @param index The index at which to retrieve the signed 64-bit integer value.
   * @return The value at the input index as a signed 64-bit integer value.
   */
  [[nodiscard]] int64_t getInt64(int index) const;

  /**
   * Get the value at the input index as an unsigned 8-bit integer. If the actual value is wider, it will be truncated
   * to the last byte.
   *
   * @param index The index at which to retrieve the unsigned 8-bit integer value.
   * @return The value at the input index as an unsigned 8-bit integer value.
   */
  [[nodiscard]] uint8_t getUint8(int index) const;

  /**
   * Get the value at the input index as an unsigned 32-bit integer. If the actual value is wider, it will be truncated
   * to the last 4 bytes.
   *
   * @param index The index at which to retrieve the unsigned 32-bit integer value.
   * @return The value at the input index as an unsigned 32-bit integer value.
   */
  [[nodiscard]] uint32_t getUint32(int index) const;

  /**
   * Get the value at the input index as an unsigned 64-bit integer. If the actual value is wider, it will be truncated
   * to the last 8 bytes.
   *
   * @param index The index at which to retrieve the unsigned 64-bit integer value.
   * @return The value at the input index as an unsigned 64-bit integer value.
   */
  [[nodiscard]] uint64_t getUint64(int index) const;

  /**
   * Get the value at the input index as a Solidity address.
   *
   * @param index The index at which to retrieve the Solidity address value.
   * @return The value at the input index as a Solidity address value.
   */
  [[nodiscard]] std::string getAddress(int index) const;

  /**
   * The ID of the contract whose functions was called.
   */
  ContractId mContractId;

  /**
   * The result returned by the function.
   */
  std::vector<std::byte> mContractCallResult;

  /**
   * The error message, in case there was an error during smart contract execution.
   */
  std::string mErrorMessage;

  /**
   * The bloom filter for the record.
   */
  std::vector<std::byte> mBloom;

  /**
   * The units of gas used to execute the contract.
   */
  uint64_t mGasUsed = 0ULL;

  /**
   * The log info for events returned by the function.
   */
  std::vector<ContractLogInfo> mLogs;

  /**
   * The created contract's 20-byte EVM address.
   */
  std::optional<EvmAddress> mEvmAddress;

  /**
   * The amount of gas available for the call, aka the gasLimit.
   *
   * This field should only be populated when the paired TransactionBody in the record stream is not a
   * ContractCreateTransactionBody or a ContractCallTransactionBody.
   */
  uint64_t mGas = 0ULL;

  /**
   * The amount sent (the function must be payable if this is nonzero).
   *
   * This field should only be populated when the paired TransactionBody in the record stream is not a
   * ContractCreateTransactionBody or a ContractCallTransactionBody.
   */
  Hbar mHbarAmount;

  /**
   * The parameters passed into the contract call.
   *
   * This field should only be populated when the paired TransactionBody in the record stream is not a
   * ContractCreateTransactionBody or a ContractCallTransactionBody.
   */
  std::vector<std::byte> mFunctionParameters;

  /**
   * The account that is the "sender." If not present it is the account ID from the transaction ID.
   *
   * This field should only be populated when the paired TransactionBody in the record stream is not a
   * ContractCreateTransactionBody or a ContractCallTransactionBody.
   */
  AccountId mSenderAccountId;

private:
  /**
   * Get bytes that are a dynamic size.
   *
   * @param index The index at which to find the bytes.
   * @return The bytes of the proper size.
   */
  [[nodiscard]] std::vector<std::byte> getDynamicBytes(int index) const;

  /**
   * Get the 32-bit integer value at a specific index.
   *
   * @param index The index from which to get the 32-bit integer.
   * @return The 32-bit integer at the input index.
   */
  [[nodiscard]] int getIntValueAt(int index) const;

  /**
   * Get the bytes at a specific start index all the way to a specific end index.
   *
   * @param start The starting index of the bytes to retrieve.
   * @param end   The ending index of the bytes to retrieve.
   * @return The bytes between the start and end indices.
   */
  [[nodiscard]] std::vector<std::byte> getByteString(int start, int end) const;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_FUNCTION_RESULT_H_

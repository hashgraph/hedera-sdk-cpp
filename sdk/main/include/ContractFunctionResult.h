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
#ifndef CONTRACT_FUNCTION_RESULT_H_
#define CONTRACT_FUNCTION_RESULT_H_

#include "AccountId.h"
#include "ContractId.h"
#include "ContractLogInfo.h"
#include "Hbar.h"

#include "helper/InitType.h"

namespace proto
{
class ContractFunctionResult;
}

namespace Hedera
{
/**
 * The result returned by a call to a smart contract function. This is part of
 * the response to a ContractCallLocal query, and is in the record for a
 * ContractCall or ContractCreateInstance transaction. The
 * ContractCreateInstance transaction record has the results of the call to the
 * constructor.
 */
class ContractFunctionResult
{
public:
  static constexpr char ERROR_PREFIX[4] = { 8, -61, 121, -96 };

  /**
   * Constructor.
   */
  ContractFunctionResult();

  /**
   * Construct from a contract function result protobuf object.
   *
   * @param proto The contract function result protobuf object.
   */
  explicit ContractFunctionResult(const proto::ContractFunctionResult& proto);

  /**
   * Retrieve the contract function result from the protobuf object.
   *
   * @param proto The contract function result protobuf object.
   * @return A contract function result object.
   */
  static ContractFunctionResult fromProtobuf(
    const proto::ContractFunctionResult& proto);

  /**
   * Convert this contract function result into a protobuf object.
   *
   * @return The contract function result protobuf object.
   */
  proto::ContractFunctionResult toProtobuf() const;

  /**
   * Get the nth returned values as a string.
   *
   * @param index The index of the string to be returned.
   * @return The value at the input index, as a string.
   */
  std::string getString(const size_t& index) const;

  /**
   * Get the nth returned value as a list of strings.
   *
   * @param index The index of the list of strings to be retrieved.
   * @return The value at the input index, as a list of strings.
   */
  std::vector<std::string> getStringArray(const size_t& index) const;

  /**
   * Get the nth value as a boolean.
   *
   * @param valIndex The index of the boolean to be retrieved.
   * @return The value at the input index, as a boolean.
   */
  bool getBool(const size_t& index) const;

  /**
   * Get the nth returned value as an 8-bit integer.
   *
   * If the actual value is wider it will be truncated to the last byte.
   *
   * @param valIndex The index of the int8_t to be retrieved.
   * @return The value at the input index, as an int8_t.
   */
  int8_t getInt8(const size_t& index) const;

  /**
   * Get the nth returned value as a 32-bit integer.
   *
   * If the actual value is wider it will be truncated to the last 4 bytes.
   *
   * @param valIndex The index of the int32_t to be retrieved.
   * @return The value at the input index, as an int32_t.
   */
  int32_t getInt32(const size_t& index) const;

  /**
   * Get the nth returned value as a 64-bit integer.
   *
   * If the actual value is wider it will be truncated to the last 8 bytes.
   *
   * @param valIndex The index of the int64_t to be retrieved.
   * @return The value at the input index, as an int64_t.
   */
  int64_t getInt64(const size_t& index) const;

  /**
   * Get the nth returned value as a 8-bit unsigned integer.
   *
   * @param valIndex The index of the uint8_t to be retrieved.
   * @return The value at the input index, as a uint8_t.
   */
  uint8_t getUInt8(const size_t index) const;

  /**
   * Get the nth returned value as a 32-bit unsigned integer.
   *
   * @param valIndex The index of the uint32_t to be retrieved.
   * @return The value at the input index, as a uint32_t.
   */
  uint32_t getUInt32(const size_t index) const;

  /**
   * Get the nth returned value as a 64-bit unsigned integer.
   *
   * @param valIndex The index of the uint64_t to be retrieved.
   * @return The value at the input index, as a uint8_t.
   */
  uint64_t getUInt64(const size_t index) const;

  /**
   * Get the nth returned value as a Solidity address.
   *
   * @param valIndex The index of the value to be retrieved
   * @return String
   */
  std::string getAddress(const size_t index) const;

  /**
   * Extract the contract ID.
   *
   * @return The contract ID.
   */
  inline InitType<ContractId> getContractId() const { return mContractId; }

  /**
   * Extract the whole raw function result.
   *
   * @return The whole raw function result.
   */
  inline std::string getContractCallResult() const
  {
    return mContractCallResult;
  }

  /**
   * Extract the error message.
   *
   * @return The error message.
   */
  inline std::string getErrorMessage() const { return mErrorMessage; }

  /**
   * Extract the bloom filter record.
   *
   * @return The bloom filter record.
   */
  inline std::string getBloomFilter() const { return mBloom; }

  /**
   * Extract the amount of gas used to execute the contract.
   *
   * @return The amount of gas used to execute the contract.
   */
  inline int64_t getGasUsed() const { return mGasUsed; }

  /**
   * Extract the log info for events caused by the contract execution.
   *
   * @return The log info for events caused by the contract execution.
   */
  inline std::vector<ContractLogInfo> getContractLogInfo() const
  {
    return mLogInfo;
  }

  /**
   * Extract the EVM address of the contract.
   *
   * @return The EVM address of the contract.
   */
  inline InitType<std::string> getEvmAddress() const { return mEvmAddress; }

  /**
   * Extract the gas limit of the contract.
   *
   * @return The gas limit of the contract.
   */
  inline int64_t getGasLimit() const { return mGas; }

  /**
   * Extract the amount of Hbars sent.
   *
   * @return The amount of Hbars sent.
   */
  inline Hbar getAmount() const { return mAmount; }

  /**
   * Extract the parameters of the contract function.
   *
   * @return The parameters of the contract function.
   */
  inline std::string getFunctionParameters() const
  {
    return mFunctionParameters;
  }

  /**
   * Extract the ID of the sender account.
   *
   * @return The ID of the sender account.
   */
  inline InitType<AccountId> getSenderAccountId() const
  {
    return mSenderAccountId;
  }

private:
  /**
   * Get the bytes at a specific index.
   *
   * @param index The index at which to get the bytes.
   * @return ByteString
   */
  std::string getDynamicBytes(const size_t& index) const;

  /**
   * Get the integer value at the specified offset of the contract function
   * result.
   *
   * @param index The index at which to get the integer value.
   * @return The integer value of the bytes at the specified index.
   */
  int getIntegerValueAt(const size_t& index) const;

  /**
   * Grab a portion of the byte string contract function result.
   *
   * @param start The index of the first byte to grab.
   * @param end   The index of the last byte to grab.
   * @return The byte string of the function result between the two indices.
   */
  std::string getByteString(const size_t& start, const size_t& end) const;

  /**
   * The smart contract instance whose function was called.
   */
  InitType<ContractId> mContractId;

  /**
   * The result returned by the function.
   */
  std::string mContractCallResult;

  /**
   * The message in case there was an error during smart contract execution.
   */
  std::string mErrorMessage;

  /**
   * Bloom filter for record.
   */
  std::string mBloom;

  /**
   * Units of gas used to execute contract.
   */
  uint64_t mGasUsed;

  /**
   * The log info for events returned by the function.
   */
  std::vector<ContractLogInfo> mLogInfo;

  /**
   * The new contract's 20-byte EVM address. Only populated after release 0.23,
   * where each created contract will have its own record. (This is an important
   * point--the field is not a list because there will be a separate child
   * record for each created contract.)
   *
   * Every contract has an EVM address determined by its shard.realm.num id.
   * This address is as follows:
   *  - The first 4 bytes are the big-endian representation of the shard.
   *  - The next 8 bytes are the big-endian representation of the realm.
   *  - The final 8 bytes are the big-endian representation of the number.
   *
   * Contracts created via CREATE2 have an additional, primary address that is
   * derived from the EIP-1014 specification, and does not have a simple
   * relation to a shard.realm.num id.
   *
   * (Please do note that CREATE2 contracts can also be referenced by the
   * three-part EVM address described above.)
   */
  InitType<std::string> mEvmAddress;

  /**
   * The amount of gas available for the call, aka the gasLimit.
   *
   * This field should only be populated when the paired TransactionBody in the
   * record stream is not a ContractCreateTransactionBody or a
   * ContractCallTransactionBody.
   */
  int64_t mGas;

  /**
   * Number of tinybars sent (the function must be payable if this is nonzero).
   *
   * This field should only be populated when the paired TransactionBody in the
   * record stream is not a ContractCreateTransactionBody or a
   * ContractCallTransactionBody.
   */
  Hbar mAmount;

  /**
   * The parameters passed into the contract call.
   *
   * This field should only be populated when the paired TransactionBody in the
   * record stream is not a ContractCreateTransactionBody or a
   * ContractCallTransactionBody.
   */
  std::string mFunctionParameters;

  /**
   * The account that is the "sender." If not present it is the account ID from
   * the transaction ID.
   *
   * This field should only be populated when the paired TransactionBody in the
   * record stream is not a ContractCreateTransactionBody or a
   * ContractCallTransactionBody.
   */
  InitType<AccountId> mSenderAccountId;
};

} // namespace Hedera

#endif // CONTRACT_FUNCTION_RESULT_H_
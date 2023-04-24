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
#include "ContractFunctionSelector.h"
#include "exceptions/IllegalStateException.h"
#include "exceptions/OpenSSLException.h"
#include "impl/Utilities.h"
#include "impl/openssl_utils/EVP_MD.h"
#include "impl/openssl_utils/EVP_MD_CTX.h"
#include "impl/openssl_utils/OSSL_LIB_CTX.h"
#include "impl/openssl_utils/OpenSSLUtils.h"

#include <openssl/evp.h>

namespace Hedera
{
namespace
{
// The size of a keccak-256 hash
constexpr size_t KECCAK_256_HASH_SIZE = 32ULL;
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addName(std::string_view name)
{
  mFunctionCall = internal::Utilities::concatenateVectors(
    { internal::Utilities::stringToByteVector(name), { std::byte('(') }, mFunctionCall });
  return *this;
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addString()
{
  return addParameter("string");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addStringArray()
{
  return addParameter("string[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addBytes()
{
  return addParameter("bytes");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addBytesArray()
{
  return addParameter("bytes[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addBytes32()
{
  return addParameter("bytes32");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addBytes32Array()
{
  return addParameter("bytes32[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addBool()
{
  return addParameter("bool");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addInt8()
{
  return addParameter("int8");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addInt32()
{
  return addParameter("int32");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addInt64()
{
  return addParameter("int64");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addInt256()
{
  return addParameter("int256");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addInt8Array()
{
  return addParameter("int8[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addInt32Array()
{
  return addParameter("int32[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addInt64Array()
{
  return addParameter("int64[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addInt256Array()
{
  return addParameter("int256[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addUint8()
{
  return addParameter("uint8");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addUint32()
{
  return addParameter("uint32");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addUint64()
{
  return addParameter("uint64");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addUint256()
{
  return addParameter("uint256");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addUint8Array()
{
  return addParameter("uint8[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addUint32Array()
{
  return addParameter("uint32[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addUint64Array()
{
  return addParameter("uint64[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addUint256Array()
{
  return addParameter("uint256[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addAddress()
{
  return addParameter("address");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addAddressArray()
{
  return addParameter("address[]");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addFunction()
{
  return addParameter("function");
}

//-----
ContractFunctionSelector& ContractFunctionSelector::addParameter(std::string_view type)
{
  if (!mSelectorBytes.empty())
  {
    throw IllegalStateException("Solidity function selector bytes have already been generated");
  }

  if (addComma)
  {
    mFunctionCall.push_back(std::byte(','));
  }

  mFunctionCall =
    internal::Utilities::concatenateVectors({ mFunctionCall, internal::Utilities::stringToByteVector(type) });
  addComma = true;

  return *this;
}

//-----
std::vector<std::byte> ContractFunctionSelector::finish()
{
  if (mSelectorBytes.empty())
  {
    mFunctionCall.push_back(std::byte(')'));

    // Generate the Keccak-256 hash
    internal::OpenSSLUtils::OSSL_LIB_CTX libraryContext(OSSL_LIB_CTX_new());
    if (!libraryContext)
    {
      throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("OSSL_LIB_CTX_new"));
    }

    const internal::OpenSSLUtils::EVP_MD messageDigest(EVP_MD_fetch(libraryContext.get(), "KECCAK-256", nullptr));
    if (!messageDigest)
    {
      throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_fetch"));
    }

    internal::OpenSSLUtils::EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
    if (!messageDigestContext)
    {
      throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_MD_CTX_new"));
    }

    if (EVP_DigestInit(messageDigestContext.get(), messageDigest.get()) <= 0)
    {
      throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestInit_ex"));
    }

    if (EVP_DigestUpdate(messageDigestContext.get(),
                         internal::Utilities::toTypePtr<unsigned char>(mFunctionCall.data()),
                         mFunctionCall.size()) <= 0)
    {
      throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestUpdate"));
    }

    mSelectorBytes.resize(KECCAK_256_HASH_SIZE);
    if (EVP_DigestFinal(messageDigestContext.get(),
                        internal::Utilities::toTypePtr<unsigned char>(mSelectorBytes.data()),
                        nullptr) <= 0)
    {
      throw OpenSSLException(internal::OpenSSLUtils::getErrorMessage("EVP_DigestFinal_ex"));
    }

    mFunctionCall.clear();
  }

  return { mSelectorBytes.cbegin(), mSelectorBytes.cbegin() + 4 };
}

} // namespace Hedera

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
#include "ECDSAsecp256k1PrivateKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "exceptions/BadKeyException.h"
#include "exceptions/UninitializedException.h"
#include "impl/DerivationPathUtils.h"
#include "impl/HexConverter.h"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace Hedera;

class ECDSAsecp256k1PrivateKeyTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::unique_ptr<ECDSAsecp256k1PrivateKey>& getTestPrivateKeyGenerated() const
  {
    return mPrivateKeyGenerated;
  }
  [[nodiscard]] inline const std::unique_ptr<ECDSAsecp256k1PrivateKey>& getTestPrivateKeyLoaded() const
  {
    return mPrivateKeyLoaded;
  }

private:
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> mPrivateKeyGenerated = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::unique_ptr<ECDSAsecp256k1PrivateKey> mPrivateKeyLoaded =
    ECDSAsecp256k1PrivateKey::fromString(mPrivateKeyGenerated->toString());
};

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, GetPublicKey)
{
  // get the public keys from the private keys
  std::shared_ptr<PublicKey> publicFromGenerated = getTestPrivateKeyGenerated()->getPublicKey();
  std::shared_ptr<PublicKey> publicFromLoaded = getTestPrivateKeyLoaded()->getPublicKey();

  EXPECT_NE(publicFromGenerated, nullptr);
  EXPECT_NE(publicFromLoaded, nullptr);

  // make sure returned public keys are the same for generated or loaded private keys
  EXPECT_EQ(publicFromGenerated->toString(), publicFromLoaded->toString());
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, Sign)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  std::vector<unsigned char> signatureFromGenerated = getTestPrivateKeyGenerated()->sign(bytesToSign);
  std::vector<unsigned char> signatureFromLoaded = getTestPrivateKeyLoaded()->sign(bytesToSign);

  // ECDSA signing includes random elements, so we cannot compare the 2 signatures for equality
  EXPECT_NE(signatureFromLoaded, signatureFromGenerated);
  // output size should be <= 72
  EXPECT_LE(signatureFromGenerated.size(), 72);
  EXPECT_LE(signatureFromLoaded.size(), 72);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, SignEmptyBytes)
{
  std::vector<unsigned char> bytesToSign;

  std::vector<unsigned char> signatureFromGenerated = getTestPrivateKeyGenerated()->sign(bytesToSign);
  std::vector<unsigned char> signatureFromLoaded = getTestPrivateKeyLoaded()->sign(bytesToSign);

  // ECDSA signing includes random elements, so we cannot compare the 2 signatures for equality
  EXPECT_NE(signatureFromLoaded, signatureFromGenerated);
  // output size should be <= 72
  EXPECT_LE(signatureFromGenerated.size(), 72);
  EXPECT_LE(signatureFromLoaded.size(), 72);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, ToString)
{
  std::string stringFromGenerated = getTestPrivateKeyGenerated()->toString();
  std::string stringFromLoaded = getTestPrivateKeyLoaded()->toString();

  EXPECT_EQ(stringFromGenerated.size(), 64);
  EXPECT_EQ(stringFromLoaded.size(), 64);
  EXPECT_EQ(stringFromGenerated, stringFromLoaded);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, FromString)
{
  // these are 2 versions of the same private key. the first conforms to the full RFC 8410 standard, the second is just
  // the private key
  std::string privateKeyStringExtended =
    "302E0201010420E8F32E723DECF4051AEFAC8E2C93C9C5B214313817CDB01A1494B917C8436B35A00706052B8104000A";
  std::string privateKeyStringShort = "E8F32E723DECF4051AEFAC8E2C93C9C5B214313817CDB01A1494B917C8436B35";

  std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromExtended =
    ECDSAsecp256k1PrivateKey::fromString(privateKeyStringExtended);
  std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromShort =
    ECDSAsecp256k1PrivateKey::fromString(privateKeyStringShort);

  EXPECT_NE(privateKeyFromExtended, nullptr);
  EXPECT_NE(privateKeyFromShort, nullptr);
  EXPECT_EQ(privateKeyFromExtended->toString(), privateKeyFromShort->toString());

  // Throw on garbage data
  EXPECT_THROW(ECDSAsecp256k1PrivateKey::fromString("asdfdsafds"), BadKeyException);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, Derive)
{
  // Throw when not initialized with a chain code
  EXPECT_THROW(auto key = getTestPrivateKeyLoaded()->derive(0), UninitializedException);
  EXPECT_THROW(auto key = getTestPrivateKeyGenerated()->derive(0), UninitializedException);
}

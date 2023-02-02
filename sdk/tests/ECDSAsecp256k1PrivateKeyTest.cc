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

TEST_F(ECDSAsecp256k1PrivateKeyTest, ToString)
{
  std::string stringFromGenerated = getTestPrivateKeyGenerated()->toString();
  std::string stringFromLoaded = getTestPrivateKeyLoaded()->toString();

  EXPECT_EQ(stringFromGenerated.size(), 64);
  EXPECT_EQ(stringFromLoaded.size(), 64);
  EXPECT_EQ(stringFromGenerated, stringFromLoaded);
}

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
}

TEST_F(ECDSAsecp256k1PrivateKeyTest, SLIP10TestVector1)
{
  // SLIP10 spec provided test vector
  std::string hexSeed = "000102030405060708090a0b0c0d0e0f";

  std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey =
    ECDSAsecp256k1PrivateKey::fromSeed(internal::HexConverter::hexToBase64(hexSeed));

  std::string expectedChainCode = "873dff81c02f525623fd1fe5167eac3a55a049de3d314bb42ee227ffed37d508";
  std::string expectedPrivate = "e8f32e723decf4051aefac8e2c93c9c5b214313817cdb01a1494b917c8436b35";
  std::string expectedPublic = "0339a36013301597daef41fbe593a02cc513d0b55527ec2df1050e2e8ff49c85c2";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(internal::DerivationPathUtils::getHardenedIndex(0));
  expectedChainCode = "47fdacbd0f1097043b78c63c20c34ef4ed9a111d980047ad16282c7ae6236141";
  expectedPrivate = "edb2e14f9ee77d26dd93b4ecede8d16ed408ce149b6cd80b0715a2d911a0afea";
  expectedPublic = "035a784662a4a20a65bf6aab9ae98a6c068a81c52e4b032c0fb5400c706cfccc56";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(1);
  expectedChainCode = "2a7857631386ba23dacac34180dd1983734e444fdbf774041578e9b6adb37c19";
  expectedPrivate = "3c6cb8d0f6a264c91ea8b5030fadaa8e538b020f0a387421a12de9319dc93368";
  expectedPublic = "03501e454bf00751f24b1b489aa925215d66af2234e3891c3b21a52bedb3cd711c";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(internal::DerivationPathUtils::getHardenedIndex(2));
  expectedChainCode = "04466b9cc8e161e966409ca52986c584f07e9dc81f735db683c3ff6ec7b1503f";
  expectedPrivate = "cbce0d719ecf7431d88e6a89fa1483e02e35092af60c042b1df2ff59fa424dca";
  expectedPublic = "0357bfe1e341d01c69fe5654309956cbea516822fba8a601743a012a7896ee8dc2";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(2);
  expectedChainCode = "cfb71883f01676f587d023cc53a35bc7f88f724b1f8c2892ac1275ac822a3edd";
  expectedPrivate = "0f479245fb19a38a1954c5c7c0ebab2f9bdfd96a17563ef28a6a4b1a2a764ef4";
  expectedPublic = "02e8445082a72f29b75ca48748a914df60622a609cacfce8ed0e35804560741d29";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(1000000000);
  expectedChainCode = "c783e67b921d2beb8f6b389cc646d7263b4145701dadd2161548a8b078e65e9e";
  expectedPrivate = "471b76e389e528d6de6d816857e012c5455051cad6660850e58372a6c3e6e7c8";
  expectedPublic = "022a471424da5e657499d1ff51cb43c47481a03b1e77f951fe64cec9f5a48f7011";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));
}

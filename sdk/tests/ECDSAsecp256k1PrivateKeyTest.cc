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

TEST_F(ECDSAsecp256k1PrivateKeyTest, SLIP10TestVector2)
{
  // SLIP10 spec provided test vector
  std::string hexSeed = "fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875"
                        "726f6c696663605d5a5754514e4b484542";

  std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKey =
    ECDSAsecp256k1PrivateKey::fromSeed(internal::HexConverter::hexToBase64(hexSeed));

  std::string expectedChainCode = "60499f801b896d83179a4374aeb7822aaeaceaa0db1f85ee3e904c4defbd9689";
  std::string expectedPrivate = "4b03d6fc340455b363f51020ad3ecca4f0850280cf436c70c727923f6db46c3e";
  std::string expectedPublic = "03cbcaa9c98c877a26977d00825c956a238e8dddfbd322cce4f74b0b5bd6ace4a7";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(0);
  expectedChainCode = "f0909affaa7ee7abe5dd4e100598d4dc53cd709d5a5c2cac40e7412f232f7c9c";
  expectedPrivate = "abe74a98f6c7eabee0428f53798f0ab8aa1bd37873999041703c742f15ac7e1e";
  expectedPublic = "02fc9e5af0ac8d9b3cecfe2a888e2117ba3d089d8585886c9c826b6b22a98d12ea";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(internal::DerivationPathUtils::getHardenedIndex(2147483647));
  expectedChainCode = "be17a268474a6bb9c61e1d720cf6215e2a88c5406c4aee7b38547f585c9a37d9";
  expectedPrivate = "877c779ad9687164e9c2f4f0f4ff0340814392330693ce95a58fe18fd52e6e93";
  expectedPublic = "03c01e7425647bdefa82b12d9bad5e3e6865bee0502694b94ca58b666abc0a5c3b";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(1);
  expectedChainCode = "f366f48f1ea9f2d1d3fe958c95ca84ea18e4c4ddb9366c336c927eb246fb38cb";
  expectedPrivate = "704addf544a06e5ee4bea37098463c23613da32020d604506da8c0518e1da4b7";
  expectedPublic = "03a7d1d856deb74c508e05031f9895dab54626251b3806e16b4bd12e781a7df5b9";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(internal::DerivationPathUtils::getHardenedIndex(2147483646));
  expectedChainCode = "637807030d55d01f9a0cb3a7839515d796bd07706386a6eddf06cc29a65a0e29";
  expectedPrivate = "f1c7c871a54a804afe328b4c83a1c33b8e5ff48f5087273f04efa83b247d6a2d";
  expectedPublic = "02d2b36900396c9282fa14628566582f206a5dd0bcc8d5e892611806cafb0301f0";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(2);
  expectedChainCode = "9452b549be8cea3ecb7a84bec10dcfd94afe4d129ebfd3b3cb58eedf394ed271";
  expectedPrivate = "bb7d39bdb83ecf58f2fd82b6d918341cbef428661ef01ab97c28a4842125ac23";
  expectedPublic = "024d902e1a2fc7a8755ab5b694c575fce742c48d9ff192e63df5193e4c7afe1f9c";

  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));
}

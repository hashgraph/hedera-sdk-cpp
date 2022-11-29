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

#include <gtest/gtest.h>
#include <memory>
#include <vector>

#include "ED25519PrivateKey.h"
#include "ED25519PublicKey.h"
#include "helper/HexConverter.h"
#include "helper/DerivationPathUtils.h"

using namespace Hedera;

class ED25519PrivateKeyTest : public ::testing::Test
{
protected:
  std::unique_ptr<Hedera::ED25519PrivateKey> privateKeyGenerated;
  std::unique_ptr<Hedera::ED25519PrivateKey> privateKeyLoaded;

  void SetUp() override
  {
    // generate a private key
    privateKeyGenerated = Hedera::ED25519PrivateKey::generatePrivateKey();

    // serialize and then load private key back in
    privateKeyLoaded = Hedera::ED25519PrivateKey::fromString(privateKeyGenerated->toString());
  }
};

TEST_F(ED25519PrivateKeyTest, GetPublicKey)
{
  // get the public keys from the private keys
  std::shared_ptr<Hedera::PublicKey> publicFromGenerated = privateKeyGenerated->getPublicKey();
  std::shared_ptr<Hedera::PublicKey> publicFromLoaded = privateKeyLoaded->getPublicKey();

  EXPECT_NE(publicFromGenerated, nullptr);
  EXPECT_NE(publicFromLoaded, nullptr);

  // make sure returned public keys are the same for generated or loaded private
  // keys
  EXPECT_EQ(publicFromGenerated->toString(), publicFromLoaded->toString());
}

TEST_F(ED25519PrivateKeyTest, Sign)
{
  std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  std::vector<unsigned char> signatureFromGenerated = privateKeyGenerated->sign(bytesToSign);
  std::vector<unsigned char> signatureFromLoaded = privateKeyLoaded->sign(bytesToSign);

  EXPECT_EQ(signatureFromGenerated.size(), 64);
  EXPECT_EQ(signatureFromLoaded.size(), 64);
  EXPECT_EQ(signatureFromGenerated, signatureFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, SignEmptyBytes)
{
  std::vector<unsigned char> bytesToSign = std::vector<unsigned char>();

  std::vector<unsigned char> signatureFromGenerated = privateKeyGenerated->sign(bytesToSign);
  std::vector<unsigned char> signatureFromLoaded = privateKeyLoaded->sign(bytesToSign);

  EXPECT_EQ(signatureFromGenerated.size(), 64);
  EXPECT_EQ(signatureFromLoaded.size(), 64);
  EXPECT_EQ(signatureFromGenerated, signatureFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, ToString)
{
  std::string stringFromGenerated = privateKeyGenerated->toString();
  std::string stringFromLoaded = privateKeyLoaded->toString();

  EXPECT_EQ(stringFromGenerated.size(), 64);
  EXPECT_EQ(stringFromLoaded.size(), 64);
  EXPECT_EQ(stringFromGenerated, stringFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, FromString)
{
  // these are 2 versions of the same private key. the first conforms to the full RFC 8410 standard, the second is just
  // the private key
  std::string privateKeyStringExtended =
    "302E020100300506032B65700422042068FBA516472B387C9F33C3E667616D806E5B9CEFF23A766E5D9A3818C77871F1";
  std::string privateKeyStringShort = "68FBA516472B387C9F33C3E667616D806E5B9CEFF23A766E5D9A3818C77871F1";

  std::unique_ptr<ED25519PrivateKey> privateKeyFromExtended = ED25519PrivateKey::fromString(privateKeyStringExtended);
  std::unique_ptr<ED25519PrivateKey> privateKeyFromShort = ED25519PrivateKey::fromString(privateKeyStringShort);

  EXPECT_NE(privateKeyFromExtended, nullptr);
  EXPECT_NE(privateKeyFromShort, nullptr);
  EXPECT_EQ(privateKeyFromExtended->toString(), privateKeyFromShort->toString());
}

TEST_F(ED25519PrivateKeyTest, SLIP10TestVector2)
{
  // from SLIP10 spec
  std::string hexSeed = "fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875"
                        "726f6c696663605d5a5754514e4b484542";

  std::unique_ptr<ED25519PrivateKey> privateKey = ED25519PrivateKey::fromSeed(HexConverter::hexToBase64(hexSeed));
  std::string expectedChainCode = "ef70a74db9c3a5af931b5fe73ed8e1a53464133654fd55e7a66f8570b8e33c3b";
  std::string expectedPrivate = "171cb88b1b3c1db25add599712e36245d75bc65a1a5c9e18d76f9f2b1eab4012";
  std::string expectedPublic = "8fe9693f8fa62a4305a140b9764c5ee01e455963744fe18204b4fb948249308a";
  EXPECT_EQ(privateKey->getChainCode(), HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(DerivationPathUtils::getHardenedIndex(0));
  expectedChainCode = "0b78a3226f915c082bf118f83618a618ab6dec793752624cbeb622acb562862d";
  expectedPrivate = "1559eb2bbec5790b0c65d8693e4d0875b1747f4970ae8b650486ed7470845635";
  expectedPublic = "86fab68dcb57aa196c77c5f264f215a112c22a912c10d123b0d03c3c28ef1037";
  EXPECT_EQ(privateKey->getChainCode(), HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(DerivationPathUtils::getHardenedIndex(2147483647));
  expectedChainCode = "138f0b2551bcafeca6ff2aa88ba8ed0ed8de070841f0c4ef0165df8181eaad7f";
  expectedPrivate = "ea4f5bfe8694d8bb74b7b59404632fd5968b774ed545e810de9c32a4fb4192f4";
  expectedPublic = "5ba3b9ac6e90e83effcd25ac4e58a1365a9e35a3d3ae5eb07b9e4d90bcf7506d";
  EXPECT_EQ(privateKey->getChainCode(), HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(DerivationPathUtils::getHardenedIndex(1));
  expectedChainCode = "73bd9fff1cfbde33a1b846c27085f711c0fe2d66fd32e139d3ebc28e5a4a6b90";
  expectedPrivate = "3757c7577170179c7868353ada796c839135b3d30554bbb74a4b1e4a5a58505c";
  expectedPublic = "2e66aa57069c86cc18249aecf5cb5a9cebbfd6fadeab056254763874a9352b45";
  EXPECT_EQ(privateKey->getChainCode(), HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(DerivationPathUtils::getHardenedIndex(2147483646));
  expectedChainCode = "0902fe8a29f9140480a00ef244bd183e8a13288e4412d8389d140aac1794825a";
  expectedPrivate = "5837736c89570de861ebc173b1086da4f505d4adb387c6a1b1342d5e4ac9ec72";
  expectedPublic = "e33c0f7d81d843c572275f287498e8d408654fdf0d1e065b84e2e6f157aab09b";
  EXPECT_EQ(privateKey->getChainCode(), HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(DerivationPathUtils::getHardenedIndex(2));
  expectedChainCode = "5d70af781f3a37b829f0d060924d5e960bdc02e85423494afc0b1a41bbe196d4";
  expectedPrivate = "551d333177df541ad876a60ea71f00447931c0a9da16f227c11ea080d7391b8d";
  expectedPublic = "47150c75db263559a70d5778bf36abbab30fb061ad69f69ece61a72b0cfa4fc0";
  EXPECT_EQ(privateKey->getChainCode(), HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), HexConverter::hexToBase64(expectedPublic));
}

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
#include "ED25519PrivateKey.h"
#include "ED25519PublicKey.h"
#include "exceptions/BadKeyException.h"
#include "exceptions/UninitializedException.h"
#include "impl/DerivationPathUtils.h"
#include "impl/HexConverter.h"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

using namespace Hedera;

class ED25519PrivateKeyTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::unique_ptr<ED25519PrivateKey>& getTestPrivateKeyGenerated() const
  {
    return mPrivateKeyGenerated;
  }

  [[nodiscard]] inline const std::unique_ptr<ED25519PrivateKey>& getTestPrivateKeyLoaded() const
  {
    return mPrivateKeyLoaded;
  }

private:
  const std::unique_ptr<ED25519PrivateKey> mPrivateKeyGenerated = ED25519PrivateKey::generatePrivateKey();
  const std::unique_ptr<ED25519PrivateKey> mPrivateKeyLoaded =
    ED25519PrivateKey::fromString(mPrivateKeyGenerated->toString());
};

TEST_F(ED25519PrivateKeyTest, CopyAndMoveConstructors)
{
  ED25519PrivateKey copiedPrivateKey(*getTestPrivateKeyGenerated());
  EXPECT_EQ(copiedPrivateKey.toString(), getTestPrivateKeyGenerated()->toString());

  copiedPrivateKey = *getTestPrivateKeyLoaded();
  EXPECT_EQ(copiedPrivateKey.toString(), getTestPrivateKeyLoaded()->toString());

  ED25519PrivateKey movedPrivateKey(std::move(copiedPrivateKey));
  EXPECT_EQ(movedPrivateKey.toString(), getTestPrivateKeyLoaded()->toString());

  copiedPrivateKey = std::move(movedPrivateKey);
  EXPECT_EQ(copiedPrivateKey.toString(), getTestPrivateKeyLoaded()->toString());
}

TEST_F(ED25519PrivateKeyTest, GetPublicKey)
{
  // get the public keys from the private keys
  const std::shared_ptr<PublicKey> publicFromGenerated = getTestPrivateKeyGenerated()->getPublicKey();
  const std::shared_ptr<PublicKey> publicFromLoaded = getTestPrivateKeyLoaded()->getPublicKey();

  EXPECT_NE(publicFromGenerated, nullptr);
  EXPECT_NE(publicFromLoaded, nullptr);
  EXPECT_EQ(publicFromGenerated->toString(), publicFromLoaded->toString());
}

TEST_F(ED25519PrivateKeyTest, Sign)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> signatureFromGenerated = getTestPrivateKeyGenerated()->sign(bytesToSign);
  const std::vector<unsigned char> signatureFromLoaded = getTestPrivateKeyLoaded()->sign(bytesToSign);

  EXPECT_EQ(signatureFromGenerated.size(), 64);
  EXPECT_EQ(signatureFromLoaded.size(), 64);
  EXPECT_EQ(signatureFromGenerated, signatureFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, SignEmptyBytes)
{
  const std::vector<unsigned char> bytesToSign;
  const std::vector<unsigned char> signatureFromGenerated = getTestPrivateKeyGenerated()->sign(bytesToSign);
  const std::vector<unsigned char> signatureFromLoaded = getTestPrivateKeyLoaded()->sign(bytesToSign);

  EXPECT_EQ(signatureFromGenerated.size(), 64);
  EXPECT_EQ(signatureFromLoaded.size(), 64);
  EXPECT_EQ(signatureFromGenerated, signatureFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, ToString)
{
  const std::string stringFromGenerated = getTestPrivateKeyGenerated()->toString();
  const std::string stringFromLoaded = getTestPrivateKeyLoaded()->toString();

  EXPECT_EQ(stringFromGenerated.size(), 64);
  EXPECT_EQ(stringFromLoaded.size(), 64);
  EXPECT_EQ(stringFromGenerated, stringFromLoaded);
}

TEST_F(ED25519PrivateKeyTest, FromString)
{
  // These are 2 versions of the same private key. The first conforms to the full RFC 8410 standard, the second is just
  // the private key.
  const std::string privateKeyStringExtended =
    "302E020100300506032B65700422042068FBA516472B387C9F33C3E667616D806E5B9CEFF23A766E5D9A3818C77871F1";
  const std::string privateKeyStringShort = "68FBA516472B387C9F33C3E667616D806E5B9CEFF23A766E5D9A3818C77871F1";

  const std::unique_ptr<ED25519PrivateKey> privateKeyFromExtended =
    ED25519PrivateKey::fromString(privateKeyStringExtended);
  const std::unique_ptr<ED25519PrivateKey> privateKeyFromShort = ED25519PrivateKey::fromString(privateKeyStringShort);

  EXPECT_NE(privateKeyFromExtended, nullptr);
  EXPECT_NE(privateKeyFromShort, nullptr);
  EXPECT_EQ(privateKeyFromExtended->toString(), privateKeyFromShort->toString());

  // Throw on garbage data
  EXPECT_THROW(ED25519PrivateKey::fromString("asdfdsafds"), BadKeyException);
}

//-----
TEST_F(ED25519PrivateKeyTest, Derive)
{
  std::unique_ptr<ED25519PrivateKey> privateKey =
    ED25519PrivateKey::fromSeed(internal::HexConverter::hexToBase64("000102030405060708090a0b0c0d0e0f"));

  // Throw when not initialized with a chain code
  EXPECT_THROW(privateKey = getTestPrivateKeyLoaded()->derive(0), UninitializedException);
  EXPECT_THROW(privateKey = getTestPrivateKeyGenerated()->derive(0), UninitializedException);

  // Throw if input index is hardened
  EXPECT_THROW(privateKey = privateKey->derive(1 | 0x80000000), std::invalid_argument);
}

TEST_F(ED25519PrivateKeyTest, SLIP10TestVector1)
{
  // SLIP10 spec provided test vector
  const std::string hexSeed = "000102030405060708090a0b0c0d0e0f";

  std::unique_ptr<ED25519PrivateKey> privateKey =
    ED25519PrivateKey::fromSeed(internal::HexConverter::hexToBase64(hexSeed));
  std::string expectedChainCode = "90046a93de5380a72b5e45010748567d5ea02bbf6522f979e05c0d8d8ca9fffb";
  std::string expectedPrivate = "2b4be7f19ee27bbf30c667b642d5f4aa69fd169872f8fc3059c08ebae2eb19e7";
  std::string expectedPublic = "a4b2856bfec510abab89753fac1ac0e1112364e7d250545963f135f2a33188ed";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(0);
  expectedChainCode = "8b59aa11380b624e81507a27fedda59fea6d0b779a778918a2fd3590e16e9c69";
  expectedPrivate = "68e0fe46dfb67e368c75379acec591dad19df3cde26e63b93a8e704f1dade7a3";
  expectedPublic = "8c8a13df77a28f3445213a0f432fde644acaa215fc72dcdf300d5efaa85d350c";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(1);
  expectedChainCode = "a320425f77d1b5c2505a6b1b27382b37368ee640e3557c315416801243552f14";
  expectedPrivate = "b1d0bad404bf35da785a64ca1ac54b2617211d2777696fbffaf208f746ae84f2";
  expectedPublic = "1932a5270f335bed617d5b935c80aedb1a35bd9fc1e31acafd5372c30f5c1187";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(2);
  expectedChainCode = "2e69929e00b5ab250f49c3fb1c12f252de4fed2c1db88387094a0f8c4c9ccd6c";
  expectedPrivate = "92a5b23c0b8a99e37d07df3fb9966917f5d06e02ddbd909c7e184371463e9fc9";
  expectedPublic = "ae98736566d30ed0e9d2f4486a64bc95740d89c7db33f52121f8ea8f76ff0fc1";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(2);
  expectedChainCode = "8f6d87f93d750e0efccda017d662a1b31a266e4a6f5993b15f5c1f07f74dd5cc";
  expectedPrivate = "30d1dc7e5fc04c31219ab25a27ae00b50f6fd66622f6e9c913253d6511d1e662";
  expectedPublic = "8abae2d66361c879b900d204ad2cc4984fa2aa344dd7ddc46007329ac76c429c";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(1000000000);
  expectedChainCode = "68789923a0cac2cd5a29172a475fe9e0fb14cd6adb5ad98a3fa70333e7afa230";
  expectedPrivate = "8f94d394a8e8fd6b1bc2f3f49f5c47e385281d5c17e65324b0f62483e37e8793";
  expectedPublic = "3c24da049451555d51a7014a37337aa4e12d41e485abccfa46b47dfb2af54b7a";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));
}

TEST_F(ED25519PrivateKeyTest, SLIP10TestVector2)
{
  // SLIP10 spec provided test vector
  const std::string hexSeed = "fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e"
                              "7b7875726f6c696663605d5a5754514e4b484542";

  std::unique_ptr<ED25519PrivateKey> privateKey =
    ED25519PrivateKey::fromSeed(internal::HexConverter::hexToBase64(hexSeed));
  std::string expectedChainCode = "ef70a74db9c3a5af931b5fe73ed8e1a53464133654fd55e7a66f8570b8e33c3b";
  std::string expectedPrivate = "171cb88b1b3c1db25add599712e36245d75bc65a1a5c9e18d76f9f2b1eab4012";
  std::string expectedPublic = "8fe9693f8fa62a4305a140b9764c5ee01e455963744fe18204b4fb948249308a";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(0);
  expectedChainCode = "0b78a3226f915c082bf118f83618a618ab6dec793752624cbeb622acb562862d";
  expectedPrivate = "1559eb2bbec5790b0c65d8693e4d0875b1747f4970ae8b650486ed7470845635";
  expectedPublic = "86fab68dcb57aa196c77c5f264f215a112c22a912c10d123b0d03c3c28ef1037";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(2147483647);
  expectedChainCode = "138f0b2551bcafeca6ff2aa88ba8ed0ed8de070841f0c4ef0165df8181eaad7f";
  expectedPrivate = "ea4f5bfe8694d8bb74b7b59404632fd5968b774ed545e810de9c32a4fb4192f4";
  expectedPublic = "5ba3b9ac6e90e83effcd25ac4e58a1365a9e35a3d3ae5eb07b9e4d90bcf7506d";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(1);
  expectedChainCode = "73bd9fff1cfbde33a1b846c27085f711c0fe2d66fd32e139d3ebc28e5a4a6b90";
  expectedPrivate = "3757c7577170179c7868353ada796c839135b3d30554bbb74a4b1e4a5a58505c";
  expectedPublic = "2e66aa57069c86cc18249aecf5cb5a9cebbfd6fadeab056254763874a9352b45";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(2147483646);
  expectedChainCode = "0902fe8a29f9140480a00ef244bd183e8a13288e4412d8389d140aac1794825a";
  expectedPrivate = "5837736c89570de861ebc173b1086da4f505d4adb387c6a1b1342d5e4ac9ec72";
  expectedPublic = "e33c0f7d81d843c572275f287498e8d408654fdf0d1e065b84e2e6f157aab09b";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));

  privateKey = privateKey->derive(2);
  expectedChainCode = "5d70af781f3a37b829f0d060924d5e960bdc02e85423494afc0b1a41bbe196d4";
  expectedPrivate = "551d333177df541ad876a60ea71f00447931c0a9da16f227c11ea080d7391b8d";
  expectedPublic = "47150c75db263559a70d5778bf36abbab30fb061ad69f69ece61a72b0cfa4fc0";
  EXPECT_EQ(privateKey->getChainCode(), internal::HexConverter::hexToBase64(expectedChainCode));
  EXPECT_EQ(privateKey->toBytes(), internal::HexConverter::hexToBase64(expectedPrivate));
  EXPECT_EQ(privateKey->getPublicKey()->toBytes(), internal::HexConverter::hexToBase64(expectedPublic));
}

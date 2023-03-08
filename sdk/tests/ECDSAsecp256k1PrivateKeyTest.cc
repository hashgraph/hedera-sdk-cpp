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
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

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
    ECDSAsecp256k1PrivateKey::fromString(mPrivateKeyGenerated->toStringRaw());
};

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, FromString)
{
  const std::string privateKeyString = "E8F32E723DECF4051AEFAC8E2C93C9C5B214313817CDB01A1494B917C8436B35";
  const std::string privateKeyStringDer = ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX + privateKeyString;

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromString =
    ECDSAsecp256k1PrivateKey::fromString(privateKeyString);

  ASSERT_NE(privateKeyFromString, nullptr);
  EXPECT_EQ(privateKeyFromString->toStringDer(), privateKeyStringDer);
  EXPECT_EQ(privateKeyFromString->toStringRaw(), privateKeyString);
  EXPECT_EQ(privateKeyFromString->toBytesDer(), internal::HexConverter::hexToBytes(privateKeyStringDer));
  EXPECT_EQ(privateKeyFromString->toBytesRaw(), internal::HexConverter::hexToBytes(privateKeyString));

  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromStringDer =
                 ECDSAsecp256k1PrivateKey::fromStringDer(privateKeyString),
               BadKeyException);

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromStringDer =
    ECDSAsecp256k1PrivateKey::fromStringDer(privateKeyStringDer);

  ASSERT_NE(privateKeyFromStringDer, nullptr);
  EXPECT_EQ(privateKeyFromStringDer->toStringDer(), privateKeyFromString->toStringDer());
  EXPECT_EQ(privateKeyFromStringDer->toStringRaw(), privateKeyFromString->toStringRaw());
  EXPECT_EQ(privateKeyFromStringDer->toBytesDer(), privateKeyFromString->toBytesDer());
  EXPECT_EQ(privateKeyFromStringDer->toBytesRaw(), privateKeyFromString->toBytesRaw());

  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromStringRaw =
                 ECDSAsecp256k1PrivateKey::fromStringRaw(privateKeyStringDer),
               BadKeyException);

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromStringRaw =
    ECDSAsecp256k1PrivateKey::fromStringRaw(privateKeyString);

  ASSERT_NE(privateKeyFromStringRaw, nullptr);
  EXPECT_EQ(privateKeyFromStringRaw->toStringDer(), privateKeyFromStringDer->toStringDer());
  EXPECT_EQ(privateKeyFromStringRaw->toStringRaw(), privateKeyFromStringDer->toStringRaw());
  EXPECT_EQ(privateKeyFromStringRaw->toBytesDer(), privateKeyFromStringDer->toBytesDer());
  EXPECT_EQ(privateKeyFromStringRaw->toBytesRaw(), privateKeyFromStringDer->toBytesRaw());

  // Throw if input garbage
  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> key =
                 ECDSAsecp256k1PrivateKey::fromString("fdsakfdsalf"),
               BadKeyException);
  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> key =
                 ECDSAsecp256k1PrivateKey::fromString(ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX + "fjdskaf;"),
               BadKeyException);
  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> key = ECDSAsecp256k1PrivateKey::fromString(
                 ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_HEX +
                 "F83DEF42411E046461D5AEEAE9S11C56F661 557F349F3412DBD95C9FE8B026X"),
               BadKeyException);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, FromBytes)
{
  const std::vector<unsigned char> privateKeyBytes = { 0xE8, 0xF3, 0x2E, 0x72, 0x3D, 0xEC, 0xF4, 0x05, 0x1A, 0xEF, 0xAC,
                                                       0x8E, 0x2C, 0x93, 0xC9, 0xC5, 0xB2, 0x14, 0x31, 0x38, 0x17, 0xCD,
                                                       0xB0, 0x1A, 0x14, 0x94, 0xB9, 0x17, 0xC8, 0x43, 0x6B, 0x35 };
  const std::vector<unsigned char> privateKeyBytesDer =
    internal::Utilities::concatenateVectors(ECDSAsecp256k1PrivateKey::DER_ENCODED_PREFIX_BYTES, privateKeyBytes);

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromBytes =
    ECDSAsecp256k1PrivateKey::fromBytes(privateKeyBytes);

  ASSERT_NE(privateKeyFromBytes, nullptr);
  EXPECT_EQ(privateKeyFromBytes->toStringDer(), internal::HexConverter::bytesToHex(privateKeyBytesDer));
  EXPECT_EQ(privateKeyFromBytes->toStringRaw(), internal::HexConverter::bytesToHex(privateKeyBytes));
  EXPECT_EQ(privateKeyFromBytes->toBytesDer(), privateKeyBytesDer);
  EXPECT_EQ(privateKeyFromBytes->toBytesRaw(), privateKeyBytes);

  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromBytesDer =
                 ECDSAsecp256k1PrivateKey::fromBytesDer(privateKeyBytes),
               BadKeyException);

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromBytesDer =
    ECDSAsecp256k1PrivateKey::fromBytesDer(privateKeyBytesDer);

  ASSERT_NE(privateKeyFromBytesDer, nullptr);
  EXPECT_EQ(privateKeyFromBytesDer->toStringDer(), privateKeyFromBytes->toStringDer());
  EXPECT_EQ(privateKeyFromBytesDer->toStringRaw(), privateKeyFromBytes->toStringRaw());
  EXPECT_EQ(privateKeyFromBytesDer->toBytesDer(), privateKeyFromBytes->toBytesDer());
  EXPECT_EQ(privateKeyFromBytesDer->toBytesRaw(), privateKeyFromBytes->toBytesRaw());

  EXPECT_THROW(const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromBytesRaw =
                 ECDSAsecp256k1PrivateKey::fromBytesRaw(privateKeyBytesDer),
               BadKeyException);

  const std::unique_ptr<ECDSAsecp256k1PrivateKey> privateKeyFromBytesRaw =
    ECDSAsecp256k1PrivateKey::fromBytesRaw(privateKeyBytes);

  ASSERT_NE(privateKeyFromBytesRaw, nullptr);
  EXPECT_EQ(privateKeyFromBytesRaw->toStringDer(), privateKeyFromBytesDer->toStringDer());
  EXPECT_EQ(privateKeyFromBytesRaw->toStringRaw(), privateKeyFromBytesDer->toStringRaw());
  EXPECT_EQ(privateKeyFromBytesRaw->toBytesDer(), privateKeyFromBytesDer->toBytesDer());
  EXPECT_EQ(privateKeyFromBytesRaw->toBytesRaw(), privateKeyFromBytesDer->toBytesRaw());
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, GetPublicKey)
{
  // get the public keys from the private keys
  const std::shared_ptr<PublicKey> publicFromGenerated = getTestPrivateKeyGenerated()->getPublicKey();
  const std::shared_ptr<PublicKey> publicFromLoaded = getTestPrivateKeyLoaded()->getPublicKey();

  EXPECT_NE(publicFromGenerated, nullptr);
  EXPECT_NE(publicFromLoaded, nullptr);

  // make sure returned public keys are the same for generated or loaded private keys
  EXPECT_EQ(publicFromGenerated->toStringDer(), publicFromLoaded->toStringDer());
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, Sign)
{
  const std::vector<unsigned char> bytesToSign = { 0x1, 0x2, 0x3 };
  const std::vector<unsigned char> signatureFromGenerated = getTestPrivateKeyGenerated()->sign(bytesToSign);
  const std::vector<unsigned char> signatureFromLoaded = getTestPrivateKeyLoaded()->sign(bytesToSign);

  // ECDSA signing includes random elements, so we cannot compare the 2 signatures for equality
  EXPECT_NE(signatureFromLoaded, signatureFromGenerated);
  // output size should be <= 72
  EXPECT_LE(signatureFromGenerated.size(), 72);
  EXPECT_LE(signatureFromLoaded.size(), 72);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, SignEmptyBytes)
{
  const std::vector<unsigned char> signatureFromGenerated = getTestPrivateKeyGenerated()->sign({});
  const std::vector<unsigned char> signatureFromLoaded = getTestPrivateKeyLoaded()->sign({});

  // ECDSA signing includes random elements, so we cannot compare the 2 signatures for equality
  EXPECT_NE(signatureFromLoaded, signatureFromGenerated);
  // output size should be <= 72
  EXPECT_LE(signatureFromGenerated.size(), 72);
  EXPECT_LE(signatureFromLoaded.size(), 72);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, ToString)
{
  std::string stringFromGenerated = getTestPrivateKeyGenerated()->toStringRaw();
  std::string stringFromLoaded = getTestPrivateKeyLoaded()->toStringRaw();

  EXPECT_EQ(stringFromGenerated.size(), 64);
  EXPECT_EQ(stringFromLoaded.size(), 64);
  EXPECT_EQ(stringFromGenerated, stringFromLoaded);
}

//-----
TEST_F(ECDSAsecp256k1PrivateKeyTest, Derive)
{
  // Throw when not initialized with a chain code
  EXPECT_THROW(auto key = getTestPrivateKeyLoaded()->derive(0), UninitializedException);
  EXPECT_THROW(auto key = getTestPrivateKeyGenerated()->derive(0), UninitializedException);
}

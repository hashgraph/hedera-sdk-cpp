/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "AccountId.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ED25519PrivateKey.h"
#include "PublicKey.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <limits>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class AccountIdTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const uint64_t& getTestShardNum() const { return mShardNum; }
  [[nodiscard]] inline const uint64_t& getTestRealmNum() const { return mRealmNum; }
  [[nodiscard]] inline const uint64_t& getTestAccountNum() const { return mAccountNum; }
  [[nodiscard]] inline const uint64_t& getTestNumTooBig() const { return mNumTooBig; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestEd25519Alias() const { return mEd25519Alias; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestEcdsaSecp256k1Alias() const
  {
    return mEcdsaSecp256k1Alias;
  }
  [[nodiscard]] inline const EvmAddress& getTestEvmAddressAlias() const { return mEvmAddressAlias; }

private:
  const uint64_t mShardNum = 8ULL;
  const uint64_t mRealmNum = 90ULL;
  const uint64_t mAccountNum = 1000ULL;
  const uint64_t mNumTooBig = static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1ULL;
  const std::shared_ptr<PublicKey> mEd25519Alias = ED25519PrivateKey::generatePrivateKey()->getPublicKey();
  const std::shared_ptr<PublicKey> mEcdsaSecp256k1Alias =
    ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  const EvmAddress mEvmAddressAlias = EvmAddress::fromString("0x0123456789abcdef0123456789abcdef01234567");
};

//-----
TEST_F(AccountIdTest, DefaultConstructAccountId)
{
  const AccountId accountId;
  EXPECT_EQ(accountId.getShardNum(), 0ULL);
  EXPECT_EQ(accountId.getRealmNum(), 0ULL);
  EXPECT_FALSE(accountId.getAccountNum().has_value());
  EXPECT_EQ(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_FALSE(accountId.getEvmAddressAlias().has_value());
}

//-----
TEST_F(AccountIdTest, ConstructWithAccountNum)
{
  const AccountId accountId(getTestAccountNum());
  EXPECT_EQ(accountId.getShardNum(), 0ULL);
  EXPECT_EQ(accountId.getRealmNum(), 0ULL);
  EXPECT_TRUE(accountId.getAccountNum().has_value());
  EXPECT_EQ(*accountId.getAccountNum(), getTestAccountNum());
  EXPECT_EQ(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_FALSE(accountId.getEvmAddressAlias().has_value());

  EXPECT_THROW(AccountId{ getTestNumTooBig() }, std::invalid_argument);
}

//-----
TEST_F(AccountIdTest, ConstructWithAccountAlias)
{
  const AccountId ed25519AliasAccountId(getTestEd25519Alias());
  EXPECT_EQ(ed25519AliasAccountId.getShardNum(), 0ULL);
  EXPECT_EQ(ed25519AliasAccountId.getRealmNum(), 0ULL);
  EXPECT_FALSE(ed25519AliasAccountId.getAccountNum().has_value());
  EXPECT_NE(ed25519AliasAccountId.getPublicKeyAlias(), nullptr);
  EXPECT_EQ(ed25519AliasAccountId.getPublicKeyAlias()->toStringDer(), getTestEd25519Alias()->toStringDer());
  EXPECT_FALSE(ed25519AliasAccountId.getEvmAddressAlias().has_value());

  const AccountId ecdsaAliasAccountId(getTestEcdsaSecp256k1Alias());
  EXPECT_EQ(ecdsaAliasAccountId.getShardNum(), 0ULL);
  EXPECT_EQ(ecdsaAliasAccountId.getRealmNum(), 0ULL);
  EXPECT_FALSE(ecdsaAliasAccountId.getAccountNum().has_value());
  EXPECT_NE(ecdsaAliasAccountId.getPublicKeyAlias(), nullptr);
  EXPECT_EQ(ecdsaAliasAccountId.getPublicKeyAlias()->toStringDer(), getTestEcdsaSecp256k1Alias()->toStringDer());
  EXPECT_FALSE(ecdsaAliasAccountId.getEvmAddressAlias().has_value());
}

//-----
TEST_F(AccountIdTest, ConstructWithEvmAddress)
{
  const AccountId accountId(getTestEvmAddressAlias());
  EXPECT_EQ(accountId.getShardNum(), 0ULL);
  EXPECT_EQ(accountId.getRealmNum(), 0ULL);
  EXPECT_FALSE(accountId.getAccountNum().has_value());
  EXPECT_EQ(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_TRUE(accountId.getEvmAddressAlias().has_value());
  EXPECT_EQ(accountId.getEvmAddressAlias()->toString(), getTestEvmAddressAlias().toString());
}

//-----
TEST_F(AccountIdTest, ConstructWithShardRealmAccountNum)
{
  const AccountId accountId(getTestShardNum(), getTestRealmNum(), getTestAccountNum());
  EXPECT_EQ(accountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(accountId.getRealmNum(), getTestRealmNum());
  EXPECT_TRUE(accountId.getAccountNum().has_value());
  EXPECT_EQ(*accountId.getAccountNum(), getTestAccountNum());
  EXPECT_EQ(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_FALSE(accountId.getEvmAddressAlias().has_value());

  EXPECT_THROW((AccountId{ getTestNumTooBig(), getTestRealmNum(), getTestAccountNum() }), std::invalid_argument);
  EXPECT_THROW((AccountId{ getTestShardNum(), getTestNumTooBig(), getTestAccountNum() }), std::invalid_argument);
  EXPECT_THROW((AccountId{ getTestShardNum(), getTestRealmNum(), getTestNumTooBig() }), std::invalid_argument);
}

//-----
TEST_F(AccountIdTest, ConstructWithShardRealmAccountAlias)
{
  const AccountId ed25519AliasAccountId(getTestShardNum(), getTestRealmNum(), getTestEd25519Alias());
  EXPECT_EQ(ed25519AliasAccountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(ed25519AliasAccountId.getRealmNum(), getTestRealmNum());
  EXPECT_FALSE(ed25519AliasAccountId.getAccountNum().has_value());
  EXPECT_NE(ed25519AliasAccountId.getPublicKeyAlias(), nullptr);
  EXPECT_EQ(ed25519AliasAccountId.getPublicKeyAlias()->toStringDer(), getTestEd25519Alias()->toStringDer());
  EXPECT_FALSE(ed25519AliasAccountId.getEvmAddressAlias().has_value());

  EXPECT_THROW((AccountId{ getTestNumTooBig(), getTestRealmNum(), getTestEd25519Alias() }), std::invalid_argument);
  EXPECT_THROW((AccountId{ getTestShardNum(), getTestNumTooBig(), getTestEd25519Alias() }), std::invalid_argument);

  const AccountId ecdsaAliasAccountId(getTestShardNum(), getTestRealmNum(), getTestEcdsaSecp256k1Alias());
  EXPECT_EQ(ecdsaAliasAccountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(ecdsaAliasAccountId.getRealmNum(), getTestRealmNum());
  EXPECT_FALSE(ecdsaAliasAccountId.getAccountNum().has_value());
  EXPECT_NE(ecdsaAliasAccountId.getPublicKeyAlias(), nullptr);
  EXPECT_EQ(ecdsaAliasAccountId.getPublicKeyAlias()->toStringDer(), getTestEcdsaSecp256k1Alias()->toStringDer());
  EXPECT_FALSE(ecdsaAliasAccountId.getEvmAddressAlias().has_value());

  EXPECT_THROW((AccountId{ getTestNumTooBig(), getTestRealmNum(), getTestEcdsaSecp256k1Alias() }),
               std::invalid_argument);
  EXPECT_THROW((AccountId{ getTestShardNum(), getTestNumTooBig(), getTestEcdsaSecp256k1Alias() }),
               std::invalid_argument);
}

//-----
TEST_F(AccountIdTest, ConstructWithShardRealmEvmAddress)
{
  const AccountId accountId(getTestShardNum(), getTestRealmNum(), getTestEvmAddressAlias());
  EXPECT_EQ(accountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(accountId.getRealmNum(), getTestRealmNum());
  EXPECT_FALSE(accountId.getAccountNum().has_value());
  EXPECT_EQ(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_TRUE(accountId.getEvmAddressAlias().has_value());
  EXPECT_EQ(accountId.getEvmAddressAlias()->toString(), getTestEvmAddressAlias().toString());

  EXPECT_THROW((AccountId{ getTestNumTooBig(), getTestRealmNum(), getTestAccountNum() }), std::invalid_argument);
  EXPECT_THROW((AccountId{ getTestShardNum(), getTestNumTooBig(), getTestAccountNum() }), std::invalid_argument);
}

//-----
TEST_F(AccountIdTest, CompareAccountIds)
{
  EXPECT_TRUE(AccountId() == AccountId());
  EXPECT_TRUE(AccountId(getTestAccountNum()) == AccountId(getTestAccountNum()));
  EXPECT_TRUE(AccountId(getTestEd25519Alias()) == AccountId(getTestEd25519Alias()));
  EXPECT_TRUE(AccountId(getTestEcdsaSecp256k1Alias()) == AccountId(getTestEcdsaSecp256k1Alias()));
  EXPECT_TRUE(AccountId(getTestEvmAddressAlias()) == AccountId(getTestEvmAddressAlias()));
  EXPECT_TRUE(AccountId(getTestShardNum(), getTestRealmNum(), getTestAccountNum()) ==
              AccountId(getTestShardNum(), getTestRealmNum(), getTestAccountNum()));
  EXPECT_TRUE(AccountId(getTestShardNum(), getTestRealmNum(), getTestEd25519Alias()) ==
              AccountId(getTestShardNum(), getTestRealmNum(), getTestEd25519Alias()));
  EXPECT_TRUE(AccountId(getTestShardNum(), getTestRealmNum(), getTestEcdsaSecp256k1Alias()) ==
              AccountId(getTestShardNum(), getTestRealmNum(), getTestEcdsaSecp256k1Alias()));
  EXPECT_TRUE(AccountId(getTestShardNum(), getTestRealmNum(), getTestEvmAddressAlias()) ==
              AccountId(getTestShardNum(), getTestRealmNum(), getTestEvmAddressAlias()));

  EXPECT_FALSE(AccountId(getTestAccountNum()) == AccountId(getTestAccountNum() - 1ULL));
  EXPECT_FALSE(AccountId(getTestEd25519Alias()) == AccountId(ED25519PrivateKey::generatePrivateKey()->getPublicKey()));
  EXPECT_FALSE(AccountId(getTestEcdsaSecp256k1Alias()) ==
               AccountId(ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey()));
  EXPECT_FALSE(AccountId(getTestEvmAddressAlias()) ==
               AccountId(EvmAddress::fromString("abcdef1234567890abcdef1234567890abcdef12")));

  EXPECT_FALSE(AccountId(getTestShardNum(), getTestRealmNum(), getTestAccountNum()) ==
               AccountId(getTestShardNum() - 1ULL, getTestRealmNum(), getTestAccountNum()));
  EXPECT_FALSE(AccountId(getTestShardNum(), getTestRealmNum(), getTestAccountNum()) ==
               AccountId(getTestShardNum(), getTestRealmNum() - 1ULL, getTestAccountNum()));

  EXPECT_FALSE(AccountId(getTestShardNum(), getTestRealmNum(), getTestAccountNum()) ==
               AccountId(getTestShardNum(), getTestRealmNum(), getTestEd25519Alias()));
  EXPECT_FALSE(AccountId(getTestShardNum(), getTestRealmNum(), getTestAccountNum()) ==
               AccountId(getTestShardNum(), getTestRealmNum(), getTestEcdsaSecp256k1Alias()));
  EXPECT_FALSE(AccountId(getTestShardNum(), getTestRealmNum(), getTestAccountNum()) ==
               AccountId(getTestShardNum(), getTestRealmNum(), getTestEvmAddressAlias()));
  EXPECT_FALSE(AccountId(getTestShardNum(), getTestRealmNum(), getTestEd25519Alias()) ==
               AccountId(getTestShardNum(), getTestRealmNum(), getTestEcdsaSecp256k1Alias()));
  EXPECT_FALSE(AccountId(getTestShardNum(), getTestRealmNum(), getTestEd25519Alias()) ==
               AccountId(getTestShardNum(), getTestRealmNum(), getTestEvmAddressAlias()));
  EXPECT_FALSE(AccountId(getTestShardNum(), getTestRealmNum(), getTestEcdsaSecp256k1Alias()) ==
               AccountId(getTestShardNum(), getTestRealmNum(), getTestEvmAddressAlias()));
}

//-----
TEST_F(AccountIdTest, ConstructFromString)
{
  const std::string testShardNumStr = std::to_string(getTestShardNum());
  const std::string testRealmNumStr = std::to_string(getTestRealmNum());
  const std::string testAccountNumStr = std::to_string(getTestAccountNum());
  const std::string testNumTooBigStr = std::to_string(getTestNumTooBig());

  AccountId accountId;
  EXPECT_NO_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + testRealmNumStr + '.' + testAccountNumStr));
  EXPECT_EQ(accountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(accountId.getRealmNum(), getTestRealmNum());
  EXPECT_TRUE(accountId.getAccountNum().has_value());
  EXPECT_EQ(*accountId.getAccountNum(), getTestAccountNum());

  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + testRealmNumStr + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString('.' + testShardNumStr + testRealmNumStr + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + testRealmNumStr + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + testRealmNumStr + '.' + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + testRealmNumStr + testAccountNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(".." + testShardNumStr + testRealmNumStr + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString('.' + testShardNumStr + '.' + testRealmNumStr + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString('.' + testShardNumStr + testRealmNumStr + '.' + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString('.' + testShardNumStr + testRealmNumStr + testAccountNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + ".." + testRealmNumStr + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + testRealmNumStr + testAccountNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + testRealmNumStr + ".." + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + testRealmNumStr + '.' + testAccountNumStr + '.'),
               std::invalid_argument);
  EXPECT_THROW(accountId =
                 AccountId::fromString('.' + testShardNumStr + '.' + testRealmNumStr + '.' + testAccountNumStr + '.'),
               std::invalid_argument);

  EXPECT_THROW(accountId = AccountId::fromString("abc"), std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString("o.o.e"), std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString("0.0.1!"), std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testNumTooBigStr + '.' + testRealmNumStr + '.' + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + testNumTooBigStr + '.' + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + testRealmNumStr + '.' + testNumTooBigStr),
               std::invalid_argument);

  const std::string ed25519AliasStr = getTestEd25519Alias()->toStringDer();
  EXPECT_NO_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + testRealmNumStr + '.' + ed25519AliasStr));
  EXPECT_EQ(accountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(accountId.getRealmNum(), getTestRealmNum());
  EXPECT_NE(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_EQ(accountId.getPublicKeyAlias()->toStringDer(), ed25519AliasStr);

  EXPECT_THROW(accountId = AccountId::fromString(ed25519AliasStr + '.' + testRealmNumStr + '.' + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + ed25519AliasStr + '.' + testAccountNumStr),
               std::invalid_argument);

  const std::string ecdsaAliasStr = getTestEcdsaSecp256k1Alias()->toStringDer();
  EXPECT_NO_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + testRealmNumStr + '.' + ecdsaAliasStr));
  EXPECT_EQ(accountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(accountId.getRealmNum(), getTestRealmNum());
  EXPECT_NE(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_EQ(accountId.getPublicKeyAlias()->toStringDer(), ecdsaAliasStr);

  EXPECT_THROW(accountId = AccountId::fromString(ecdsaAliasStr + '.' + testRealmNumStr + '.' + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + ecdsaAliasStr + '.' + testAccountNumStr),
               std::invalid_argument);

  const std::string evmAddressStr = getTestEvmAddressAlias().toString();
  EXPECT_NO_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + testRealmNumStr + '.' + evmAddressStr));
  EXPECT_EQ(accountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(accountId.getRealmNum(), getTestRealmNum());
  EXPECT_TRUE(accountId.getEvmAddressAlias());
  EXPECT_EQ(accountId.getEvmAddressAlias()->toString(), evmAddressStr);

  EXPECT_THROW(accountId = AccountId::fromString(evmAddressStr + '.' + testRealmNumStr + '.' + testAccountNumStr),
               std::invalid_argument);
  EXPECT_THROW(accountId = AccountId::fromString(testShardNumStr + '.' + evmAddressStr + '.' + testAccountNumStr),
               std::invalid_argument);
}

//-----
TEST_F(AccountIdTest, SetGetShardNum)
{
  AccountId accountId;
  accountId.setShardNum(getTestShardNum());

  EXPECT_EQ(accountId.getShardNum(), getTestShardNum());
  EXPECT_THROW(accountId.setShardNum(getTestNumTooBig()), std::invalid_argument);
}

//-----
TEST_F(AccountIdTest, SetGetRealmNum)
{
  AccountId accountId;
  accountId.setRealmNum(getTestRealmNum());

  EXPECT_EQ(accountId.getRealmNum(), getTestRealmNum());
  EXPECT_THROW(accountId.setRealmNum(getTestNumTooBig()), std::invalid_argument);
}

//-----
TEST_F(AccountIdTest, SetGetAccountNum)
{
  AccountId accountId;
  accountId.setAccountNum(getTestAccountNum());

  EXPECT_TRUE(accountId.getAccountNum().has_value());
  EXPECT_EQ(*accountId.getAccountNum(), getTestAccountNum());
  EXPECT_THROW(accountId.setAccountNum(getTestNumTooBig()), std::invalid_argument);
}

//-----
TEST_F(AccountIdTest, SetGetPublicKeyAlias)
{
  AccountId accountId;
  accountId.setPublicKeyAlias(getTestEd25519Alias());

  EXPECT_NE(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_EQ(accountId.getPublicKeyAlias()->toStringDer(), getTestEd25519Alias()->toStringDer());

  accountId.setPublicKeyAlias(getTestEcdsaSecp256k1Alias());

  EXPECT_NE(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_EQ(accountId.getPublicKeyAlias()->toStringDer(), getTestEcdsaSecp256k1Alias()->toStringDer());
}

//-----
TEST_F(AccountIdTest, SetGetEvmAddressAlias)
{
  AccountId accountId;
  accountId.setEvmAddressAlias(getTestEvmAddressAlias());

  EXPECT_TRUE(accountId.getEvmAddressAlias().has_value());
  EXPECT_EQ(accountId.getEvmAddressAlias()->toString(), getTestEvmAddressAlias().toString());
}

//-----
TEST_F(AccountIdTest, ResetMutuallyExclusiveAccountNumbers)
{
  AccountId accountId;

  accountId.setEvmAddressAlias(getTestEvmAddressAlias());
  accountId.setAccountNum(getTestAccountNum());
  EXPECT_EQ(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_FALSE(accountId.getEvmAddressAlias().has_value());

  accountId.setPublicKeyAlias(getTestEcdsaSecp256k1Alias());
  EXPECT_FALSE(accountId.getAccountNum().has_value());
  EXPECT_FALSE(accountId.getEvmAddressAlias().has_value());

  accountId.setEvmAddressAlias(getTestEvmAddressAlias());
  EXPECT_FALSE(accountId.getAccountNum().has_value());
  EXPECT_EQ(accountId.getPublicKeyAlias(), nullptr);
}

//-----
TEST_F(AccountIdTest, ProtobufAccountId)
{
  AccountId accountId;
  accountId.setShardNum(getTestShardNum());
  accountId.setRealmNum(getTestRealmNum());
  accountId.setAccountNum(getTestAccountNum());

  // Serialize shard, realm, account number
  std::unique_ptr<proto::AccountID> protoAccountId = accountId.toProtobuf();
  EXPECT_EQ(protoAccountId->shardnum(), getTestShardNum());
  EXPECT_EQ(protoAccountId->realmnum(), getTestRealmNum());
  EXPECT_EQ(protoAccountId->account_case(), proto::AccountID::AccountCase::kAccountNum);
  EXPECT_EQ(protoAccountId->accountnum(), getTestAccountNum());

  // Adjust protobuf fields
  const uint64_t adjustment = 3ULL;
  const uint64_t newShard = getTestShardNum() + adjustment;
  const uint64_t newRealm = getTestRealmNum() - adjustment;
  const uint64_t newAccount = getTestAccountNum() * adjustment;

  protoAccountId->set_shardnum(static_cast<int64_t>(newShard));
  protoAccountId->set_realmnum(static_cast<int64_t>(newRealm));
  protoAccountId->set_accountnum(static_cast<int64_t>(newAccount));

  // Deserialize shard, realm, account number
  accountId = AccountId::fromProtobuf(*protoAccountId);
  EXPECT_EQ(accountId.getShardNum(), newShard);
  EXPECT_EQ(accountId.getRealmNum(), newRealm);
  EXPECT_TRUE(accountId.getAccountNum().has_value());
  EXPECT_EQ(*accountId.getAccountNum(), newAccount);

  // Serialize ED25519 alias
  accountId.setPublicKeyAlias(getTestEd25519Alias());
  protoAccountId = accountId.toProtobuf();
  EXPECT_EQ(protoAccountId->account_case(), proto::AccountID::AccountCase::kAlias);

  // Adjust protobuf fields
  std::unique_ptr<PrivateKey> key = ED25519PrivateKey::generatePrivateKey();
  std::vector<std::byte> testBytes =
    internal::Utilities::stringToByteVector(key->getPublicKey()->toProtobufKey()->SerializeAsString());
  protoAccountId->set_alias(internal::Utilities::byteVectorToString(testBytes));

  // Deserialize ED25519 alias
  accountId = AccountId::fromProtobuf(*protoAccountId);
  EXPECT_NE(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_EQ(accountId.getPublicKeyAlias()->toBytesDer(), key->getPublicKey()->toBytesDer());

  // Serialize ECDSA alias
  accountId.setPublicKeyAlias(getTestEcdsaSecp256k1Alias());
  protoAccountId = accountId.toProtobuf();
  EXPECT_EQ(protoAccountId->account_case(), proto::AccountID::AccountCase::kAlias);

  // Adjust protobuf fields
  key = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  testBytes = internal::Utilities::stringToByteVector(key->getPublicKey()->toProtobufKey()->SerializeAsString());
  protoAccountId->set_alias(internal::Utilities::byteVectorToString(testBytes));

  // Deserialize ECDSA alias
  accountId = AccountId::fromProtobuf(*protoAccountId);
  EXPECT_NE(accountId.getPublicKeyAlias(), nullptr);
  EXPECT_EQ(accountId.getPublicKeyAlias()->toBytesDer(), key->getPublicKey()->toBytesDer());

  // Serialize EVM address
  accountId.setEvmAddressAlias(getTestEvmAddressAlias());
  protoAccountId = accountId.toProtobuf();
  EXPECT_EQ(protoAccountId->account_case(), proto::AccountID::AccountCase::kAlias);

  // Adjust protobuf fields
  testBytes = { std::byte('0'), std::byte('1'), std::byte('2'), std::byte('3'), std::byte('4'),
                std::byte('5'), std::byte('6'), std::byte('7'), std::byte('8'), std::byte('9'),
                std::byte('a'), std::byte('b'), std::byte('c'), std::byte('d'), std::byte('e'),
                std::byte('f'), std::byte('g'), std::byte('h'), std::byte('i'), std::byte('j') };
  protoAccountId->set_allocated_alias(new std::string(internal::Utilities::byteVectorToString(testBytes)));

  // Deserialize EVM address
  accountId = AccountId::fromProtobuf(*protoAccountId);
  EXPECT_TRUE(accountId.getEvmAddressAlias().has_value());
  EXPECT_EQ(accountId.getEvmAddressAlias()->toBytes(), testBytes);
}

//-----
TEST_F(AccountIdTest, ToString)
{
  AccountId accountId;
  EXPECT_EQ(accountId.toString(), "0.0.0");

  accountId.setShardNum(getTestShardNum());
  accountId.setRealmNum(getTestRealmNum());
  accountId.setAccountNum(getTestAccountNum());
  EXPECT_EQ(accountId.toString(),
            std::to_string(getTestShardNum()) + '.' + std::to_string(getTestRealmNum()) + '.' +
              std::to_string(getTestAccountNum()));

  accountId.setPublicKeyAlias(getTestEd25519Alias());
  EXPECT_EQ(accountId.toString(),
            std::to_string(getTestShardNum()) + '.' + std::to_string(getTestRealmNum()) + '.' +
              getTestEd25519Alias()->toStringDer());

  accountId.setPublicKeyAlias(getTestEcdsaSecp256k1Alias());
  EXPECT_EQ(accountId.toString(),
            std::to_string(getTestShardNum()) + '.' + std::to_string(getTestRealmNum()) + '.' +
              getTestEcdsaSecp256k1Alias()->toStringDer());

  accountId.setEvmAddressAlias(getTestEvmAddressAlias());
  EXPECT_EQ(accountId.toString(),
            std::to_string(getTestShardNum()) + '.' + std::to_string(getTestRealmNum()) + '.' +
              getTestEvmAddressAlias().toString());
}
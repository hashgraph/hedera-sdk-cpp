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

#include "BaseIntegrationTest.h"
#include "ECDSAsecp256k1PrivateKey.h"

#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace Hedera;

class ECDSAKeysCompatibilityIntegrationTest : public BaseIntegrationTest
{
protected:
  [[nodiscard]] inline const std::unordered_map<std::string_view, std::pair<std::string_view, std::string_view>>
  getExpectedPrivateKeyPairs() const
  {
    return expectedPrivateKeyPairs;
  };

private:
  const std::string_view unencryptedUncompressedPEMPrivateKey =
    "-----BEGIN EC PRIVATE "
    "KEY-----MHQCAQEEIG8I+jKi+"
    "iGVa7ttbfnlnML5AdvPugbgBWnseYjrle6qoAcGBSuBBAAKoUQDQgAEqf5BmMeBzkU1Ra9UAbZJo3tytVOlb7erTc36LRLP20mOLU7+mFY+"
    "3CfefAZgBtPXRAmDtRvYGODswAalW85GKA==-----END EC PRIVATE KEY-----";
  const std::string_view unencryptedCompressedPEMPrivateKey =
    "-----BEGIN EC PRIVATE "
    "KEY-----MFQCAQEEIOHyhclwHbha3f281Kvd884rhBzltxGJxCZyaQCagH9joAcGBSuBBAAKoSQDIgACREr6gFZa4K7hBP+bA25VdgQ+0ABFgM+"
    "g5RYw/W6T1Og=-----END EC PRIVATE KEY-----";
  const std::string_view legacyDERPrivateKey =
    "3030020100300706052b8104000a042204208c2cdc9575fe67493443967d74958fd7808a3787fd3337e99cfeebbc7566b586";
  const std::string_view openSSLCompatibleCompressedDERPrivateKey =
    "30540201010420ac318ea8ff8d991ab2f16172b4738e74dc35a56681199cfb1c0cb2e7cb560ffda00706052b8104000aa124032200036843f5"
    "cb338bbb4cdb21b0da4ea739d910951d6e8a5f703d313efe31afe788f4";
  const std::string_view openSSLCompatibleUncompressedDERPrivateKey =
    "307402010104208927647ad12b29646a1d051da8453462937bb2c813c6815cac6c0b720526ffc6a00706052b8104000aa14403420004aaac1c"
    "3ac1bea0245b8e00ce1e2018f9eab61b6331fbef7266f2287750a6597795f855ddcad2377e22259d1fcb4e0f1d35e8f2056300c15070bcbfce"
    "3759cc9d";
  const std::string_view openSSLCompatibleDERPrivateKeyWithoutPublicKey =
    "302e0201010420a6170a6aa6389a5bd3a3a8f9375f57bd91aa7f7d8b8b46ce0b702e000a21a5fea00706052b8104000a";

  const std::unordered_map<std::string_view, std::pair<std::string_view, std::string_view>> expectedPrivateKeyPairs{
    {unencryptedUncompressedPEMPrivateKey,
     { "6F08FA32A2FA21956BBB6D6DF9E59CC2F901DBCFBA06E00569EC7988EB95EEAA",
        "02A9FE4198C781CE453545AF5401B649A37B72B553A56FB7AB4DCDFA2D12CFDB49" }},
    { unencryptedCompressedPEMPrivateKey,
     { "E1F285C9701DB85ADDFDBCD4ABDDF3CE2B841CE5B71189C4267269009A807F63",
        "02444AFA80565AE0AEE104FF9B036E5576043ED0004580CFA0E51630FD6E93D4E8" }},
    { legacyDERPrivateKey,
     { "8C2CDC9575FE67493443967D74958FD7808A3787FD3337E99CFEEBBC7566B586",
        "028173079D2E996EF6B2D064FC82D5FC7094367211E28422BEC50A2F75C365F5FD" }},
    { openSSLCompatibleCompressedDERPrivateKey,
     { "AC318EA8FF8D991AB2F16172B4738E74DC35A56681199CFB1C0CB2E7CB560FFD",
        "036843F5CB338BBB4CDB21B0DA4EA739D910951D6E8A5F703D313EFE31AFE788F4" }},
    { openSSLCompatibleUncompressedDERPrivateKey,
     { "8927647AD12B29646A1D051DA8453462937BB2C813C6815CAC6C0B720526FFC6",
        "03AAAC1C3AC1BEA0245B8E00CE1E2018F9EAB61B6331FBEF7266F2287750A65977" }},
    { openSSLCompatibleDERPrivateKeyWithoutPublicKey,
     { "A6170A6AA6389A5BD3A3A8F9375F57BD91AA7F7D8B8B46CE0B702E000A21A5FE",
        "03B69A75A5DDB1C0747E995D47555019E5D8A28003AB5202BD92F534361FB4EC8A" }}
  };
};

//-----
TEST_F(ECDSAKeysCompatibilityIntegrationTest, ECPrivateKey)
{
  // Given
  auto expectedKeys = getExpectedPrivateKeyPairs();

  // When // Then
  for (auto key : expectedKeys)
  {
    auto actualKey = key.first;

    auto expectedPrivateKey = key.second.first;
    auto expectedPublicKey = key.second.second;

    auto actualResultKeyPair = ECDSAsecp256k1PrivateKey::fromString(actualKey);
    ASSERT_EQ(actualResultKeyPair->toStringRaw(), expectedPrivateKey);
    ASSERT_EQ(actualResultKeyPair->getPublicKey()->toStringRaw(), expectedPublicKey);
  }
}

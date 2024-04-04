/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "AccountCreateTransaction.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"

#include <dotenv.h>
#include <iostream>

#ifdef __ANDROID__
#include "AndroidHelper.h"
#include "CreateAccountExample.h"
#include <android/log.h>
#include <jni.h>
#endif

using namespace Hedera;

#ifdef __ANDROID__
void nativeMain()
{
  __android_log_print(ANDROID_LOG_INFO, "tag", __FUNCTION__);
#else
int main(int argc, char** argv)
{
  dotenv::init();
#endif

  const AccountId operatorAccountId = AccountId::fromString(std::getenv("OPERATOR_ID"));
  std::cout << "got operator id" << std::endl;
  const std::shared_ptr<PrivateKey> operatorPrivateKey = ED25519PrivateKey::fromString(std::getenv("OPERATOR_KEY"));
  std::cout << "got operator key" << std::endl;

  // Get a client for the Hedera testnet, and set the operator account ID and key such that all generated transactions
  // will be paid for by this account and be signed by this key.
  Client client = Client::forTestnet();
  client.setOperator(operatorAccountId, operatorPrivateKey);

  // Generate a ED25519 private, public key pair
  const std::unique_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();

  std::cout << "Generated private key: " << privateKey->toStringRaw() << std::endl;
  std::cout << "Generated public key: " << publicKey->toStringRaw() << std::endl;

  // Create a new account with an initial balance of 1000 tinybars. The only required field here is the key.
  TransactionResponse txResp =
    AccountCreateTransaction().setKey(publicKey).setInitialBalance(Hbar(1000ULL, HbarUnit::TINYBAR())).execute(client);

  // Get the receipt when it becomes available
  TransactionReceipt txReceipt = txResp.getReceipt(client);

  const AccountId newAccountId = txReceipt.mAccountId.value();
  std::cout << "Created new account with ID " << newAccountId.toString() << std::endl;

#ifdef __ANDROID__
  return;
#else
  return 0;
#endif
}

#ifdef __ANDROID__
JNIEXPORT void JNICALL
Java_com_hedera_hashgraph_sdk_cpp_examples_CreateAccountExample_createAccountExample(JNIEnv* env,
                                                                                     jobject,
                                                                                     jobject assetManager)
{
  Android::setAssetManager(AAssetManager_fromJava(env, assetManager));
  nativeMain();
}
#endif

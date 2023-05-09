set(TEST_PROJECT_NAME ${PROJECT_NAME}-unit-tests)
add_executable(${TEST_PROJECT_NAME}
        AccountAllowanceApproveTransactionTest.cc
        AccountAllowanceDeleteTransactionTest.cc
        AccountBalanceQueryTest.cc
        AccountBalanceTest.cc
        AccountCreateTransactionTest.cc
        AccountDeleteTransactionTest.cc
        AccountIdTest.cc
        AccountInfoQueryTest.cc
        AccountInfoTest.cc
        AccountRecordsQueryTest.cc
        AccountRecordsTest.cc
        AccountUpdateTransactionTest.cc
        ClientTest.cc
        ContractByteCodeQueryTest.cc
        ContractCallQueryTest.cc
        ContractCreateTransactionTest.cc
        ContractDeleteTransactionTest.cc
        ContractFunctionParametersTest.cc
        ContractFunctionResultTest.cc
        ContractFunctionSelectorTest.cc
        ContractIdTest.cc
        ContractLogInfoTest.cc
        ECDSAsecp256k1PrivateKeyTest.cc
        ECDSAsecp256k1PublicKeyTest.cc
        ED25519PrivateKeyTest.cc
        ED25519PublicKeyTest.cc
        EthereumTransactionTest.cc
        EvmAddressTest.cc
        ExchangeRateTest.cc
        FileCreateTransactionTest.cc
        FileIdTest.cc
        HbarAllowanceTest.cc
        HbarTest.cc
        HbarTransferTest.cc
        LedgerIdTest.cc
        NftIdTest.cc
        NodeAddressTest.cc
        StakingInfoTest.cc
        TokenAllowanceTest.cc
        TokenIdTest.cc
        TokenNftAllowanceTest.cc
        TokenNftRemoveAllowanceTest.cc
        TokenNftTransferTest.cc
        TokenTransferTest.cc
        TransactionIdTest.cc
        TransactionReceiptQueryTest.cc
        TransactionReceiptTest.cc
        TransactionRecordQueryTest.cc
        TransactionRecordTest.cc
        TransactionResponseTest.cc
        TransactionTest.cc
        TransferTransactionTest.cc

        vectors/BIP39TestVectors.cc
        vectors/Hedera12WordMnemonicBIP39TestVectors.cc
        vectors/Hedera24WordMnemonicBIP39TestVectors.cc
        vectors/RFC8032TestVectors.cc
        vectors/SLIP10TestVectors.cc)

target_link_libraries(${TEST_PROJECT_NAME} PRIVATE gtest_main gtest ${PROJECT_NAME})
gtest_discover_tests(${TEST_PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

target_link_libraries(${TEST_PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

file(COPY ${PROJECT_SOURCE_DIR}/addressbook/previewnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
file(COPY ${PROJECT_SOURCE_DIR}/addressbook/testnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
file(COPY ${PROJECT_SOURCE_DIR}/addressbook/mainnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

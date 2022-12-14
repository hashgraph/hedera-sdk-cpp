include(FetchContent)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1)
FetchContent_MakeAvailable(googletest)

set(TestSources
        AccountBalanceQueryTest.cc
        AccountCreateTransactionTest.cc
        AccountIdTest.cc
        ClientTest.cc
        ContractIdTest.cc
        ECDSAPrivateKeyTest.cc
        ED25519PrivateKeyTest.cc
        ED25519PublicKeyTest.cc
        ExchangeRateTest.cc
        HbarTest.cc
        MnemonicBIP39Test.cc
        TransactionIdTest.cc
        TransactionReceiptQueryTest.cc
        TransactionReceiptTest.cc
        TransactionRecordQueryTest.cc
        TransactionRecordTest.cc
        TransactionResponseTest.cc
        TransferTest.cc
        TransferTransactionTest.cc
        )

set(TEST_PROJECT_NAME ${PROJECT_NAME}-tests)

add_executable(${TEST_PROJECT_NAME} ${TestSources})

message(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
message(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${PROJECT_SOURCE_DIR}/addressbook/testnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/release)
file(COPY ${PROJECT_SOURCE_DIR}/addressbook/mainnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/release)

file(COPY ${PROJECT_SOURCE_DIR}/wordlists/bip39-english.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/release)
file(COPY ${PROJECT_SOURCE_DIR}/wordlists/legacy-english.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/release)

target_link_libraries(${TEST_PROJECT_NAME}
        PRIVATE
        gtest_main
        ${PROJECT_NAME})

add_test(
        NAME ${TEST_PROJECT_NAME}
        COMMAND ${TEST_PROJECT_NAME})
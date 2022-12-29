include(FetchContent)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(googletest)

set(TEST_PROJECT_NAME ${PROJECT_NAME}-tests)
add_executable(${TEST_PROJECT_NAME}
        AccountBalanceQueryTest.cc
        AccountCreateTransactionTest.cc
        AccountIdTest.cc
        ClientTest.cc
        ContractIdTest.cc
        ECDSAPrivateKeyTest.cc
        ECDSAPublicKeyTest.cc
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
        TransferTransactionTest.cc)
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE gtest_main ${PROJECT_NAME})
gtest_discover_tests(${TEST_PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/release)

file(COPY ${PROJECT_SOURCE_DIR}/addressbook/testnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/release)
file(COPY ${PROJECT_SOURCE_DIR}/addressbook/mainnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/release)

file(COPY ${PROJECT_SOURCE_DIR}/wordlists/bip39-english.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/release)
file(COPY ${PROJECT_SOURCE_DIR}/wordlists/legacy-english.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/release)

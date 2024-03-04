set(TEST_PROJECT_NAME ${PROJECT_NAME}-integration-tests)
add_executable(${TEST_PROJECT_NAME}
        AccountAllowanceApproveTransactionIntegrationTests.cc
        AccountBalanceQueryIntegrationTests.cc
        AccountCreateTransactionIntegrationTests.cc
        AccountDeleteTransactionIntegrationTests.cc
        AccountInfoQueryIntegrationTests.cc
        AccountRecordsQueryIntegrationTests.cc
        AccountStakersQueryIntegrationTests.cc
        AccountUpdateTransactionIntegrationTests.cc
        AddressBookQueryIntegrationTests.cc
        BaseIntegrationTest.cc
        ChunkedTransactionIntegrationTests.cc
        ClientIntegrationTests.cc
        ContractBytecodeQueryIntegrationTests.cc
        ContractCallQueryIntegrationTests.cc
        ContractCreateFlowIntegrationTests.cc
        ContractCreateTransactionIntegrationTests.cc
        ContractDeleteTransactionIntegrationTests.cc
        ContractExecuteTransactionIntegrationTests.cc
        ContractNonceInfoIntegrationTests.cc
        ContractUpdateTransactionIntegrationTests.cc
        ContractInfoQueryIntegrationTests.cc
        EthereumTransactionIntegrationTests.cc
        ExecutableIntegrationTests.cc
        FileAppendTransactionIntegrationTests.cc
        FileContentsQueryIntegrationTests.cc
        FileCreateTransactionIntegrationTests.cc
        FileDeleteTransactionIntegrationTests.cc
        FileInfoQueryIntegrationTests.cc
        FileUpdateTransactionIntegrationTests.cc
        FreezeTransactionIntegrationTests.cc
        HttpClientIntegrationTests.cc
        JSONIntegrationTests.cc
        NetworkVersionInfoQueryIntegrationTests.cc
        PrngTransactionIntegrationTests.cc
        QueryIntegrationTests.cc
        ScheduleCreateTransactionIntegrationTests.cc
        ScheduleDeleteTransactionIntegrationTests.cc
        ScheduleInfoQueryIntegrationTests.cc
        ScheduleSignTransactionIntegrationTests.cc
        SystemDeleteTransactionIntegrationTests.cc
        SystemUndeleteTransactionIntegrationTests.cc
        TokenAssociateTransactionIntegrationTests.cc
        TokenBurnTransactionIntegrationTests.cc
        TokenCreateTransactionIntegrationTests.cc
        TokenDeleteTransactionIntegrationTests.cc
        TokenDissociateTransactionIntegrationTests.cc
        TokenFeeScheduleUpdateTransactionIntegrationTests.cc
        TokenFreezeTransactionIntegrationTests.cc
        TokenGrantKycTransactionIntegrationTests.cc
        TokenInfoQueryIntegrationTests.cc
        TokenMintTransactionIntegrationTests.cc
        TokenNftInfoQueryIntegrationTests.cc
        TokenPauseTransactionIntegrationTests.cc
        TokenRevokeKycTransactionIntegrationTests.cc
        TokenUnfreezeTransactionIntegrationTests.cc
        TokenUnpauseTransactionIntegrationTests.cc
        TokenUpdateTransactionIntegrationTests.cc
        TokenWipeTransactionIntegrationTests.cc
        TopicCreateTransactionIntegrationTests.cc
        TopicDeleteTransactionIntegrationTests.cc
        TopicInfoQueryIntegrationTests.cc
        TopicMessageQueryIntegrationTests.cc
        TopicMessageSubmitTransactionIntegrationTests.cc
        TopicUpdateTransactionIntegrationTests.cc
        TransactionIntegrationTests.cc
        TransactionReceiptIntegrationTests.cc
        TransactionReceiptQueryIntegrationTests.cc
        TransactionRecordIntegrationTests.cc
        TransactionRecordQueryIntegrationTests.cc
        TransferTransactionIntegrationTests.cc)

# gtest_main is put in a manual-link directory within the vcpkg installation
target_link_directories(${TEST_PROJECT_NAME} PRIVATE ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/manual-link)
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE GTest::gtest_main GTest::gtest ${PROJECT_NAME})
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE CURL::libcurl)
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Do not discover tests for cross-compile builds
if (NOT ANDROID)
    gtest_discover_tests(${TEST_PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
endif ()

# Install gRPC's roots.pem file for Windows with the integration tests so that it can be easily referenced.
if (WIN32)
    file(COPY ${PROJECT_SOURCE_DIR}/vcpkg/packages/grpc_x64-windows/share/grpc/roots.pem
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
endif ()

file(COPY ${PROJECT_SOURCE_DIR}/config/local_node.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
file(COPY ${PROJECT_SOURCE_DIR}/config/hello_world.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

set(TEST_PROJECT_NAME ${PROJECT_NAME}-integration-tests)
add_executable(${TEST_PROJECT_NAME}
        AccountAllowanceApproveTransactionIntegrationTests.cc
        AccountBalanceQueryIntegrationTest.cc
        AccountCreateTransactionIntegrationTest.cc
        AccountDeleteTransactionIntegrationTest.cc
        AccountInfoQueryIntegrationTests.cc
        AccountRecordsQueryIntegrationTests.cc
        AccountUpdateTransactionIntegrationTest.cc
        BaseIntegrationTest.cc
        ContractBytecodeQueryIntegrationTests.cc
        ContractCallQueryIntegrationTests.cc
        ContractCreateFlowIntegrationTest.cc
        ContractCreateTransactionIntegrationTests.cc
        ContractDeleteTransactionIntegrationTests.cc
        ContractExecuteTransactionIntegrationTests.cc
        ContractUpdateTransactionIntegrationTests.cc
        ClientIntegrationTest.cc
        ContractInfoQueryIntegrationTests.cc
        FileAppendTransactionIntegrationTests.cc
        FileContentsQueryIntegrationTest.cc
        FileCreateTransactionIntegrationTests.cc
        FileDeleteTransactionIntegrationTests.cc
        FileInfoQueryIntegrationTests.cc
        FileUpdateTransactionIntegrationTests.cc
        JSONIntegrationTest.cc
        TokenAssociateTransactionIntegrationTests.cc
        TokenCreateTransactionIntegrationTests.cc
        TokenDeleteTransactionIntegrationTests.cc
        TransactionIntegrationTest.cc
        TransactionReceiptIntegrationTest.cc
        TransactionReceiptQueryIntegrationTest.cc
        TransactionRecordIntegrationTest.cc
        TransactionRecordQueryIntegrationTest.cc
        TransferTransactionIntegrationTest.cc)

target_link_libraries(${TEST_PROJECT_NAME} PRIVATE gtest_main gtest ${PROJECT_NAME})
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
gtest_discover_tests(${TEST_PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

# Install gRPC's roots.pem file for Windows with the integration tests so that it can be easily referenced.
if (WIN32)
    file(COPY ${PROJECT_SOURCE_DIR}/vcpkg/packages/grpc_x64-windows/share/grpc/roots.pem
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
endif ()

file(COPY ${PROJECT_SOURCE_DIR}/config/local_node.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
file(COPY ${PROJECT_SOURCE_DIR}/config/hello_world.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})


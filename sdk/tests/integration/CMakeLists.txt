set(TEST_PROJECT_NAME ${PROJECT_NAME}-integration-tests)
add_executable(${TEST_PROJECT_NAME}
        AccountAllowanceApproveTransactionIntegrationTests.cc
        AccountBalanceQueryIntegrationTest.cc
        AccountCreateTransactionIntegrationTest.cc
        AccountDeleteTransactionIntegrationTest.cc
        AccountInfoQueryIntegrationTests.cc
        AccountUpdateTransactionIntegrationTest.cc
        ClientIntegrationTest.cc
        JSONIntegrationTest.cc
        TransactionIntegrationTest.cc
        TransactionReceiptQueryIntegrationTest.cc
        TransactionRecordQueryIntegrationTest.cc
        TransferTransactionIntegrationTest.cc)

target_link_libraries(${TEST_PROJECT_NAME} PRIVATE gtest_main gtest ${PROJECT_NAME})
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
gtest_discover_tests(${TEST_PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

file(COPY ${PROJECT_SOURCE_DIR}/config/local_node.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

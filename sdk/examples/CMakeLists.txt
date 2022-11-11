set(CREATE_ACCOUNT_EXAMPLE_NAME ${PROJECT_NAME}-create-account-example)
set(GET_ACCOUNT_BALANCE_EXAMPLE_NAME ${PROJECT_NAME}-get-account-balance-example)
set(TRANSFER_CRYPTO_EXAMPLE_NAME ${PROJECT_NAME}-transfer-crypto-example)

add_executable(${CREATE_ACCOUNT_EXAMPLE_NAME} CreateAccountExample.cc)
add_executable(${GET_ACCOUNT_BALANCE_EXAMPLE_NAME} GetAccountBalanceExample.cc)
add_executable(${TRANSFER_CRYPTO_EXAMPLE_NAME} TransferCryptoExample.cc)

file(COPY ${PROJECT_SOURCE_DIR}/addressbook/testnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/release)
file(COPY ${PROJECT_SOURCE_DIR}/addressbook/mainnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/release)

target_link_libraries(${CREATE_ACCOUNT_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${GET_ACCOUNT_BALANCE_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${TRANSFER_CRYPTO_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})

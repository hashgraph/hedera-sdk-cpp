set(ACCOUNT_ALLOWANCE_EXAMPLE_NAME ${PROJECT_NAME}-account-allowance-example)
set(CREATE_ACCOUNT_EXAMPLE_NAME ${PROJECT_NAME}-create-account-example)
set(CREATE_SIMPLE_CONTRACT_EXAMPLE_NAME ${PROJECT_NAME}-create-simple-contract-example)
set(CREATE_STATEFUL_CONTRACT_EXAMPLE_NAME ${PROJECT_NAME}-create-stateful-contract-example)
set(DELETE_ACCOUNT_EXAMPLE_NAME ${PROJECT_NAME}-delete-account-example)
set(DELETE_FILE_EXAMPLE_NAME ${PROJECT_NAME}-delete-file-example)
set(FILE_APPEND_CHUNKED_EXAMPLE_NAME ${PROJECT_NAME}-file-append-chunked-example)
set(GENERATE_PRIVATE_KEY_FROM_MNEMONIC_EXAMPLE_NAME ${PROJECT_NAME}-generate-private-key-from-mnemonic-example)
set(GET_ACCOUNT_BALANCE_EXAMPLE_NAME ${PROJECT_NAME}-get-account-balance-example)
set(GET_FILE_CONTENTS_EXAMPLE_NAME ${PROJECT_NAME}-get-file-contents-example)
set(STAKING_EXAMPLE_NAME ${PROJECT_NAME}-staking-example)
set(TRANSFER_CRYPTO_EXAMPLE_NAME ${PROJECT_NAME}-transfer-crypto-example)
set(TRANSFER_TOKENS_EXAMPLE_NAME ${PROJECT_NAME}-transfer-tokens-example)
set(UPDATE_ACCOUNT_PUBLIC_KEY_EXAMPLE_NAME ${PROJECT_NAME}-update-account-public-key-example)

add_executable(${ACCOUNT_ALLOWANCE_EXAMPLE_NAME} AccountAllowanceExample.cc)
add_executable(${CREATE_ACCOUNT_EXAMPLE_NAME} CreateAccountExample.cc)
add_executable(${CREATE_SIMPLE_CONTRACT_EXAMPLE_NAME} CreateSimpleContractExample.cc)
add_executable(${CREATE_STATEFUL_CONTRACT_EXAMPLE_NAME} CreateStatefulContractExample.cc)
add_executable(${DELETE_ACCOUNT_EXAMPLE_NAME} DeleteAccountExample.cc)
add_executable(${DELETE_FILE_EXAMPLE_NAME} DeleteFileExample.cc)
add_executable(${FILE_APPEND_CHUNKED_EXAMPLE_NAME} FileAppendChunkedExample.cc)
add_executable(${GENERATE_PRIVATE_KEY_FROM_MNEMONIC_EXAMPLE_NAME} GeneratePrivateKeyFromMnemonic.cc)
add_executable(${GET_ACCOUNT_BALANCE_EXAMPLE_NAME} GetAccountBalanceExample.cc)
add_executable(${GET_FILE_CONTENTS_EXAMPLE_NAME} GetFileContentsExample.cc)
add_executable(${STAKING_EXAMPLE_NAME} StakingExample.cc)
add_executable(${TRANSFER_CRYPTO_EXAMPLE_NAME} TransferCryptoExample.cc)
add_executable(${TRANSFER_TOKENS_EXAMPLE_NAME} TransferTokensExample.cc)
add_executable(${UPDATE_ACCOUNT_PUBLIC_KEY_EXAMPLE_NAME} UpdateAccountPublicKeyExample.cc)

file(COPY ${PROJECT_SOURCE_DIR}/addressbook/mainnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
file(COPY ${PROJECT_SOURCE_DIR}/addressbook/previewnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
file(COPY ${PROJECT_SOURCE_DIR}/addressbook/testnet.pb
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

file(COPY ${PROJECT_SOURCE_DIR}/config/hello_world.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
file(COPY ${PROJECT_SOURCE_DIR}/config/stateful.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

# Install gRPC's roots.pem file for Windows with the examples so that it can be easily referenced.
if (WIN32)
    file(COPY ${PROJECT_SOURCE_DIR}/vcpkg/packages/grpc_x64-windows/share/grpc/roots.pem
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
endif ()

target_link_libraries(${ACCOUNT_ALLOWANCE_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${CREATE_ACCOUNT_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${CREATE_SIMPLE_CONTRACT_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${CREATE_STATEFUL_CONTRACT_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${DELETE_ACCOUNT_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${DELETE_FILE_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${FILE_APPEND_CHUNKED_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${GENERATE_PRIVATE_KEY_FROM_MNEMONIC_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${GET_ACCOUNT_BALANCE_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${GET_FILE_CONTENTS_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${STAKING_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${TRANSFER_CRYPTO_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${TRANSFER_TOKENS_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})
target_link_libraries(${UPDATE_ACCOUNT_PUBLIC_KEY_EXAMPLE_NAME} PUBLIC ${PROJECT_NAME})

target_link_libraries(${CREATE_SIMPLE_CONTRACT_EXAMPLE_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${CREATE_STATEFUL_CONTRACT_EXAMPLE_NAME} PRIVATE nlohmann_json::nlohmann_json)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE} DESTINATION examples FILES_MATCHING PATTERN *)

install(TARGETS
        ${ACCOUNT_ALLOWANCE_EXAMPLE_NAME}
        ${CREATE_ACCOUNT_EXAMPLE_NAME}
        ${CREATE_SIMPLE_CONTRACT_EXAMPLE_NAME}
        ${CREATE_STATEFUL_CONTRACT_EXAMPLE_NAME}
        ${DELETE_ACCOUNT_EXAMPLE_NAME}
        ${DELETE_FILE_EXAMPLE_NAME}
        ${FILE_APPEND_CHUNKED_EXAMPLE_NAME}
        ${GENERATE_PRIVATE_KEY_FROM_MNEMONIC_EXAMPLE_NAME}
        ${GET_ACCOUNT_BALANCE_EXAMPLE_NAME}
        ${GET_FILE_CONTENTS_EXAMPLE_NAME}
        ${STAKING_EXAMPLE_NAME}
        ${TRANSFER_CRYPTO_EXAMPLE_NAME}
        ${TRANSFER_TOKENS_EXAMPLE_NAME}
        ${UPDATE_ACCOUNT_PUBLIC_KEY_EXAMPLE_NAME}
        RUNTIME DESTINATION examples)

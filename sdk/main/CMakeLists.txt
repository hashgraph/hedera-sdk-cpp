add_library(${PROJECT_NAME} STATIC
    src/AccountAllowanceApproveTransaction.cc
    src/AccountAllowanceDeleteTransaction.cc
    src/AccountBalance.cc
    src/AccountBalanceQuery.cc
    src/AccountCreateTransaction.cc
    src/AccountDeleteTransaction.cc
    src/AccountId.cc
    src/AccountInfo.cc
    src/AccountInfoQuery.cc
    src/AccountRecordsQuery.cc
    src/AccountStakersQuery.cc
    src/AccountUpdateTransaction.cc
    src/ContractByteCodeQuery.cc
    src/ContractCallQuery.cc
    src/ContractCreateTransaction.cc
    src/ContractDeleteTransaction.cc
    src/ContractExecuteTransaction.cc
    src/ContractFunctionResult.cc
    src/ContractInfo.cc
    src/ContractInfoQuery.cc
    src/ContractUpdateTransaction.cc
    src/ED25519Keypair.cc
    src/ED25519PrivateKey.cc
    src/ED25519PublicKey.cc
    src/EthereumTransaction.cc
    src/FileAppendTransaction.cc
    src/FileContentsQuery.cc
    src/FileCreateTransaction.cc
    src/HbarUnit.cc

    src/helper/DurationConverter.cc
    src/helper/InstantConverter.cc)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC ${HAPI_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} hapi)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSIONS 0.1.0)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
find_package(Sodium REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${sodium_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${sodium_LIBRARY_RELEASE})

# Install Library & Headers
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
install(DIRECTORY include/ TYPE INCLUDE FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${HAPI_INCLUDE_DIR}/ TYPE INCLUDE)
install(DIRECTORY ${HAPI_LIB_DIR}/ TYPE LIB)
install(DIRECTORY ${Protobuf_INCLUDE_DIRS} DESTINATION ${CMAKE_INSTALL_PREFIX})
add_library(${PROJECT_NAME} STATIC
        #    src/AccountAllowanceApproveTransaction.cc
        #    src/AccountAllowanceDeleteTransaction.cc
        src/AccountBalance.cc
        src/AccountBalanceQuery.cc
        #src/AccountCreateTransaction.cc
        #    src/AccountDeleteTransaction.cc
        src/AccountId.cc
        #    src/AccountInfo.cc
        #    src/AccountInfoQuery.cc
        #    src/AccountRecordsQuery.cc
        #    src/AccountStakersQuery.cc
        #    src/AccountUpdateTransaction.cc
        src/Channel.cc
        src/Client.cc
        #    src/ContractByteCodeQuery.cc
        #    src/ContractCallQuery.cc
        #    src/ContractCreateTransaction.cc
        #    src/ContractDeleteTransaction.cc
        #    src/ContractExecuteTransaction.cc
        #    src/ContractFunctionResult.cc
        #    src/ContractInfo.cc
        #    src/ContractInfoQuery.cc
        #    src/ContractUpdateTransaction.cc
        src/ED25519Keypair.cc
        src/ED25519PublicKey.cc
        #    src/EthereumTransaction.cc
        src/EvmAddress.cc
        src/ExchangeRate.cc
        src/Executable.cc
        #    src/FileAppendTransaction.cc
        #    src/FileContentsQuery.cc
        #    src/FileCreateTransaction.cc
        src/HbarUnit.cc
        src/Network.cc
        src/Node.cc
        src/NodeAddress.cc
        src/PublicKey.cc
        src/Transaction.cc
        src/TransactionId.cc

        src/helper/DurationConverter.cc
        src/helper/TimestampConverter.cc
        src/helper/HexConverter.cc)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC ${HAPI_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} hapi)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSIONS 0.1.0)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

# Install Library & Headers
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
install(DIRECTORY include/ TYPE INCLUDE FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${HAPI_INCLUDE_DIR}/ TYPE INCLUDE)
install(DIRECTORY ${HAPI_LIB_DIR}/ TYPE LIB)
install(DIRECTORY ${Protobuf_INCLUDE_DIRS} DESTINATION ${CMAKE_INSTALL_PREFIX})
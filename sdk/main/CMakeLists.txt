include(FetchContent)

FetchContent_Declare(protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG        v3.21.7)
FetchContent_MakeAvailable(protobuf)

add_library(${PROJECT_NAME} SHARED
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

include_directories(${PROJECT_NAME} PUBLIC include)
include_directories(${PROJECT_NAME} PRIVATE ../generated/include)
include_directories(${protobuf_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} libprotobuf)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSIONS 0.1.0)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
find_package(Sodium REQUIRED)
include_directories(${sodium_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${sodium_LIBRARY_RELEASE})
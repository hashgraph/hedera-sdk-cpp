cmake_minimum_required(VERSION 3.24.1)
project(hedera-sdk-cpp VERSION 0.1.0 DESCRIPTION "Hedera SDK C++")

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
set (CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} SHARED
    sdk/generated/src/basic_types.pb.cc
    sdk/generated/src/consensus_create_topic.pb.cc
    sdk/generated/src/consensus_delete_topic.pb.cc
    sdk/generated/src/consensus_get_topic_info.pb.cc
    sdk/generated/src/consensus_service.pb.cc
    sdk/generated/src/consensus_submit_message.pb.cc
    sdk/generated/src/consensus_topic_info.pb.cc
    sdk/generated/src/consensus_update_topic.pb.cc
    sdk/generated/src/contract_call_local.pb.cc
    sdk/generated/src/contract_call.pb.cc
    sdk/generated/src/contract_create.pb.cc
    sdk/generated/src/contract_delete.pb.cc
    sdk/generated/src/contract_get_bytecode.pb.cc
    sdk/generated/src/contract_get_info.pb.cc
    sdk/generated/src/contract_get_records.pb.cc
    sdk/generated/src/contract_update.pb.cc
    sdk/generated/src/crypto_add_live_hash.pb.cc
    sdk/generated/src/crypto_approve_allowance.pb.cc
    sdk/generated/src/crypto_create.pb.cc
    sdk/generated/src/crypto_delete_allowance.pb.cc
    sdk/generated/src/crypto_delete_live_hash.pb.cc
    sdk/generated/src/crypto_delete.pb.cc
    sdk/generated/src/crypto_get_account_balance.pb.cc
    sdk/generated/src/crypto_get_account_records.pb.cc
    sdk/generated/src/crypto_get_info.pb.cc
    sdk/generated/src/crypto_get_live_hash.pb.cc
    sdk/generated/src/crypto_get_stakers.pb.cc
    sdk/generated/src/crypto_service.pb.cc
    sdk/generated/src/crypto_transfer.pb.cc
    sdk/generated/src/crypto_update.pb.cc
    sdk/generated/src/custom_fees.pb.cc
    sdk/generated/src/duration.pb.cc
    sdk/generated/src/ethereum_transaction.pb.cc
    sdk/generated/src/exchange_rate.pb.cc
    sdk/generated/src/file_append.pb.cc
    sdk/generated/src/file_create.pb.cc
    sdk/generated/src/file_delete.pb.cc
    sdk/generated/src/file_get_contents.pb.cc
    sdk/generated/src/file_get_info.pb.cc
    sdk/generated/src/file_service.pb.cc
    sdk/generated/src/file_update.pb.cc
    sdk/generated/src/freeze_service.pb.cc
    sdk/generated/src/freeze_type.pb.cc
    sdk/generated/src/freeze.pb.cc
    sdk/generated/src/get_account_details.pb.cc
    sdk/generated/src/get_by_key.pb.cc
    sdk/generated/src/get_by_solidity_id.pb.cc
    sdk/generated/src/network_get_execution_time.pb.cc
    sdk/generated/src/network_get_version_info.pb.cc
    sdk/generated/src/network_service.pb.cc
    sdk/generated/src/node_stake_update.pb.cc
    sdk/generated/src/query_header.pb.cc
    sdk/generated/src/query.pb.cc
    sdk/generated/src/response_code.pb.cc
    sdk/generated/src/response_header.pb.cc
    sdk/generated/src/response.pb.cc
    sdk/generated/src/schedulable_transaction_body.pb.cc
    sdk/generated/src/schedule_create.pb.cc
    sdk/generated/src/schedule_delete.pb.cc
    sdk/generated/src/schedule_get_info.pb.cc
    sdk/generated/src/schedule_service.pb.cc
    sdk/generated/src/schedule_sign.pb.cc
    sdk/generated/src/smart_contract_service.pb.cc
    sdk/generated/src/system_delete.pb.cc
    sdk/generated/src/system_undelete.pb.cc
    sdk/generated/src/throttle_definitions.pb.cc
    sdk/generated/src/timestamp.pb.cc
    sdk/generated/src/token_associate.pb.cc
    sdk/generated/src/token_burn.pb.cc
    sdk/generated/src/token_create.pb.cc
    sdk/generated/src/token_delete.pb.cc
    sdk/generated/src/token_dissociate.pb.cc
    sdk/generated/src/token_fee_schedule_update.pb.cc
    sdk/generated/src/token_freeze_account.pb.cc
    sdk/generated/src/token_get_account_nft_infos.pb.cc
    sdk/generated/src/token_get_info.pb.cc
    sdk/generated/src/token_get_nft_info.pb.cc
    sdk/generated/src/token_get_nft_infos.pb.cc
    sdk/generated/src/token_grant_kyc.pb.cc
    sdk/generated/src/token_mint.pb.cc
    sdk/generated/src/token_pause.pb.cc
    sdk/generated/src/token_revoke_kyc.pb.cc
    sdk/generated/src/token_service.pb.cc
    sdk/generated/src/token_unfreeze_account.pb.cc
    sdk/generated/src/token_unpause.pb.cc
    sdk/generated/src/token_update.pb.cc
    sdk/generated/src/token_wipe_account.pb.cc
    sdk/generated/src/transaction_body.pb.cc
    sdk/generated/src/transaction_contents.pb.cc
    sdk/generated/src/transaction_get_fast_record.pb.cc
    sdk/generated/src/transaction_get_receipt.pb.cc
    sdk/generated/src/transaction_get_record.pb.cc
    sdk/generated/src/transaction_receipt.pb.cc
    sdk/generated/src/transaction_record.pb.cc
    sdk/generated/src/transaction_response.pb.cc
    sdk/generated/src/transaction.pb.cc
    sdk/generated/src/unchecked_submit.pb.cc
    sdk/generated/src/util_prng.pb.cc
    sdk/generated/src/util_service.pb.cc
    
    sdk/main/src/AccountAllowanceApproveTransaction.cc
    sdk/main/src/AccountAllowanceDeleteTransaction.cc
    sdk/main/src/AccountBalance.cc
    sdk/main/src/AccountBalanceQuery.cc
    sdk/main/src/AccountCreateTransaction.cc
    sdk/main/src/AccountDeleteTransaction.cc
    sdk/main/src/AccountId.cc
    sdk/main/src/AccountInfo.cc
    sdk/main/src/HbarUnit.cc
    
    sdk/main/src/helper/DurationConverter.cc
    sdk/main/src/helper/InstantConverter.cc)

include_directories(${PROJECT_NAME} PRIVATE
    sdk/generated/include
    sdk/main/include)

target_link_libraries(${PROJECT_NAME} ${Protobuf_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSIONS 0.1.0)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER sdk/main/include/HbarUnit.h)

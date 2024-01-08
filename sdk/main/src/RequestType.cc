/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "RequestType.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
const std::unordered_map<proto::HederaFunctionality, RequestType> gProtobufHederaFunctionalityToRequestType = {
  {proto::HederaFunctionality::NONE,                        RequestType::NONE                         },
  { proto::HederaFunctionality::CryptoTransfer,             RequestType::CRYPTO_TRANSFER              },
  { proto::HederaFunctionality::CryptoUpdate,               RequestType::CRYPTO_UPDATE                },
  { proto::HederaFunctionality::CryptoDelete,               RequestType::CRYPTO_DELETE                },
  { proto::HederaFunctionality::CryptoAddLiveHash,          RequestType::CRYPTO_ADD_LIVE_HASH         },
  { proto::HederaFunctionality::CryptoDeleteLiveHash,       RequestType::CRYPTO_DELETE_LIVE_HASH      },
  { proto::HederaFunctionality::ContractCall,               RequestType::CONTRACT_CALL                },
  { proto::HederaFunctionality::ContractCreate,             RequestType::CONTRACT_CREATE              },
  { proto::HederaFunctionality::ContractUpdate,             RequestType::CONTRACT_UPDATE              },
  { proto::HederaFunctionality::FileCreate,                 RequestType::FILE_CREATE                  },
  { proto::HederaFunctionality::FileAppend,                 RequestType::FILE_APPEND                  },
  { proto::HederaFunctionality::FileUpdate,                 RequestType::FILE_UPDATE                  },
  { proto::HederaFunctionality::FileDelete,                 RequestType::FILE_DELETE                  },
  { proto::HederaFunctionality::CryptoGetAccountBalance,    RequestType::CRYPTO_GET_ACCOUNT_BALANCE   },
  { proto::HederaFunctionality::CryptoGetAccountRecords,    RequestType::CRYPTO_GET_ACCOUNT_RECORDS   },
  { proto::HederaFunctionality::CryptoGetInfo,              RequestType::CRYPTO_GET_INFO              },
  { proto::HederaFunctionality::ContractCallLocal,          RequestType::CONTRACT_CALL_LOCAL          },
  { proto::HederaFunctionality::ContractGetInfo,            RequestType::CONTRACT_GET_INFO            },
  { proto::HederaFunctionality::ContractGetBytecode,        RequestType::CONTRACT_GET_BYTECODE        },
  { proto::HederaFunctionality::GetBySolidityID,            RequestType::GET_BY_SOLIDITY_ID           },
  { proto::HederaFunctionality::GetByKey,                   RequestType::GET_BY_KEY                   },
  { proto::HederaFunctionality::CryptoGetLiveHash,          RequestType::CRYPTO_GET_LIVE_HASH         },
  { proto::HederaFunctionality::CryptoGetStakers,           RequestType::CRYPTO_GET_STAKERS           },
  { proto::HederaFunctionality::FileGetContents,            RequestType::FILE_GET_CONTENTS            },
  { proto::HederaFunctionality::FileGetInfo,                RequestType::FILE_GET_INFO                },
  { proto::HederaFunctionality::TransactionGetRecord,       RequestType::TRANSACTION_GET_RECORD       },
  { proto::HederaFunctionality::ContractGetRecords,         RequestType::CONTRACT_GET_RECORDS         },
  { proto::HederaFunctionality::CryptoCreate,               RequestType::CRYPTO_CREATE                },
  { proto::HederaFunctionality::SystemDelete,               RequestType::SYSTEM_DELETE                },
  { proto::HederaFunctionality::SystemUndelete,             RequestType::SYSTEM_UNDELETE              },
  { proto::HederaFunctionality::ContractDelete,             RequestType::CONTRACT_DELETE              },
  { proto::HederaFunctionality::Freeze,                     RequestType::FREEZE                       },
  { proto::HederaFunctionality::CreateTransactionRecord,    RequestType::CREATE_TRANSACTION_RECORD    },
  { proto::HederaFunctionality::CryptoAccountAutoRenew,     RequestType::CRYPTO_ACCOUNT_AUTO_RENEW    },
  { proto::HederaFunctionality::ContractAutoRenew,          RequestType::CONTRACT_AUTO_RENEW          },
  { proto::HederaFunctionality::GetVersionInfo,             RequestType::GET_VERSION_INFO             },
  { proto::HederaFunctionality::TransactionGetReceipt,      RequestType::TRANSACTION_GET_RECEIPT      },
  { proto::HederaFunctionality::ConsensusCreateTopic,       RequestType::CONSENSUS_CREATE_TOPIC       },
  { proto::HederaFunctionality::ConsensusUpdateTopic,       RequestType::CONSENSUS_UPDATE_TOPIC       },
  { proto::HederaFunctionality::ConsensusDeleteTopic,       RequestType::CONSENSUS_DELETE_TOPIC       },
  { proto::HederaFunctionality::ConsensusGetTopicInfo,      RequestType::CONSENSUS_GET_TOPIC_INFO     },
  { proto::HederaFunctionality::ConsensusSubmitMessage,     RequestType::CONSENSUS_SUBMIT_MESSAGE     },
  { proto::HederaFunctionality::UncheckedSubmit,            RequestType::UNCHECKED_SUBMIT             },
  { proto::HederaFunctionality::TokenCreate,                RequestType::TOKEN_CREATE                 },
  { proto::HederaFunctionality::TokenGetInfo,               RequestType::TOKEN_GET_INFO               },
  { proto::HederaFunctionality::TokenFreezeAccount,         RequestType::TOKEN_FREEZE_ACCOUNT         },
  { proto::HederaFunctionality::TokenUnfreezeAccount,       RequestType::TOKEN_UNFREEZE_ACCOUNT       },
  { proto::HederaFunctionality::TokenGrantKycToAccount,     RequestType::TOKEN_GRANT_KYC_TO_ACCOUNT   },
  { proto::HederaFunctionality::TokenRevokeKycFromAccount,  RequestType::TOKEN_REVOKE_KYC_FROM_ACCOUNT},
  { proto::HederaFunctionality::TokenDelete,                RequestType::TOKEN_DELETE                 },
  { proto::HederaFunctionality::TokenUpdate,                RequestType::TOKEN_UPDATE                 },
  { proto::HederaFunctionality::TokenMint,                  RequestType::TOKEN_MINT                   },
  { proto::HederaFunctionality::TokenBurn,                  RequestType::TOKEN_BURN                   },
  { proto::HederaFunctionality::TokenAccountWipe,           RequestType::TOKEN_ACCOUNT_WIPE           },
  { proto::HederaFunctionality::TokenAssociateToAccount,    RequestType::TOKEN_ASSOCIATE_TO_ACCOUNT   },
  { proto::HederaFunctionality::TokenDissociateFromAccount, RequestType::TOKEN_DISSOCIATE_FROM_ACCOUNT},
  { proto::HederaFunctionality::ScheduleCreate,             RequestType::SCHEDULE_CREATE              },
  { proto::HederaFunctionality::ScheduleDelete,             RequestType::SCHEDULE_DELETE              },
  { proto::HederaFunctionality::ScheduleSign,               RequestType::SCHEDULE_SIGN                },
  { proto::HederaFunctionality::ScheduleGetInfo,            RequestType::SCHEDULE_GET_INFO            },
  { proto::HederaFunctionality::TokenGetAccountNftInfos,    RequestType::TOKEN_GET_ACCOUNT_NFT_INFOS  },
  { proto::HederaFunctionality::TokenGetNftInfo,            RequestType::TOKEN_GET_NFT_INFO           },
  { proto::HederaFunctionality::TokenGetNftInfos,           RequestType::TOKEN_GET_NFT_INFOS          },
  { proto::HederaFunctionality::TokenFeeScheduleUpdate,     RequestType::TOKEN_FEE_SCHEDULE_UPDATE    },
  { proto::HederaFunctionality::NetworkGetExecutionTime,    RequestType::NETWORK_GET_EXECUTION_TIME   },
  { proto::HederaFunctionality::TokenPause,                 RequestType::TOKEN_PAUSE                  },
  { proto::HederaFunctionality::TokenUnpause,               RequestType::TOKEN_UNPAUSE                },
  { proto::HederaFunctionality::CryptoApproveAllowance,     RequestType::CRYPTO_APPROVE_ALLOWANCE     },
  { proto::HederaFunctionality::CryptoDeleteAllowance,      RequestType::CRYPTO_DELETE_ALLOWANCE      },
  { proto::HederaFunctionality::GetAccountDetails,          RequestType::GET_ACCOUNT_DETAILS          },
  { proto::HederaFunctionality::EthereumTransaction,        RequestType::ETHEREUM_TRANSACTION         },
  { proto::HederaFunctionality::NodeStakeUpdate,            RequestType::NODE_STAKE_UPDATE            },
  { proto::HederaFunctionality::UtilPrng,                   RequestType::UTIL_PRNG                    },
};

//-----
const std::unordered_map<RequestType, proto::HederaFunctionality> gRequestTypeToProtobufHederaFunctionality = {
  {RequestType::NONE,                           proto::HederaFunctionality::NONE                      },
  { RequestType::CRYPTO_TRANSFER,               proto::HederaFunctionality::CryptoTransfer            },
  { RequestType::CRYPTO_UPDATE,                 proto::HederaFunctionality::CryptoUpdate              },
  { RequestType::CRYPTO_DELETE,                 proto::HederaFunctionality::CryptoDelete              },
  { RequestType::CRYPTO_ADD_LIVE_HASH,          proto::HederaFunctionality::CryptoAddLiveHash         },
  { RequestType::CRYPTO_DELETE_LIVE_HASH,       proto::HederaFunctionality::CryptoDeleteLiveHash      },
  { RequestType::CONTRACT_CALL,                 proto::HederaFunctionality::ContractCall              },
  { RequestType::CONTRACT_CREATE,               proto::HederaFunctionality::ContractCreate            },
  { RequestType::CONTRACT_UPDATE,               proto::HederaFunctionality::ContractUpdate            },
  { RequestType::FILE_CREATE,                   proto::HederaFunctionality::FileCreate                },
  { RequestType::FILE_APPEND,                   proto::HederaFunctionality::FileAppend                },
  { RequestType::FILE_UPDATE,                   proto::HederaFunctionality::FileUpdate                },
  { RequestType::FILE_DELETE,                   proto::HederaFunctionality::FileDelete                },
  { RequestType::CRYPTO_GET_ACCOUNT_BALANCE,    proto::HederaFunctionality::CryptoGetAccountBalance   },
  { RequestType::CRYPTO_GET_ACCOUNT_RECORDS,    proto::HederaFunctionality::CryptoGetAccountRecords   },
  { RequestType::CRYPTO_GET_INFO,               proto::HederaFunctionality::CryptoGetInfo             },
  { RequestType::CONTRACT_CALL_LOCAL,           proto::HederaFunctionality::ContractCallLocal         },
  { RequestType::CONTRACT_GET_INFO,             proto::HederaFunctionality::ContractGetInfo           },
  { RequestType::CONTRACT_GET_BYTECODE,         proto::HederaFunctionality::ContractGetBytecode       },
  { RequestType::GET_BY_SOLIDITY_ID,            proto::HederaFunctionality::GetBySolidityID           },
  { RequestType::GET_BY_KEY,                    proto::HederaFunctionality::GetByKey                  },
  { RequestType::CRYPTO_GET_LIVE_HASH,          proto::HederaFunctionality::CryptoGetLiveHash         },
  { RequestType::CRYPTO_GET_STAKERS,            proto::HederaFunctionality::CryptoGetStakers          },
  { RequestType::FILE_GET_CONTENTS,             proto::HederaFunctionality::FileGetContents           },
  { RequestType::FILE_GET_INFO,                 proto::HederaFunctionality::FileGetInfo               },
  { RequestType::TRANSACTION_GET_RECORD,        proto::HederaFunctionality::TransactionGetRecord      },
  { RequestType::CONTRACT_GET_RECORDS,          proto::HederaFunctionality::ContractGetRecords        },
  { RequestType::CRYPTO_CREATE,                 proto::HederaFunctionality::CryptoCreate              },
  { RequestType::SYSTEM_DELETE,                 proto::HederaFunctionality::SystemDelete              },
  { RequestType::SYSTEM_UNDELETE,               proto::HederaFunctionality::SystemUndelete            },
  { RequestType::CONTRACT_DELETE,               proto::HederaFunctionality::ContractDelete            },
  { RequestType::FREEZE,                        proto::HederaFunctionality::Freeze                    },
  { RequestType::CREATE_TRANSACTION_RECORD,     proto::HederaFunctionality::CreateTransactionRecord   },
  { RequestType::CRYPTO_ACCOUNT_AUTO_RENEW,     proto::HederaFunctionality::CryptoAccountAutoRenew    },
  { RequestType::CONTRACT_AUTO_RENEW,           proto::HederaFunctionality::ContractAutoRenew         },
  { RequestType::GET_VERSION_INFO,              proto::HederaFunctionality::GetVersionInfo            },
  { RequestType::TRANSACTION_GET_RECEIPT,       proto::HederaFunctionality::TransactionGetReceipt     },
  { RequestType::CONSENSUS_CREATE_TOPIC,        proto::HederaFunctionality::ConsensusCreateTopic      },
  { RequestType::CONSENSUS_UPDATE_TOPIC,        proto::HederaFunctionality::ConsensusUpdateTopic      },
  { RequestType::CONSENSUS_DELETE_TOPIC,        proto::HederaFunctionality::ConsensusDeleteTopic      },
  { RequestType::CONSENSUS_GET_TOPIC_INFO,      proto::HederaFunctionality::ConsensusGetTopicInfo     },
  { RequestType::CONSENSUS_SUBMIT_MESSAGE,      proto::HederaFunctionality::ConsensusSubmitMessage    },
  { RequestType::UNCHECKED_SUBMIT,              proto::HederaFunctionality::UncheckedSubmit           },
  { RequestType::TOKEN_CREATE,                  proto::HederaFunctionality::TokenCreate               },
  { RequestType::TOKEN_GET_INFO,                proto::HederaFunctionality::TokenGetInfo              },
  { RequestType::TOKEN_FREEZE_ACCOUNT,          proto::HederaFunctionality::TokenFreezeAccount        },
  { RequestType::TOKEN_UNFREEZE_ACCOUNT,        proto::HederaFunctionality::TokenUnfreezeAccount      },
  { RequestType::TOKEN_GRANT_KYC_TO_ACCOUNT,    proto::HederaFunctionality::TokenGrantKycToAccount    },
  { RequestType::TOKEN_REVOKE_KYC_FROM_ACCOUNT, proto::HederaFunctionality::TokenRevokeKycFromAccount },
  { RequestType::TOKEN_DELETE,                  proto::HederaFunctionality::TokenDelete               },
  { RequestType::TOKEN_UPDATE,                  proto::HederaFunctionality::TokenUpdate               },
  { RequestType::TOKEN_MINT,                    proto::HederaFunctionality::TokenMint                 },
  { RequestType::TOKEN_BURN,                    proto::HederaFunctionality::TokenBurn                 },
  { RequestType::TOKEN_ACCOUNT_WIPE,            proto::HederaFunctionality::TokenAccountWipe          },
  { RequestType::TOKEN_ASSOCIATE_TO_ACCOUNT,    proto::HederaFunctionality::TokenAssociateToAccount   },
  { RequestType::TOKEN_DISSOCIATE_FROM_ACCOUNT, proto::HederaFunctionality::TokenDissociateFromAccount},
  { RequestType::SCHEDULE_CREATE,               proto::HederaFunctionality::ScheduleCreate            },
  { RequestType::SCHEDULE_DELETE,               proto::HederaFunctionality::ScheduleDelete            },
  { RequestType::SCHEDULE_SIGN,                 proto::HederaFunctionality::ScheduleSign              },
  { RequestType::SCHEDULE_GET_INFO,             proto::HederaFunctionality::ScheduleGetInfo           },
  { RequestType::TOKEN_GET_ACCOUNT_NFT_INFOS,   proto::HederaFunctionality::TokenGetAccountNftInfos   },
  { RequestType::TOKEN_GET_NFT_INFO,            proto::HederaFunctionality::TokenGetNftInfo           },
  { RequestType::TOKEN_GET_NFT_INFOS,           proto::HederaFunctionality::TokenGetNftInfos          },
  { RequestType::TOKEN_FEE_SCHEDULE_UPDATE,     proto::HederaFunctionality::TokenFeeScheduleUpdate    },
  { RequestType::NETWORK_GET_EXECUTION_TIME,    proto::HederaFunctionality::NetworkGetExecutionTime   },
  { RequestType::TOKEN_PAUSE,                   proto::HederaFunctionality::TokenPause                },
  { RequestType::TOKEN_UNPAUSE,                 proto::HederaFunctionality::TokenUnpause              },
  { RequestType::CRYPTO_APPROVE_ALLOWANCE,      proto::HederaFunctionality::CryptoApproveAllowance    },
  { RequestType::CRYPTO_DELETE_ALLOWANCE,       proto::HederaFunctionality::CryptoDeleteAllowance     },
  { RequestType::GET_ACCOUNT_DETAILS,           proto::HederaFunctionality::GetAccountDetails         },
  { RequestType::ETHEREUM_TRANSACTION,          proto::HederaFunctionality::EthereumTransaction       },
  { RequestType::NODE_STAKE_UPDATE,             proto::HederaFunctionality::NodeStakeUpdate           },
  { RequestType::UTIL_PRNG,                     proto::HederaFunctionality::UtilPrng                  }
};

//-----
const std::unordered_map<RequestType, std::string> gRequestTypeToString = {
  {RequestType::NONE,                           "NONE"                         },
  { RequestType::CRYPTO_TRANSFER,               "CRYPTO_TRANSFER"              },
  { RequestType::CRYPTO_UPDATE,                 "CRYPTO_UPDATE"                },
  { RequestType::CRYPTO_DELETE,                 "CRYPTO_DELETE"                },
  { RequestType::CRYPTO_ADD_LIVE_HASH,          "CRYPTO_ADD_LIVE_HASH"         },
  { RequestType::CRYPTO_DELETE_LIVE_HASH,       "CRYPTO_DELETE_LIVE_HASH"      },
  { RequestType::CONTRACT_CALL,                 "CONTRACT_CALL"                },
  { RequestType::CONTRACT_CREATE,               "CONTRACT_CREATE"              },
  { RequestType::CONTRACT_UPDATE,               "CONTRACT_UPDATE"              },
  { RequestType::FILE_CREATE,                   "FILE_CREATE"                  },
  { RequestType::FILE_APPEND,                   "FILE_APPEND"                  },
  { RequestType::FILE_UPDATE,                   "FILE_UPDATE"                  },
  { RequestType::FILE_DELETE,                   "FILE_DELETE"                  },
  { RequestType::CRYPTO_GET_ACCOUNT_BALANCE,    "CRYPTO_GET_ACCOUNT_BALANCE"   },
  { RequestType::CRYPTO_GET_ACCOUNT_RECORDS,    "CRYPTO_GET_ACCOUNT_RECORDS"   },
  { RequestType::CRYPTO_GET_INFO,               "CRYPTO_GET_INFO"              },
  { RequestType::CONTRACT_CALL_LOCAL,           "CONTRACT_CALL_LOCAL"          },
  { RequestType::CONTRACT_GET_INFO,             "CONTRACT_GET_INFO"            },
  { RequestType::CONTRACT_GET_BYTECODE,         "CONTRACT_GET_BYTECODE"        },
  { RequestType::GET_BY_SOLIDITY_ID,            "GET_BY_SOLIDITY_ID"           },
  { RequestType::GET_BY_KEY,                    "GET_BY_KEY"                   },
  { RequestType::CRYPTO_GET_LIVE_HASH,          "CRYPTO_GET_LIVE_HASH"         },
  { RequestType::CRYPTO_GET_STAKERS,            "CRYPTO_GET_STAKERS"           },
  { RequestType::FILE_GET_CONTENTS,             "FILE_GET_CONTENTS"            },
  { RequestType::FILE_GET_INFO,                 "FILE_GET_INFO"                },
  { RequestType::TRANSACTION_GET_RECORD,        "TRANSACTION_GET_RECORD"       },
  { RequestType::CONTRACT_GET_RECORDS,          "CONTRACT_GET_RECORDS"         },
  { RequestType::CRYPTO_CREATE,                 "CRYPTO_CREATE"                },
  { RequestType::SYSTEM_DELETE,                 "SYSTEM_DELETE"                },
  { RequestType::SYSTEM_UNDELETE,               "SYSTEM_UNDELETE"              },
  { RequestType::CONTRACT_DELETE,               "CONTRACT_DELETE"              },
  { RequestType::FREEZE,                        "FREEZE"                       },
  { RequestType::CREATE_TRANSACTION_RECORD,     "CREATE_TRANSACTION_RECORD"    },
  { RequestType::CRYPTO_ACCOUNT_AUTO_RENEW,     "CRYPTO_ACCOUNT_AUTO_RENEW"    },
  { RequestType::CONTRACT_AUTO_RENEW,           "CONTRACT_AUTO_RENEW"          },
  { RequestType::GET_VERSION_INFO,              "GET_VERSION_INFO"             },
  { RequestType::TRANSACTION_GET_RECEIPT,       "TRANSACTION_GET_RECEIPT"      },
  { RequestType::CONSENSUS_CREATE_TOPIC,        "CONSENSUS_CREATE_TOPIC"       },
  { RequestType::CONSENSUS_UPDATE_TOPIC,        "CONSENSUS_UPDATE_TOPIC"       },
  { RequestType::CONSENSUS_DELETE_TOPIC,        "CONSENSUS_DELETE_TOPIC"       },
  { RequestType::CONSENSUS_GET_TOPIC_INFO,      "CONSENSUS_GET_TOPIC_INFO"     },
  { RequestType::CONSENSUS_SUBMIT_MESSAGE,      "CONSENSUS_SUBMIT_MESSAGE"     },
  { RequestType::UNCHECKED_SUBMIT,              "UNCHECKED_SUBMIT"             },
  { RequestType::TOKEN_CREATE,                  "TOKEN_CREATE"                 },
  { RequestType::TOKEN_GET_INFO,                "TOKEN_GET_INFO"               },
  { RequestType::TOKEN_FREEZE_ACCOUNT,          "TOKEN_FREEZE_ACCOUNT"         },
  { RequestType::TOKEN_UNFREEZE_ACCOUNT,        "TOKEN_UNFREEZE_ACCOUNT"       },
  { RequestType::TOKEN_GRANT_KYC_TO_ACCOUNT,    "TOKEN_GRANT_KYC_TO_ACCOUNT"   },
  { RequestType::TOKEN_REVOKE_KYC_FROM_ACCOUNT, "TOKEN_REVOKE_KYC_FROM_ACCOUNT"},
  { RequestType::TOKEN_DELETE,                  "TOKEN_DELETE"                 },
  { RequestType::TOKEN_UPDATE,                  "TOKEN_UPDATE"                 },
  { RequestType::TOKEN_MINT,                    "TOKEN_MINT"                   },
  { RequestType::TOKEN_BURN,                    "TOKEN_BURN"                   },
  { RequestType::TOKEN_ACCOUNT_WIPE,            "TOKEN_ACCOUNT_WIPE"           },
  { RequestType::TOKEN_ASSOCIATE_TO_ACCOUNT,    "TOKEN_ASSOCIATE_TO_ACCOUNT"   },
  { RequestType::TOKEN_DISSOCIATE_FROM_ACCOUNT, "TOKEN_DISSOCIATE_FROM_ACCOUNT"},
  { RequestType::SCHEDULE_CREATE,               "SCHEDULE_CREATE"              },
  { RequestType::SCHEDULE_DELETE,               "SCHEDULE_DELETE"              },
  { RequestType::SCHEDULE_SIGN,                 "SCHEDULE_SIGN"                },
  { RequestType::SCHEDULE_GET_INFO,             "SCHEDULE_GET_INFO"            },
  { RequestType::TOKEN_GET_ACCOUNT_NFT_INFOS,   "TOKEN_GET_ACCOUNT_NFT_INFOS"  },
  { RequestType::TOKEN_GET_NFT_INFO,            "TOKEN_GET_NFT_INFO"           },
  { RequestType::TOKEN_GET_NFT_INFOS,           "TOKEN_GET_NFT_INFOS"          },
  { RequestType::TOKEN_FEE_SCHEDULE_UPDATE,     "TOKEN_FEE_SCHEDULE_UPDATE"    },
  { RequestType::NETWORK_GET_EXECUTION_TIME,    "NETWORK_GET_EXECUTION_TIME"   },
  { RequestType::TOKEN_PAUSE,                   "TOKEN_PAUSE"                  },
  { RequestType::TOKEN_UNPAUSE,                 "TOKEN_UNPAUSE"                },
  { RequestType::CRYPTO_APPROVE_ALLOWANCE,      "CRYPTO_APPROVE_ALLOWANCE"     },
  { RequestType::CRYPTO_DELETE_ALLOWANCE,       "CRYPTO_DELETE_ALLOWANCE"      },
  { RequestType::GET_ACCOUNT_DETAILS,           "GET_ACCOUNT_DETAILS"          },
  { RequestType::ETHEREUM_TRANSACTION,          "ETHEREUM_TRANSACTION"         },
  { RequestType::NODE_STAKE_UPDATE,             "NODE_STAKE_UPDATE"            },
  { RequestType::UTIL_PRNG,                     "UTIL_PRNG"                    }
};

} // namespace Hedera

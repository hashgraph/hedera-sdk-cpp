// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: system_undelete.proto

#include "system_undelete.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace proto {
PROTOBUF_CONSTEXPR SystemUndeleteTransactionBody::SystemUndeleteTransactionBody(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.id_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_._oneof_case_)*/{}} {}
struct SystemUndeleteTransactionBodyDefaultTypeInternal {
  PROTOBUF_CONSTEXPR SystemUndeleteTransactionBodyDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~SystemUndeleteTransactionBodyDefaultTypeInternal() {}
  union {
    SystemUndeleteTransactionBody _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 SystemUndeleteTransactionBodyDefaultTypeInternal _SystemUndeleteTransactionBody_default_instance_;
}  // namespace proto
static ::_pb::Metadata file_level_metadata_system_5fundelete_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_system_5fundelete_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_system_5fundelete_2eproto = nullptr;

const uint32_t TableStruct_system_5fundelete_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::proto::SystemUndeleteTransactionBody, _internal_metadata_),
  ~0u,  // no _extensions_
  PROTOBUF_FIELD_OFFSET(::proto::SystemUndeleteTransactionBody, _impl_._oneof_case_[0]),
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  ::_pbi::kInvalidFieldOffsetTag,
  ::_pbi::kInvalidFieldOffsetTag,
  PROTOBUF_FIELD_OFFSET(::proto::SystemUndeleteTransactionBody, _impl_.id_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::proto::SystemUndeleteTransactionBody)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::proto::_SystemUndeleteTransactionBody_default_instance_._instance,
};

const char descriptor_table_protodef_system_5fundelete_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\025system_undelete.proto\022\005proto\032\021basic_ty"
  "pes.proto\"o\n\035SystemUndeleteTransactionBo"
  "dy\022\037\n\006fileID\030\001 \001(\0132\r.proto.FileIDH\000\022\'\n\nc"
  "ontractID\030\002 \001(\0132\021.proto.ContractIDH\000B\004\n\002"
  "idB&\n\"com.hederahashgraph.api.proto.java"
  "P\001b\006proto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_system_5fundelete_2eproto_deps[1] = {
  &::descriptor_table_basic_5ftypes_2eproto,
};
static ::_pbi::once_flag descriptor_table_system_5fundelete_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_system_5fundelete_2eproto = {
    false, false, 210, descriptor_table_protodef_system_5fundelete_2eproto,
    "system_undelete.proto",
    &descriptor_table_system_5fundelete_2eproto_once, descriptor_table_system_5fundelete_2eproto_deps, 1, 1,
    schemas, file_default_instances, TableStruct_system_5fundelete_2eproto::offsets,
    file_level_metadata_system_5fundelete_2eproto, file_level_enum_descriptors_system_5fundelete_2eproto,
    file_level_service_descriptors_system_5fundelete_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_system_5fundelete_2eproto_getter() {
  return &descriptor_table_system_5fundelete_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_system_5fundelete_2eproto(&descriptor_table_system_5fundelete_2eproto);
namespace proto {

// ===================================================================

class SystemUndeleteTransactionBody::_Internal {
 public:
  static const ::proto::FileID& fileid(const SystemUndeleteTransactionBody* msg);
  static const ::proto::ContractID& contractid(const SystemUndeleteTransactionBody* msg);
};

const ::proto::FileID&
SystemUndeleteTransactionBody::_Internal::fileid(const SystemUndeleteTransactionBody* msg) {
  return *msg->_impl_.id_.fileid_;
}
const ::proto::ContractID&
SystemUndeleteTransactionBody::_Internal::contractid(const SystemUndeleteTransactionBody* msg) {
  return *msg->_impl_.id_.contractid_;
}
void SystemUndeleteTransactionBody::set_allocated_fileid(::proto::FileID* fileid) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  clear_id();
  if (fileid) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(fileid));
    if (message_arena != submessage_arena) {
      fileid = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, fileid, submessage_arena);
    }
    set_has_fileid();
    _impl_.id_.fileid_ = fileid;
  }
  // @@protoc_insertion_point(field_set_allocated:proto.SystemUndeleteTransactionBody.fileID)
}
void SystemUndeleteTransactionBody::clear_fileid() {
  if (_internal_has_fileid()) {
    if (GetArenaForAllocation() == nullptr) {
      delete _impl_.id_.fileid_;
    }
    clear_has_id();
  }
}
void SystemUndeleteTransactionBody::set_allocated_contractid(::proto::ContractID* contractid) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  clear_id();
  if (contractid) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(contractid));
    if (message_arena != submessage_arena) {
      contractid = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, contractid, submessage_arena);
    }
    set_has_contractid();
    _impl_.id_.contractid_ = contractid;
  }
  // @@protoc_insertion_point(field_set_allocated:proto.SystemUndeleteTransactionBody.contractID)
}
void SystemUndeleteTransactionBody::clear_contractid() {
  if (_internal_has_contractid()) {
    if (GetArenaForAllocation() == nullptr) {
      delete _impl_.id_.contractid_;
    }
    clear_has_id();
  }
}
SystemUndeleteTransactionBody::SystemUndeleteTransactionBody(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:proto.SystemUndeleteTransactionBody)
}
SystemUndeleteTransactionBody::SystemUndeleteTransactionBody(const SystemUndeleteTransactionBody& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  SystemUndeleteTransactionBody* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.id_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , /*decltype(_impl_._oneof_case_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  clear_has_id();
  switch (from.id_case()) {
    case kFileID: {
      _this->_internal_mutable_fileid()->::proto::FileID::MergeFrom(
          from._internal_fileid());
      break;
    }
    case kContractID: {
      _this->_internal_mutable_contractid()->::proto::ContractID::MergeFrom(
          from._internal_contractid());
      break;
    }
    case ID_NOT_SET: {
      break;
    }
  }
  // @@protoc_insertion_point(copy_constructor:proto.SystemUndeleteTransactionBody)
}

inline void SystemUndeleteTransactionBody::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.id_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , /*decltype(_impl_._oneof_case_)*/{}
  };
  clear_has_id();
}

SystemUndeleteTransactionBody::~SystemUndeleteTransactionBody() {
  // @@protoc_insertion_point(destructor:proto.SystemUndeleteTransactionBody)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void SystemUndeleteTransactionBody::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (has_id()) {
    clear_id();
  }
}

void SystemUndeleteTransactionBody::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void SystemUndeleteTransactionBody::clear_id() {
// @@protoc_insertion_point(one_of_clear_start:proto.SystemUndeleteTransactionBody)
  switch (id_case()) {
    case kFileID: {
      if (GetArenaForAllocation() == nullptr) {
        delete _impl_.id_.fileid_;
      }
      break;
    }
    case kContractID: {
      if (GetArenaForAllocation() == nullptr) {
        delete _impl_.id_.contractid_;
      }
      break;
    }
    case ID_NOT_SET: {
      break;
    }
  }
  _impl_._oneof_case_[0] = ID_NOT_SET;
}


void SystemUndeleteTransactionBody::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.SystemUndeleteTransactionBody)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  clear_id();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* SystemUndeleteTransactionBody::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .proto.FileID fileID = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_fileid(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.ContractID contractID = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr = ctx->ParseMessage(_internal_mutable_contractid(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* SystemUndeleteTransactionBody::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto.SystemUndeleteTransactionBody)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .proto.FileID fileID = 1;
  if (_internal_has_fileid()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, _Internal::fileid(this),
        _Internal::fileid(this).GetCachedSize(), target, stream);
  }

  // .proto.ContractID contractID = 2;
  if (_internal_has_contractid()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(2, _Internal::contractid(this),
        _Internal::contractid(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.SystemUndeleteTransactionBody)
  return target;
}

size_t SystemUndeleteTransactionBody::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.SystemUndeleteTransactionBody)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  switch (id_case()) {
    // .proto.FileID fileID = 1;
    case kFileID: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
          *_impl_.id_.fileid_);
      break;
    }
    // .proto.ContractID contractID = 2;
    case kContractID: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
          *_impl_.id_.contractid_);
      break;
    }
    case ID_NOT_SET: {
      break;
    }
  }
  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData SystemUndeleteTransactionBody::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    SystemUndeleteTransactionBody::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*SystemUndeleteTransactionBody::GetClassData() const { return &_class_data_; }


void SystemUndeleteTransactionBody::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<SystemUndeleteTransactionBody*>(&to_msg);
  auto& from = static_cast<const SystemUndeleteTransactionBody&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:proto.SystemUndeleteTransactionBody)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  switch (from.id_case()) {
    case kFileID: {
      _this->_internal_mutable_fileid()->::proto::FileID::MergeFrom(
          from._internal_fileid());
      break;
    }
    case kContractID: {
      _this->_internal_mutable_contractid()->::proto::ContractID::MergeFrom(
          from._internal_contractid());
      break;
    }
    case ID_NOT_SET: {
      break;
    }
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void SystemUndeleteTransactionBody::CopyFrom(const SystemUndeleteTransactionBody& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.SystemUndeleteTransactionBody)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SystemUndeleteTransactionBody::IsInitialized() const {
  return true;
}

void SystemUndeleteTransactionBody::InternalSwap(SystemUndeleteTransactionBody* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.id_, other->_impl_.id_);
  swap(_impl_._oneof_case_[0], other->_impl_._oneof_case_[0]);
}

::PROTOBUF_NAMESPACE_ID::Metadata SystemUndeleteTransactionBody::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_system_5fundelete_2eproto_getter, &descriptor_table_system_5fundelete_2eproto_once,
      file_level_metadata_system_5fundelete_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto::SystemUndeleteTransactionBody*
Arena::CreateMaybeMessage< ::proto::SystemUndeleteTransactionBody >(Arena* arena) {
  return Arena::CreateMessageInternal< ::proto::SystemUndeleteTransactionBody >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>

// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: system_delete.proto

#include "system_delete.pb.h"

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
PROTOBUF_CONSTEXPR SystemDeleteTransactionBody::SystemDeleteTransactionBody(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.expirationtime_)*/nullptr
  , /*decltype(_impl_.id_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_._oneof_case_)*/{}} {}
struct SystemDeleteTransactionBodyDefaultTypeInternal {
  PROTOBUF_CONSTEXPR SystemDeleteTransactionBodyDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~SystemDeleteTransactionBodyDefaultTypeInternal() {}
  union {
    SystemDeleteTransactionBody _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 SystemDeleteTransactionBodyDefaultTypeInternal _SystemDeleteTransactionBody_default_instance_;
}  // namespace proto
static ::_pb::Metadata file_level_metadata_system_5fdelete_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_system_5fdelete_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_system_5fdelete_2eproto = nullptr;

const uint32_t TableStruct_system_5fdelete_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::proto::SystemDeleteTransactionBody, _internal_metadata_),
  ~0u,  // no _extensions_
  PROTOBUF_FIELD_OFFSET(::proto::SystemDeleteTransactionBody, _impl_._oneof_case_[0]),
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  ::_pbi::kInvalidFieldOffsetTag,
  ::_pbi::kInvalidFieldOffsetTag,
  PROTOBUF_FIELD_OFFSET(::proto::SystemDeleteTransactionBody, _impl_.expirationtime_),
  PROTOBUF_FIELD_OFFSET(::proto::SystemDeleteTransactionBody, _impl_.id_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::proto::SystemDeleteTransactionBody)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::proto::_SystemDeleteTransactionBody_default_instance_._instance,
};

const char descriptor_table_protodef_system_5fdelete_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\023system_delete.proto\022\005proto\032\021basic_type"
  "s.proto\032\017timestamp.proto\"\236\001\n\033SystemDelet"
  "eTransactionBody\022\037\n\006fileID\030\001 \001(\0132\r.proto"
  ".FileIDH\000\022\'\n\ncontractID\030\002 \001(\0132\021.proto.Co"
  "ntractIDH\000\022/\n\016expirationTime\030\003 \001(\0132\027.pro"
  "to.TimestampSecondsB\004\n\002idB&\n\"com.hederah"
  "ashgraph.api.proto.javaP\001b\006proto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_system_5fdelete_2eproto_deps[2] = {
  &::descriptor_table_basic_5ftypes_2eproto,
  &::descriptor_table_timestamp_2eproto,
};
static ::_pbi::once_flag descriptor_table_system_5fdelete_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_system_5fdelete_2eproto = {
    false, false, 273, descriptor_table_protodef_system_5fdelete_2eproto,
    "system_delete.proto",
    &descriptor_table_system_5fdelete_2eproto_once, descriptor_table_system_5fdelete_2eproto_deps, 2, 1,
    schemas, file_default_instances, TableStruct_system_5fdelete_2eproto::offsets,
    file_level_metadata_system_5fdelete_2eproto, file_level_enum_descriptors_system_5fdelete_2eproto,
    file_level_service_descriptors_system_5fdelete_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_system_5fdelete_2eproto_getter() {
  return &descriptor_table_system_5fdelete_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_system_5fdelete_2eproto(&descriptor_table_system_5fdelete_2eproto);
namespace proto {

// ===================================================================

class SystemDeleteTransactionBody::_Internal {
 public:
  static const ::proto::FileID& fileid(const SystemDeleteTransactionBody* msg);
  static const ::proto::ContractID& contractid(const SystemDeleteTransactionBody* msg);
  static const ::proto::TimestampSeconds& expirationtime(const SystemDeleteTransactionBody* msg);
};

const ::proto::FileID&
SystemDeleteTransactionBody::_Internal::fileid(const SystemDeleteTransactionBody* msg) {
  return *msg->_impl_.id_.fileid_;
}
const ::proto::ContractID&
SystemDeleteTransactionBody::_Internal::contractid(const SystemDeleteTransactionBody* msg) {
  return *msg->_impl_.id_.contractid_;
}
const ::proto::TimestampSeconds&
SystemDeleteTransactionBody::_Internal::expirationtime(const SystemDeleteTransactionBody* msg) {
  return *msg->_impl_.expirationtime_;
}
void SystemDeleteTransactionBody::set_allocated_fileid(::proto::FileID* fileid) {
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
  // @@protoc_insertion_point(field_set_allocated:proto.SystemDeleteTransactionBody.fileID)
}
void SystemDeleteTransactionBody::clear_fileid() {
  if (_internal_has_fileid()) {
    if (GetArenaForAllocation() == nullptr) {
      delete _impl_.id_.fileid_;
    }
    clear_has_id();
  }
}
void SystemDeleteTransactionBody::set_allocated_contractid(::proto::ContractID* contractid) {
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
  // @@protoc_insertion_point(field_set_allocated:proto.SystemDeleteTransactionBody.contractID)
}
void SystemDeleteTransactionBody::clear_contractid() {
  if (_internal_has_contractid()) {
    if (GetArenaForAllocation() == nullptr) {
      delete _impl_.id_.contractid_;
    }
    clear_has_id();
  }
}
void SystemDeleteTransactionBody::clear_expirationtime() {
  if (GetArenaForAllocation() == nullptr && _impl_.expirationtime_ != nullptr) {
    delete _impl_.expirationtime_;
  }
  _impl_.expirationtime_ = nullptr;
}
SystemDeleteTransactionBody::SystemDeleteTransactionBody(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:proto.SystemDeleteTransactionBody)
}
SystemDeleteTransactionBody::SystemDeleteTransactionBody(const SystemDeleteTransactionBody& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  SystemDeleteTransactionBody* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.expirationtime_){nullptr}
    , decltype(_impl_.id_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , /*decltype(_impl_._oneof_case_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_expirationtime()) {
    _this->_impl_.expirationtime_ = new ::proto::TimestampSeconds(*from._impl_.expirationtime_);
  }
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
  // @@protoc_insertion_point(copy_constructor:proto.SystemDeleteTransactionBody)
}

inline void SystemDeleteTransactionBody::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.expirationtime_){nullptr}
    , decltype(_impl_.id_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , /*decltype(_impl_._oneof_case_)*/{}
  };
  clear_has_id();
}

SystemDeleteTransactionBody::~SystemDeleteTransactionBody() {
  // @@protoc_insertion_point(destructor:proto.SystemDeleteTransactionBody)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void SystemDeleteTransactionBody::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (this != internal_default_instance()) delete _impl_.expirationtime_;
  if (has_id()) {
    clear_id();
  }
}

void SystemDeleteTransactionBody::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void SystemDeleteTransactionBody::clear_id() {
// @@protoc_insertion_point(one_of_clear_start:proto.SystemDeleteTransactionBody)
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


void SystemDeleteTransactionBody::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.SystemDeleteTransactionBody)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaForAllocation() == nullptr && _impl_.expirationtime_ != nullptr) {
    delete _impl_.expirationtime_;
  }
  _impl_.expirationtime_ = nullptr;
  clear_id();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* SystemDeleteTransactionBody::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
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
      // .proto.TimestampSeconds expirationTime = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          ptr = ctx->ParseMessage(_internal_mutable_expirationtime(), ptr);
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

uint8_t* SystemDeleteTransactionBody::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto.SystemDeleteTransactionBody)
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

  // .proto.TimestampSeconds expirationTime = 3;
  if (this->_internal_has_expirationtime()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(3, _Internal::expirationtime(this),
        _Internal::expirationtime(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.SystemDeleteTransactionBody)
  return target;
}

size_t SystemDeleteTransactionBody::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.SystemDeleteTransactionBody)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .proto.TimestampSeconds expirationTime = 3;
  if (this->_internal_has_expirationtime()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.expirationtime_);
  }

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

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData SystemDeleteTransactionBody::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    SystemDeleteTransactionBody::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*SystemDeleteTransactionBody::GetClassData() const { return &_class_data_; }


void SystemDeleteTransactionBody::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<SystemDeleteTransactionBody*>(&to_msg);
  auto& from = static_cast<const SystemDeleteTransactionBody&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:proto.SystemDeleteTransactionBody)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_expirationtime()) {
    _this->_internal_mutable_expirationtime()->::proto::TimestampSeconds::MergeFrom(
        from._internal_expirationtime());
  }
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

void SystemDeleteTransactionBody::CopyFrom(const SystemDeleteTransactionBody& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.SystemDeleteTransactionBody)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SystemDeleteTransactionBody::IsInitialized() const {
  return true;
}

void SystemDeleteTransactionBody::InternalSwap(SystemDeleteTransactionBody* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.expirationtime_, other->_impl_.expirationtime_);
  swap(_impl_.id_, other->_impl_.id_);
  swap(_impl_._oneof_case_[0], other->_impl_._oneof_case_[0]);
}

::PROTOBUF_NAMESPACE_ID::Metadata SystemDeleteTransactionBody::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_system_5fdelete_2eproto_getter, &descriptor_table_system_5fdelete_2eproto_once,
      file_level_metadata_system_5fdelete_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto::SystemDeleteTransactionBody*
Arena::CreateMaybeMessage< ::proto::SystemDeleteTransactionBody >(Arena* arena) {
  return Arena::CreateMessageInternal< ::proto::SystemDeleteTransactionBody >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>

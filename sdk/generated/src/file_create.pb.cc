// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: file_create.proto

#include "file_create.pb.h"

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
PROTOBUF_CONSTEXPR FileCreateTransactionBody::FileCreateTransactionBody(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.contents_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.memo_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.expirationtime_)*/nullptr
  , /*decltype(_impl_.keys_)*/nullptr
  , /*decltype(_impl_.shardid_)*/nullptr
  , /*decltype(_impl_.realmid_)*/nullptr
  , /*decltype(_impl_.newrealmadminkey_)*/nullptr
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct FileCreateTransactionBodyDefaultTypeInternal {
  PROTOBUF_CONSTEXPR FileCreateTransactionBodyDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~FileCreateTransactionBodyDefaultTypeInternal() {}
  union {
    FileCreateTransactionBody _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 FileCreateTransactionBodyDefaultTypeInternal _FileCreateTransactionBody_default_instance_;
}  // namespace proto
static ::_pb::Metadata file_level_metadata_file_5fcreate_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_file_5fcreate_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_file_5fcreate_2eproto = nullptr;

const uint32_t TableStruct_file_5fcreate_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::proto::FileCreateTransactionBody, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::proto::FileCreateTransactionBody, _impl_.expirationtime_),
  PROTOBUF_FIELD_OFFSET(::proto::FileCreateTransactionBody, _impl_.keys_),
  PROTOBUF_FIELD_OFFSET(::proto::FileCreateTransactionBody, _impl_.contents_),
  PROTOBUF_FIELD_OFFSET(::proto::FileCreateTransactionBody, _impl_.shardid_),
  PROTOBUF_FIELD_OFFSET(::proto::FileCreateTransactionBody, _impl_.realmid_),
  PROTOBUF_FIELD_OFFSET(::proto::FileCreateTransactionBody, _impl_.newrealmadminkey_),
  PROTOBUF_FIELD_OFFSET(::proto::FileCreateTransactionBody, _impl_.memo_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::proto::FileCreateTransactionBody)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::proto::_FileCreateTransactionBody_default_instance_._instance,
};

const char descriptor_table_protodef_file_5fcreate_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\021file_create.proto\022\005proto\032\021basic_types."
  "proto\032\017timestamp.proto\"\353\001\n\031FileCreateTra"
  "nsactionBody\022(\n\016expirationTime\030\002 \001(\0132\020.p"
  "roto.Timestamp\022\034\n\004keys\030\003 \001(\0132\016.proto.Key"
  "List\022\020\n\010contents\030\004 \001(\014\022\037\n\007shardID\030\005 \001(\0132"
  "\016.proto.ShardID\022\037\n\007realmID\030\006 \001(\0132\016.proto"
  ".RealmID\022$\n\020newRealmAdminKey\030\007 \001(\0132\n.pro"
  "to.Key\022\014\n\004memo\030\010 \001(\tB&\n\"com.hederahashgr"
  "aph.api.proto.javaP\001b\006proto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_file_5fcreate_2eproto_deps[2] = {
  &::descriptor_table_basic_5ftypes_2eproto,
  &::descriptor_table_timestamp_2eproto,
};
static ::_pbi::once_flag descriptor_table_file_5fcreate_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_file_5fcreate_2eproto = {
    false, false, 348, descriptor_table_protodef_file_5fcreate_2eproto,
    "file_create.proto",
    &descriptor_table_file_5fcreate_2eproto_once, descriptor_table_file_5fcreate_2eproto_deps, 2, 1,
    schemas, file_default_instances, TableStruct_file_5fcreate_2eproto::offsets,
    file_level_metadata_file_5fcreate_2eproto, file_level_enum_descriptors_file_5fcreate_2eproto,
    file_level_service_descriptors_file_5fcreate_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_file_5fcreate_2eproto_getter() {
  return &descriptor_table_file_5fcreate_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_file_5fcreate_2eproto(&descriptor_table_file_5fcreate_2eproto);
namespace proto {

// ===================================================================

class FileCreateTransactionBody::_Internal {
 public:
  static const ::proto::Timestamp& expirationtime(const FileCreateTransactionBody* msg);
  static const ::proto::KeyList& keys(const FileCreateTransactionBody* msg);
  static const ::proto::ShardID& shardid(const FileCreateTransactionBody* msg);
  static const ::proto::RealmID& realmid(const FileCreateTransactionBody* msg);
  static const ::proto::Key& newrealmadminkey(const FileCreateTransactionBody* msg);
};

const ::proto::Timestamp&
FileCreateTransactionBody::_Internal::expirationtime(const FileCreateTransactionBody* msg) {
  return *msg->_impl_.expirationtime_;
}
const ::proto::KeyList&
FileCreateTransactionBody::_Internal::keys(const FileCreateTransactionBody* msg) {
  return *msg->_impl_.keys_;
}
const ::proto::ShardID&
FileCreateTransactionBody::_Internal::shardid(const FileCreateTransactionBody* msg) {
  return *msg->_impl_.shardid_;
}
const ::proto::RealmID&
FileCreateTransactionBody::_Internal::realmid(const FileCreateTransactionBody* msg) {
  return *msg->_impl_.realmid_;
}
const ::proto::Key&
FileCreateTransactionBody::_Internal::newrealmadminkey(const FileCreateTransactionBody* msg) {
  return *msg->_impl_.newrealmadminkey_;
}
void FileCreateTransactionBody::clear_expirationtime() {
  if (GetArenaForAllocation() == nullptr && _impl_.expirationtime_ != nullptr) {
    delete _impl_.expirationtime_;
  }
  _impl_.expirationtime_ = nullptr;
}
void FileCreateTransactionBody::clear_keys() {
  if (GetArenaForAllocation() == nullptr && _impl_.keys_ != nullptr) {
    delete _impl_.keys_;
  }
  _impl_.keys_ = nullptr;
}
void FileCreateTransactionBody::clear_shardid() {
  if (GetArenaForAllocation() == nullptr && _impl_.shardid_ != nullptr) {
    delete _impl_.shardid_;
  }
  _impl_.shardid_ = nullptr;
}
void FileCreateTransactionBody::clear_realmid() {
  if (GetArenaForAllocation() == nullptr && _impl_.realmid_ != nullptr) {
    delete _impl_.realmid_;
  }
  _impl_.realmid_ = nullptr;
}
void FileCreateTransactionBody::clear_newrealmadminkey() {
  if (GetArenaForAllocation() == nullptr && _impl_.newrealmadminkey_ != nullptr) {
    delete _impl_.newrealmadminkey_;
  }
  _impl_.newrealmadminkey_ = nullptr;
}
FileCreateTransactionBody::FileCreateTransactionBody(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:proto.FileCreateTransactionBody)
}
FileCreateTransactionBody::FileCreateTransactionBody(const FileCreateTransactionBody& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  FileCreateTransactionBody* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.contents_){}
    , decltype(_impl_.memo_){}
    , decltype(_impl_.expirationtime_){nullptr}
    , decltype(_impl_.keys_){nullptr}
    , decltype(_impl_.shardid_){nullptr}
    , decltype(_impl_.realmid_){nullptr}
    , decltype(_impl_.newrealmadminkey_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.contents_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.contents_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_contents().empty()) {
    _this->_impl_.contents_.Set(from._internal_contents(), 
      _this->GetArenaForAllocation());
  }
  _impl_.memo_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.memo_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_memo().empty()) {
    _this->_impl_.memo_.Set(from._internal_memo(), 
      _this->GetArenaForAllocation());
  }
  if (from._internal_has_expirationtime()) {
    _this->_impl_.expirationtime_ = new ::proto::Timestamp(*from._impl_.expirationtime_);
  }
  if (from._internal_has_keys()) {
    _this->_impl_.keys_ = new ::proto::KeyList(*from._impl_.keys_);
  }
  if (from._internal_has_shardid()) {
    _this->_impl_.shardid_ = new ::proto::ShardID(*from._impl_.shardid_);
  }
  if (from._internal_has_realmid()) {
    _this->_impl_.realmid_ = new ::proto::RealmID(*from._impl_.realmid_);
  }
  if (from._internal_has_newrealmadminkey()) {
    _this->_impl_.newrealmadminkey_ = new ::proto::Key(*from._impl_.newrealmadminkey_);
  }
  // @@protoc_insertion_point(copy_constructor:proto.FileCreateTransactionBody)
}

inline void FileCreateTransactionBody::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.contents_){}
    , decltype(_impl_.memo_){}
    , decltype(_impl_.expirationtime_){nullptr}
    , decltype(_impl_.keys_){nullptr}
    , decltype(_impl_.shardid_){nullptr}
    , decltype(_impl_.realmid_){nullptr}
    , decltype(_impl_.newrealmadminkey_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.contents_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.contents_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.memo_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.memo_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

FileCreateTransactionBody::~FileCreateTransactionBody() {
  // @@protoc_insertion_point(destructor:proto.FileCreateTransactionBody)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void FileCreateTransactionBody::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.contents_.Destroy();
  _impl_.memo_.Destroy();
  if (this != internal_default_instance()) delete _impl_.expirationtime_;
  if (this != internal_default_instance()) delete _impl_.keys_;
  if (this != internal_default_instance()) delete _impl_.shardid_;
  if (this != internal_default_instance()) delete _impl_.realmid_;
  if (this != internal_default_instance()) delete _impl_.newrealmadminkey_;
}

void FileCreateTransactionBody::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void FileCreateTransactionBody::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.FileCreateTransactionBody)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.contents_.ClearToEmpty();
  _impl_.memo_.ClearToEmpty();
  if (GetArenaForAllocation() == nullptr && _impl_.expirationtime_ != nullptr) {
    delete _impl_.expirationtime_;
  }
  _impl_.expirationtime_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.keys_ != nullptr) {
    delete _impl_.keys_;
  }
  _impl_.keys_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.shardid_ != nullptr) {
    delete _impl_.shardid_;
  }
  _impl_.shardid_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.realmid_ != nullptr) {
    delete _impl_.realmid_;
  }
  _impl_.realmid_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.newrealmadminkey_ != nullptr) {
    delete _impl_.newrealmadminkey_;
  }
  _impl_.newrealmadminkey_ = nullptr;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* FileCreateTransactionBody::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .proto.Timestamp expirationTime = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr = ctx->ParseMessage(_internal_mutable_expirationtime(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.KeyList keys = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          ptr = ctx->ParseMessage(_internal_mutable_keys(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bytes contents = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 34)) {
          auto str = _internal_mutable_contents();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.ShardID shardID = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 42)) {
          ptr = ctx->ParseMessage(_internal_mutable_shardid(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.RealmID realmID = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 50)) {
          ptr = ctx->ParseMessage(_internal_mutable_realmid(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.Key newRealmAdminKey = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 58)) {
          ptr = ctx->ParseMessage(_internal_mutable_newrealmadminkey(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string memo = 8;
      case 8:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 66)) {
          auto str = _internal_mutable_memo();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "proto.FileCreateTransactionBody.memo"));
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

uint8_t* FileCreateTransactionBody::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto.FileCreateTransactionBody)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .proto.Timestamp expirationTime = 2;
  if (this->_internal_has_expirationtime()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(2, _Internal::expirationtime(this),
        _Internal::expirationtime(this).GetCachedSize(), target, stream);
  }

  // .proto.KeyList keys = 3;
  if (this->_internal_has_keys()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(3, _Internal::keys(this),
        _Internal::keys(this).GetCachedSize(), target, stream);
  }

  // bytes contents = 4;
  if (!this->_internal_contents().empty()) {
    target = stream->WriteBytesMaybeAliased(
        4, this->_internal_contents(), target);
  }

  // .proto.ShardID shardID = 5;
  if (this->_internal_has_shardid()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(5, _Internal::shardid(this),
        _Internal::shardid(this).GetCachedSize(), target, stream);
  }

  // .proto.RealmID realmID = 6;
  if (this->_internal_has_realmid()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(6, _Internal::realmid(this),
        _Internal::realmid(this).GetCachedSize(), target, stream);
  }

  // .proto.Key newRealmAdminKey = 7;
  if (this->_internal_has_newrealmadminkey()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(7, _Internal::newrealmadminkey(this),
        _Internal::newrealmadminkey(this).GetCachedSize(), target, stream);
  }

  // string memo = 8;
  if (!this->_internal_memo().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_memo().data(), static_cast<int>(this->_internal_memo().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "proto.FileCreateTransactionBody.memo");
    target = stream->WriteStringMaybeAliased(
        8, this->_internal_memo(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.FileCreateTransactionBody)
  return target;
}

size_t FileCreateTransactionBody::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.FileCreateTransactionBody)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes contents = 4;
  if (!this->_internal_contents().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_contents());
  }

  // string memo = 8;
  if (!this->_internal_memo().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_memo());
  }

  // .proto.Timestamp expirationTime = 2;
  if (this->_internal_has_expirationtime()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.expirationtime_);
  }

  // .proto.KeyList keys = 3;
  if (this->_internal_has_keys()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.keys_);
  }

  // .proto.ShardID shardID = 5;
  if (this->_internal_has_shardid()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.shardid_);
  }

  // .proto.RealmID realmID = 6;
  if (this->_internal_has_realmid()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.realmid_);
  }

  // .proto.Key newRealmAdminKey = 7;
  if (this->_internal_has_newrealmadminkey()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.newrealmadminkey_);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData FileCreateTransactionBody::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    FileCreateTransactionBody::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*FileCreateTransactionBody::GetClassData() const { return &_class_data_; }


void FileCreateTransactionBody::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<FileCreateTransactionBody*>(&to_msg);
  auto& from = static_cast<const FileCreateTransactionBody&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:proto.FileCreateTransactionBody)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_contents().empty()) {
    _this->_internal_set_contents(from._internal_contents());
  }
  if (!from._internal_memo().empty()) {
    _this->_internal_set_memo(from._internal_memo());
  }
  if (from._internal_has_expirationtime()) {
    _this->_internal_mutable_expirationtime()->::proto::Timestamp::MergeFrom(
        from._internal_expirationtime());
  }
  if (from._internal_has_keys()) {
    _this->_internal_mutable_keys()->::proto::KeyList::MergeFrom(
        from._internal_keys());
  }
  if (from._internal_has_shardid()) {
    _this->_internal_mutable_shardid()->::proto::ShardID::MergeFrom(
        from._internal_shardid());
  }
  if (from._internal_has_realmid()) {
    _this->_internal_mutable_realmid()->::proto::RealmID::MergeFrom(
        from._internal_realmid());
  }
  if (from._internal_has_newrealmadminkey()) {
    _this->_internal_mutable_newrealmadminkey()->::proto::Key::MergeFrom(
        from._internal_newrealmadminkey());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void FileCreateTransactionBody::CopyFrom(const FileCreateTransactionBody& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.FileCreateTransactionBody)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FileCreateTransactionBody::IsInitialized() const {
  return true;
}

void FileCreateTransactionBody::InternalSwap(FileCreateTransactionBody* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.contents_, lhs_arena,
      &other->_impl_.contents_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.memo_, lhs_arena,
      &other->_impl_.memo_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(FileCreateTransactionBody, _impl_.newrealmadminkey_)
      + sizeof(FileCreateTransactionBody::_impl_.newrealmadminkey_)
      - PROTOBUF_FIELD_OFFSET(FileCreateTransactionBody, _impl_.expirationtime_)>(
          reinterpret_cast<char*>(&_impl_.expirationtime_),
          reinterpret_cast<char*>(&other->_impl_.expirationtime_));
}

::PROTOBUF_NAMESPACE_ID::Metadata FileCreateTransactionBody::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_file_5fcreate_2eproto_getter, &descriptor_table_file_5fcreate_2eproto_once,
      file_level_metadata_file_5fcreate_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto::FileCreateTransactionBody*
Arena::CreateMaybeMessage< ::proto::FileCreateTransactionBody >(Arena* arena) {
  return Arena::CreateMessageInternal< ::proto::FileCreateTransactionBody >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>

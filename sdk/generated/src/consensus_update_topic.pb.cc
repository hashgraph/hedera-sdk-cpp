// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: consensus_update_topic.proto

#include "consensus_update_topic.pb.h"

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
PROTOBUF_CONSTEXPR ConsensusUpdateTopicTransactionBody::ConsensusUpdateTopicTransactionBody(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.topicid_)*/nullptr
  , /*decltype(_impl_.memo_)*/nullptr
  , /*decltype(_impl_.expirationtime_)*/nullptr
  , /*decltype(_impl_.adminkey_)*/nullptr
  , /*decltype(_impl_.submitkey_)*/nullptr
  , /*decltype(_impl_.autorenewperiod_)*/nullptr
  , /*decltype(_impl_.autorenewaccount_)*/nullptr
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct ConsensusUpdateTopicTransactionBodyDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ConsensusUpdateTopicTransactionBodyDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~ConsensusUpdateTopicTransactionBodyDefaultTypeInternal() {}
  union {
    ConsensusUpdateTopicTransactionBody _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ConsensusUpdateTopicTransactionBodyDefaultTypeInternal _ConsensusUpdateTopicTransactionBody_default_instance_;
}  // namespace proto
static ::_pb::Metadata file_level_metadata_consensus_5fupdate_5ftopic_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_consensus_5fupdate_5ftopic_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_consensus_5fupdate_5ftopic_2eproto = nullptr;

const uint32_t TableStruct_consensus_5fupdate_5ftopic_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::proto::ConsensusUpdateTopicTransactionBody, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::proto::ConsensusUpdateTopicTransactionBody, _impl_.topicid_),
  PROTOBUF_FIELD_OFFSET(::proto::ConsensusUpdateTopicTransactionBody, _impl_.memo_),
  PROTOBUF_FIELD_OFFSET(::proto::ConsensusUpdateTopicTransactionBody, _impl_.expirationtime_),
  PROTOBUF_FIELD_OFFSET(::proto::ConsensusUpdateTopicTransactionBody, _impl_.adminkey_),
  PROTOBUF_FIELD_OFFSET(::proto::ConsensusUpdateTopicTransactionBody, _impl_.submitkey_),
  PROTOBUF_FIELD_OFFSET(::proto::ConsensusUpdateTopicTransactionBody, _impl_.autorenewperiod_),
  PROTOBUF_FIELD_OFFSET(::proto::ConsensusUpdateTopicTransactionBody, _impl_.autorenewaccount_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::proto::ConsensusUpdateTopicTransactionBody)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::proto::_ConsensusUpdateTopicTransactionBody_default_instance_._instance,
};

const char descriptor_table_protodef_consensus_5fupdate_5ftopic_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\034consensus_update_topic.proto\022\005proto\032\036g"
  "oogle/protobuf/wrappers.proto\032\021basic_typ"
  "es.proto\032\016duration.proto\032\017timestamp.prot"
  "o\"\257\002\n#ConsensusUpdateTopicTransactionBod"
  "y\022\037\n\007topicID\030\001 \001(\0132\016.proto.TopicID\022*\n\004me"
  "mo\030\002 \001(\0132\034.google.protobuf.StringValue\022("
  "\n\016expirationTime\030\004 \001(\0132\020.proto.Timestamp"
  "\022\034\n\010adminKey\030\006 \001(\0132\n.proto.Key\022\035\n\tsubmit"
  "Key\030\007 \001(\0132\n.proto.Key\022(\n\017autoRenewPeriod"
  "\030\010 \001(\0132\017.proto.Duration\022*\n\020autoRenewAcco"
  "unt\030\t \001(\0132\020.proto.AccountIDB&\n\"com.heder"
  "ahashgraph.api.proto.javaP\001b\006proto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_consensus_5fupdate_5ftopic_2eproto_deps[4] = {
  &::descriptor_table_basic_5ftypes_2eproto,
  &::descriptor_table_duration_2eproto,
  &::descriptor_table_google_2fprotobuf_2fwrappers_2eproto,
  &::descriptor_table_timestamp_2eproto,
};
static ::_pbi::once_flag descriptor_table_consensus_5fupdate_5ftopic_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_consensus_5fupdate_5ftopic_2eproto = {
    false, false, 475, descriptor_table_protodef_consensus_5fupdate_5ftopic_2eproto,
    "consensus_update_topic.proto",
    &descriptor_table_consensus_5fupdate_5ftopic_2eproto_once, descriptor_table_consensus_5fupdate_5ftopic_2eproto_deps, 4, 1,
    schemas, file_default_instances, TableStruct_consensus_5fupdate_5ftopic_2eproto::offsets,
    file_level_metadata_consensus_5fupdate_5ftopic_2eproto, file_level_enum_descriptors_consensus_5fupdate_5ftopic_2eproto,
    file_level_service_descriptors_consensus_5fupdate_5ftopic_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_consensus_5fupdate_5ftopic_2eproto_getter() {
  return &descriptor_table_consensus_5fupdate_5ftopic_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_consensus_5fupdate_5ftopic_2eproto(&descriptor_table_consensus_5fupdate_5ftopic_2eproto);
namespace proto {

// ===================================================================

class ConsensusUpdateTopicTransactionBody::_Internal {
 public:
  static const ::proto::TopicID& topicid(const ConsensusUpdateTopicTransactionBody* msg);
  static const ::PROTOBUF_NAMESPACE_ID::StringValue& memo(const ConsensusUpdateTopicTransactionBody* msg);
  static const ::proto::Timestamp& expirationtime(const ConsensusUpdateTopicTransactionBody* msg);
  static const ::proto::Key& adminkey(const ConsensusUpdateTopicTransactionBody* msg);
  static const ::proto::Key& submitkey(const ConsensusUpdateTopicTransactionBody* msg);
  static const ::proto::Duration& autorenewperiod(const ConsensusUpdateTopicTransactionBody* msg);
  static const ::proto::AccountID& autorenewaccount(const ConsensusUpdateTopicTransactionBody* msg);
};

const ::proto::TopicID&
ConsensusUpdateTopicTransactionBody::_Internal::topicid(const ConsensusUpdateTopicTransactionBody* msg) {
  return *msg->_impl_.topicid_;
}
const ::PROTOBUF_NAMESPACE_ID::StringValue&
ConsensusUpdateTopicTransactionBody::_Internal::memo(const ConsensusUpdateTopicTransactionBody* msg) {
  return *msg->_impl_.memo_;
}
const ::proto::Timestamp&
ConsensusUpdateTopicTransactionBody::_Internal::expirationtime(const ConsensusUpdateTopicTransactionBody* msg) {
  return *msg->_impl_.expirationtime_;
}
const ::proto::Key&
ConsensusUpdateTopicTransactionBody::_Internal::adminkey(const ConsensusUpdateTopicTransactionBody* msg) {
  return *msg->_impl_.adminkey_;
}
const ::proto::Key&
ConsensusUpdateTopicTransactionBody::_Internal::submitkey(const ConsensusUpdateTopicTransactionBody* msg) {
  return *msg->_impl_.submitkey_;
}
const ::proto::Duration&
ConsensusUpdateTopicTransactionBody::_Internal::autorenewperiod(const ConsensusUpdateTopicTransactionBody* msg) {
  return *msg->_impl_.autorenewperiod_;
}
const ::proto::AccountID&
ConsensusUpdateTopicTransactionBody::_Internal::autorenewaccount(const ConsensusUpdateTopicTransactionBody* msg) {
  return *msg->_impl_.autorenewaccount_;
}
void ConsensusUpdateTopicTransactionBody::clear_topicid() {
  if (GetArenaForAllocation() == nullptr && _impl_.topicid_ != nullptr) {
    delete _impl_.topicid_;
  }
  _impl_.topicid_ = nullptr;
}
void ConsensusUpdateTopicTransactionBody::clear_memo() {
  if (GetArenaForAllocation() == nullptr && _impl_.memo_ != nullptr) {
    delete _impl_.memo_;
  }
  _impl_.memo_ = nullptr;
}
void ConsensusUpdateTopicTransactionBody::clear_expirationtime() {
  if (GetArenaForAllocation() == nullptr && _impl_.expirationtime_ != nullptr) {
    delete _impl_.expirationtime_;
  }
  _impl_.expirationtime_ = nullptr;
}
void ConsensusUpdateTopicTransactionBody::clear_adminkey() {
  if (GetArenaForAllocation() == nullptr && _impl_.adminkey_ != nullptr) {
    delete _impl_.adminkey_;
  }
  _impl_.adminkey_ = nullptr;
}
void ConsensusUpdateTopicTransactionBody::clear_submitkey() {
  if (GetArenaForAllocation() == nullptr && _impl_.submitkey_ != nullptr) {
    delete _impl_.submitkey_;
  }
  _impl_.submitkey_ = nullptr;
}
void ConsensusUpdateTopicTransactionBody::clear_autorenewperiod() {
  if (GetArenaForAllocation() == nullptr && _impl_.autorenewperiod_ != nullptr) {
    delete _impl_.autorenewperiod_;
  }
  _impl_.autorenewperiod_ = nullptr;
}
void ConsensusUpdateTopicTransactionBody::clear_autorenewaccount() {
  if (GetArenaForAllocation() == nullptr && _impl_.autorenewaccount_ != nullptr) {
    delete _impl_.autorenewaccount_;
  }
  _impl_.autorenewaccount_ = nullptr;
}
ConsensusUpdateTopicTransactionBody::ConsensusUpdateTopicTransactionBody(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:proto.ConsensusUpdateTopicTransactionBody)
}
ConsensusUpdateTopicTransactionBody::ConsensusUpdateTopicTransactionBody(const ConsensusUpdateTopicTransactionBody& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  ConsensusUpdateTopicTransactionBody* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.topicid_){nullptr}
    , decltype(_impl_.memo_){nullptr}
    , decltype(_impl_.expirationtime_){nullptr}
    , decltype(_impl_.adminkey_){nullptr}
    , decltype(_impl_.submitkey_){nullptr}
    , decltype(_impl_.autorenewperiod_){nullptr}
    , decltype(_impl_.autorenewaccount_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_topicid()) {
    _this->_impl_.topicid_ = new ::proto::TopicID(*from._impl_.topicid_);
  }
  if (from._internal_has_memo()) {
    _this->_impl_.memo_ = new ::PROTOBUF_NAMESPACE_ID::StringValue(*from._impl_.memo_);
  }
  if (from._internal_has_expirationtime()) {
    _this->_impl_.expirationtime_ = new ::proto::Timestamp(*from._impl_.expirationtime_);
  }
  if (from._internal_has_adminkey()) {
    _this->_impl_.adminkey_ = new ::proto::Key(*from._impl_.adminkey_);
  }
  if (from._internal_has_submitkey()) {
    _this->_impl_.submitkey_ = new ::proto::Key(*from._impl_.submitkey_);
  }
  if (from._internal_has_autorenewperiod()) {
    _this->_impl_.autorenewperiod_ = new ::proto::Duration(*from._impl_.autorenewperiod_);
  }
  if (from._internal_has_autorenewaccount()) {
    _this->_impl_.autorenewaccount_ = new ::proto::AccountID(*from._impl_.autorenewaccount_);
  }
  // @@protoc_insertion_point(copy_constructor:proto.ConsensusUpdateTopicTransactionBody)
}

inline void ConsensusUpdateTopicTransactionBody::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.topicid_){nullptr}
    , decltype(_impl_.memo_){nullptr}
    , decltype(_impl_.expirationtime_){nullptr}
    , decltype(_impl_.adminkey_){nullptr}
    , decltype(_impl_.submitkey_){nullptr}
    , decltype(_impl_.autorenewperiod_){nullptr}
    , decltype(_impl_.autorenewaccount_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

ConsensusUpdateTopicTransactionBody::~ConsensusUpdateTopicTransactionBody() {
  // @@protoc_insertion_point(destructor:proto.ConsensusUpdateTopicTransactionBody)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void ConsensusUpdateTopicTransactionBody::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (this != internal_default_instance()) delete _impl_.topicid_;
  if (this != internal_default_instance()) delete _impl_.memo_;
  if (this != internal_default_instance()) delete _impl_.expirationtime_;
  if (this != internal_default_instance()) delete _impl_.adminkey_;
  if (this != internal_default_instance()) delete _impl_.submitkey_;
  if (this != internal_default_instance()) delete _impl_.autorenewperiod_;
  if (this != internal_default_instance()) delete _impl_.autorenewaccount_;
}

void ConsensusUpdateTopicTransactionBody::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void ConsensusUpdateTopicTransactionBody::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.ConsensusUpdateTopicTransactionBody)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaForAllocation() == nullptr && _impl_.topicid_ != nullptr) {
    delete _impl_.topicid_;
  }
  _impl_.topicid_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.memo_ != nullptr) {
    delete _impl_.memo_;
  }
  _impl_.memo_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.expirationtime_ != nullptr) {
    delete _impl_.expirationtime_;
  }
  _impl_.expirationtime_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.adminkey_ != nullptr) {
    delete _impl_.adminkey_;
  }
  _impl_.adminkey_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.submitkey_ != nullptr) {
    delete _impl_.submitkey_;
  }
  _impl_.submitkey_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.autorenewperiod_ != nullptr) {
    delete _impl_.autorenewperiod_;
  }
  _impl_.autorenewperiod_ = nullptr;
  if (GetArenaForAllocation() == nullptr && _impl_.autorenewaccount_ != nullptr) {
    delete _impl_.autorenewaccount_;
  }
  _impl_.autorenewaccount_ = nullptr;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ConsensusUpdateTopicTransactionBody::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // .proto.TopicID topicID = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_topicid(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .google.protobuf.StringValue memo = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr = ctx->ParseMessage(_internal_mutable_memo(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.Timestamp expirationTime = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 34)) {
          ptr = ctx->ParseMessage(_internal_mutable_expirationtime(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.Key adminKey = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 50)) {
          ptr = ctx->ParseMessage(_internal_mutable_adminkey(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.Key submitKey = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 58)) {
          ptr = ctx->ParseMessage(_internal_mutable_submitkey(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.Duration autoRenewPeriod = 8;
      case 8:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 66)) {
          ptr = ctx->ParseMessage(_internal_mutable_autorenewperiod(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.AccountID autoRenewAccount = 9;
      case 9:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 74)) {
          ptr = ctx->ParseMessage(_internal_mutable_autorenewaccount(), ptr);
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

uint8_t* ConsensusUpdateTopicTransactionBody::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto.ConsensusUpdateTopicTransactionBody)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .proto.TopicID topicID = 1;
  if (this->_internal_has_topicid()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, _Internal::topicid(this),
        _Internal::topicid(this).GetCachedSize(), target, stream);
  }

  // .google.protobuf.StringValue memo = 2;
  if (this->_internal_has_memo()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(2, _Internal::memo(this),
        _Internal::memo(this).GetCachedSize(), target, stream);
  }

  // .proto.Timestamp expirationTime = 4;
  if (this->_internal_has_expirationtime()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(4, _Internal::expirationtime(this),
        _Internal::expirationtime(this).GetCachedSize(), target, stream);
  }

  // .proto.Key adminKey = 6;
  if (this->_internal_has_adminkey()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(6, _Internal::adminkey(this),
        _Internal::adminkey(this).GetCachedSize(), target, stream);
  }

  // .proto.Key submitKey = 7;
  if (this->_internal_has_submitkey()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(7, _Internal::submitkey(this),
        _Internal::submitkey(this).GetCachedSize(), target, stream);
  }

  // .proto.Duration autoRenewPeriod = 8;
  if (this->_internal_has_autorenewperiod()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(8, _Internal::autorenewperiod(this),
        _Internal::autorenewperiod(this).GetCachedSize(), target, stream);
  }

  // .proto.AccountID autoRenewAccount = 9;
  if (this->_internal_has_autorenewaccount()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(9, _Internal::autorenewaccount(this),
        _Internal::autorenewaccount(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.ConsensusUpdateTopicTransactionBody)
  return target;
}

size_t ConsensusUpdateTopicTransactionBody::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.ConsensusUpdateTopicTransactionBody)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .proto.TopicID topicID = 1;
  if (this->_internal_has_topicid()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.topicid_);
  }

  // .google.protobuf.StringValue memo = 2;
  if (this->_internal_has_memo()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.memo_);
  }

  // .proto.Timestamp expirationTime = 4;
  if (this->_internal_has_expirationtime()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.expirationtime_);
  }

  // .proto.Key adminKey = 6;
  if (this->_internal_has_adminkey()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.adminkey_);
  }

  // .proto.Key submitKey = 7;
  if (this->_internal_has_submitkey()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.submitkey_);
  }

  // .proto.Duration autoRenewPeriod = 8;
  if (this->_internal_has_autorenewperiod()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.autorenewperiod_);
  }

  // .proto.AccountID autoRenewAccount = 9;
  if (this->_internal_has_autorenewaccount()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.autorenewaccount_);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData ConsensusUpdateTopicTransactionBody::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    ConsensusUpdateTopicTransactionBody::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*ConsensusUpdateTopicTransactionBody::GetClassData() const { return &_class_data_; }


void ConsensusUpdateTopicTransactionBody::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<ConsensusUpdateTopicTransactionBody*>(&to_msg);
  auto& from = static_cast<const ConsensusUpdateTopicTransactionBody&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:proto.ConsensusUpdateTopicTransactionBody)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_topicid()) {
    _this->_internal_mutable_topicid()->::proto::TopicID::MergeFrom(
        from._internal_topicid());
  }
  if (from._internal_has_memo()) {
    _this->_internal_mutable_memo()->::PROTOBUF_NAMESPACE_ID::StringValue::MergeFrom(
        from._internal_memo());
  }
  if (from._internal_has_expirationtime()) {
    _this->_internal_mutable_expirationtime()->::proto::Timestamp::MergeFrom(
        from._internal_expirationtime());
  }
  if (from._internal_has_adminkey()) {
    _this->_internal_mutable_adminkey()->::proto::Key::MergeFrom(
        from._internal_adminkey());
  }
  if (from._internal_has_submitkey()) {
    _this->_internal_mutable_submitkey()->::proto::Key::MergeFrom(
        from._internal_submitkey());
  }
  if (from._internal_has_autorenewperiod()) {
    _this->_internal_mutable_autorenewperiod()->::proto::Duration::MergeFrom(
        from._internal_autorenewperiod());
  }
  if (from._internal_has_autorenewaccount()) {
    _this->_internal_mutable_autorenewaccount()->::proto::AccountID::MergeFrom(
        from._internal_autorenewaccount());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void ConsensusUpdateTopicTransactionBody::CopyFrom(const ConsensusUpdateTopicTransactionBody& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.ConsensusUpdateTopicTransactionBody)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ConsensusUpdateTopicTransactionBody::IsInitialized() const {
  return true;
}

void ConsensusUpdateTopicTransactionBody::InternalSwap(ConsensusUpdateTopicTransactionBody* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(ConsensusUpdateTopicTransactionBody, _impl_.autorenewaccount_)
      + sizeof(ConsensusUpdateTopicTransactionBody::_impl_.autorenewaccount_)
      - PROTOBUF_FIELD_OFFSET(ConsensusUpdateTopicTransactionBody, _impl_.topicid_)>(
          reinterpret_cast<char*>(&_impl_.topicid_),
          reinterpret_cast<char*>(&other->_impl_.topicid_));
}

::PROTOBUF_NAMESPACE_ID::Metadata ConsensusUpdateTopicTransactionBody::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_consensus_5fupdate_5ftopic_2eproto_getter, &descriptor_table_consensus_5fupdate_5ftopic_2eproto_once,
      file_level_metadata_consensus_5fupdate_5ftopic_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto::ConsensusUpdateTopicTransactionBody*
Arena::CreateMaybeMessage< ::proto::ConsensusUpdateTopicTransactionBody >(Arena* arena) {
  return Arena::CreateMessageInternal< ::proto::ConsensusUpdateTopicTransactionBody >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>

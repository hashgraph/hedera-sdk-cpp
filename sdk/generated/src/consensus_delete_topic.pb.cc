// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: consensus_delete_topic.proto

#include "consensus_delete_topic.pb.h"

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
PROTOBUF_CONSTEXPR ConsensusDeleteTopicTransactionBody::ConsensusDeleteTopicTransactionBody(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.topicid_)*/nullptr
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct ConsensusDeleteTopicTransactionBodyDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ConsensusDeleteTopicTransactionBodyDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~ConsensusDeleteTopicTransactionBodyDefaultTypeInternal() {}
  union {
    ConsensusDeleteTopicTransactionBody _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ConsensusDeleteTopicTransactionBodyDefaultTypeInternal _ConsensusDeleteTopicTransactionBody_default_instance_;
}  // namespace proto
static ::_pb::Metadata file_level_metadata_consensus_5fdelete_5ftopic_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_consensus_5fdelete_5ftopic_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_consensus_5fdelete_5ftopic_2eproto = nullptr;

const uint32_t TableStruct_consensus_5fdelete_5ftopic_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::proto::ConsensusDeleteTopicTransactionBody, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::proto::ConsensusDeleteTopicTransactionBody, _impl_.topicid_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::proto::ConsensusDeleteTopicTransactionBody)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::proto::_ConsensusDeleteTopicTransactionBody_default_instance_._instance,
};

const char descriptor_table_protodef_consensus_5fdelete_5ftopic_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\034consensus_delete_topic.proto\022\005proto\032\021b"
  "asic_types.proto\"F\n#ConsensusDeleteTopic"
  "TransactionBody\022\037\n\007topicID\030\001 \001(\0132\016.proto"
  ".TopicIDB&\n\"com.hederahashgraph.api.prot"
  "o.javaP\001b\006proto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_consensus_5fdelete_5ftopic_2eproto_deps[1] = {
  &::descriptor_table_basic_5ftypes_2eproto,
};
static ::_pbi::once_flag descriptor_table_consensus_5fdelete_5ftopic_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_consensus_5fdelete_5ftopic_2eproto = {
    false, false, 176, descriptor_table_protodef_consensus_5fdelete_5ftopic_2eproto,
    "consensus_delete_topic.proto",
    &descriptor_table_consensus_5fdelete_5ftopic_2eproto_once, descriptor_table_consensus_5fdelete_5ftopic_2eproto_deps, 1, 1,
    schemas, file_default_instances, TableStruct_consensus_5fdelete_5ftopic_2eproto::offsets,
    file_level_metadata_consensus_5fdelete_5ftopic_2eproto, file_level_enum_descriptors_consensus_5fdelete_5ftopic_2eproto,
    file_level_service_descriptors_consensus_5fdelete_5ftopic_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_consensus_5fdelete_5ftopic_2eproto_getter() {
  return &descriptor_table_consensus_5fdelete_5ftopic_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_consensus_5fdelete_5ftopic_2eproto(&descriptor_table_consensus_5fdelete_5ftopic_2eproto);
namespace proto {

// ===================================================================

class ConsensusDeleteTopicTransactionBody::_Internal {
 public:
  static const ::proto::TopicID& topicid(const ConsensusDeleteTopicTransactionBody* msg);
};

const ::proto::TopicID&
ConsensusDeleteTopicTransactionBody::_Internal::topicid(const ConsensusDeleteTopicTransactionBody* msg) {
  return *msg->_impl_.topicid_;
}
void ConsensusDeleteTopicTransactionBody::clear_topicid() {
  if (GetArenaForAllocation() == nullptr && _impl_.topicid_ != nullptr) {
    delete _impl_.topicid_;
  }
  _impl_.topicid_ = nullptr;
}
ConsensusDeleteTopicTransactionBody::ConsensusDeleteTopicTransactionBody(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:proto.ConsensusDeleteTopicTransactionBody)
}
ConsensusDeleteTopicTransactionBody::ConsensusDeleteTopicTransactionBody(const ConsensusDeleteTopicTransactionBody& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  ConsensusDeleteTopicTransactionBody* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.topicid_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_topicid()) {
    _this->_impl_.topicid_ = new ::proto::TopicID(*from._impl_.topicid_);
  }
  // @@protoc_insertion_point(copy_constructor:proto.ConsensusDeleteTopicTransactionBody)
}

inline void ConsensusDeleteTopicTransactionBody::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.topicid_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

ConsensusDeleteTopicTransactionBody::~ConsensusDeleteTopicTransactionBody() {
  // @@protoc_insertion_point(destructor:proto.ConsensusDeleteTopicTransactionBody)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void ConsensusDeleteTopicTransactionBody::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (this != internal_default_instance()) delete _impl_.topicid_;
}

void ConsensusDeleteTopicTransactionBody::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void ConsensusDeleteTopicTransactionBody::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.ConsensusDeleteTopicTransactionBody)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArenaForAllocation() == nullptr && _impl_.topicid_ != nullptr) {
    delete _impl_.topicid_;
  }
  _impl_.topicid_ = nullptr;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ConsensusDeleteTopicTransactionBody::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
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

uint8_t* ConsensusDeleteTopicTransactionBody::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto.ConsensusDeleteTopicTransactionBody)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // .proto.TopicID topicID = 1;
  if (this->_internal_has_topicid()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, _Internal::topicid(this),
        _Internal::topicid(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.ConsensusDeleteTopicTransactionBody)
  return target;
}

size_t ConsensusDeleteTopicTransactionBody::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.ConsensusDeleteTopicTransactionBody)
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

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData ConsensusDeleteTopicTransactionBody::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    ConsensusDeleteTopicTransactionBody::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*ConsensusDeleteTopicTransactionBody::GetClassData() const { return &_class_data_; }


void ConsensusDeleteTopicTransactionBody::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<ConsensusDeleteTopicTransactionBody*>(&to_msg);
  auto& from = static_cast<const ConsensusDeleteTopicTransactionBody&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:proto.ConsensusDeleteTopicTransactionBody)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_topicid()) {
    _this->_internal_mutable_topicid()->::proto::TopicID::MergeFrom(
        from._internal_topicid());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void ConsensusDeleteTopicTransactionBody::CopyFrom(const ConsensusDeleteTopicTransactionBody& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.ConsensusDeleteTopicTransactionBody)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ConsensusDeleteTopicTransactionBody::IsInitialized() const {
  return true;
}

void ConsensusDeleteTopicTransactionBody::InternalSwap(ConsensusDeleteTopicTransactionBody* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.topicid_, other->_impl_.topicid_);
}

::PROTOBUF_NAMESPACE_ID::Metadata ConsensusDeleteTopicTransactionBody::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_consensus_5fdelete_5ftopic_2eproto_getter, &descriptor_table_consensus_5fdelete_5ftopic_2eproto_once,
      file_level_metadata_consensus_5fdelete_5ftopic_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto::ConsensusDeleteTopicTransactionBody*
Arena::CreateMaybeMessage< ::proto::ConsensusDeleteTopicTransactionBody >(Arena* arena) {
  return Arena::CreateMessageInternal< ::proto::ConsensusDeleteTopicTransactionBody >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>

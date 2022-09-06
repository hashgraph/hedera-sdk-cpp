// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: transaction_contents.proto

#include "transaction_contents.pb.h"

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
PROTOBUF_CONSTEXPR SignedTransaction::SignedTransaction(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.bodybytes_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.sigmap_)*/nullptr
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct SignedTransactionDefaultTypeInternal {
  PROTOBUF_CONSTEXPR SignedTransactionDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~SignedTransactionDefaultTypeInternal() {}
  union {
    SignedTransaction _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 SignedTransactionDefaultTypeInternal _SignedTransaction_default_instance_;
}  // namespace proto
static ::_pb::Metadata file_level_metadata_transaction_5fcontents_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_transaction_5fcontents_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_transaction_5fcontents_2eproto = nullptr;

const uint32_t TableStruct_transaction_5fcontents_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::proto::SignedTransaction, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::proto::SignedTransaction, _impl_.bodybytes_),
  PROTOBUF_FIELD_OFFSET(::proto::SignedTransaction, _impl_.sigmap_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::proto::SignedTransaction)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::proto::_SignedTransaction_default_instance_._instance,
};

const char descriptor_table_protodef_transaction_5fcontents_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\032transaction_contents.proto\022\005proto\032\021bas"
  "ic_types.proto\"K\n\021SignedTransaction\022\021\n\tb"
  "odyBytes\030\001 \001(\014\022#\n\006sigMap\030\002 \001(\0132\023.proto.S"
  "ignatureMapB&\n\"com.hederahashgraph.api.p"
  "roto.javaP\001b\006proto3"
  ;
static const ::_pbi::DescriptorTable* const descriptor_table_transaction_5fcontents_2eproto_deps[1] = {
  &::descriptor_table_basic_5ftypes_2eproto,
};
static ::_pbi::once_flag descriptor_table_transaction_5fcontents_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_transaction_5fcontents_2eproto = {
    false, false, 179, descriptor_table_protodef_transaction_5fcontents_2eproto,
    "transaction_contents.proto",
    &descriptor_table_transaction_5fcontents_2eproto_once, descriptor_table_transaction_5fcontents_2eproto_deps, 1, 1,
    schemas, file_default_instances, TableStruct_transaction_5fcontents_2eproto::offsets,
    file_level_metadata_transaction_5fcontents_2eproto, file_level_enum_descriptors_transaction_5fcontents_2eproto,
    file_level_service_descriptors_transaction_5fcontents_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_transaction_5fcontents_2eproto_getter() {
  return &descriptor_table_transaction_5fcontents_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_transaction_5fcontents_2eproto(&descriptor_table_transaction_5fcontents_2eproto);
namespace proto {

// ===================================================================

class SignedTransaction::_Internal {
 public:
  static const ::proto::SignatureMap& sigmap(const SignedTransaction* msg);
};

const ::proto::SignatureMap&
SignedTransaction::_Internal::sigmap(const SignedTransaction* msg) {
  return *msg->_impl_.sigmap_;
}
void SignedTransaction::clear_sigmap() {
  if (GetArenaForAllocation() == nullptr && _impl_.sigmap_ != nullptr) {
    delete _impl_.sigmap_;
  }
  _impl_.sigmap_ = nullptr;
}
SignedTransaction::SignedTransaction(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:proto.SignedTransaction)
}
SignedTransaction::SignedTransaction(const SignedTransaction& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  SignedTransaction* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.bodybytes_){}
    , decltype(_impl_.sigmap_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.bodybytes_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.bodybytes_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_bodybytes().empty()) {
    _this->_impl_.bodybytes_.Set(from._internal_bodybytes(), 
      _this->GetArenaForAllocation());
  }
  if (from._internal_has_sigmap()) {
    _this->_impl_.sigmap_ = new ::proto::SignatureMap(*from._impl_.sigmap_);
  }
  // @@protoc_insertion_point(copy_constructor:proto.SignedTransaction)
}

inline void SignedTransaction::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.bodybytes_){}
    , decltype(_impl_.sigmap_){nullptr}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.bodybytes_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.bodybytes_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

SignedTransaction::~SignedTransaction() {
  // @@protoc_insertion_point(destructor:proto.SignedTransaction)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void SignedTransaction::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.bodybytes_.Destroy();
  if (this != internal_default_instance()) delete _impl_.sigmap_;
}

void SignedTransaction::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void SignedTransaction::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.SignedTransaction)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.bodybytes_.ClearToEmpty();
  if (GetArenaForAllocation() == nullptr && _impl_.sigmap_ != nullptr) {
    delete _impl_.sigmap_;
  }
  _impl_.sigmap_ = nullptr;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* SignedTransaction::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // bytes bodyBytes = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_bodybytes();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto.SignatureMap sigMap = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr = ctx->ParseMessage(_internal_mutable_sigmap(), ptr);
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

uint8_t* SignedTransaction::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto.SignedTransaction)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // bytes bodyBytes = 1;
  if (!this->_internal_bodybytes().empty()) {
    target = stream->WriteBytesMaybeAliased(
        1, this->_internal_bodybytes(), target);
  }

  // .proto.SignatureMap sigMap = 2;
  if (this->_internal_has_sigmap()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(2, _Internal::sigmap(this),
        _Internal::sigmap(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.SignedTransaction)
  return target;
}

size_t SignedTransaction::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.SignedTransaction)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes bodyBytes = 1;
  if (!this->_internal_bodybytes().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_bodybytes());
  }

  // .proto.SignatureMap sigMap = 2;
  if (this->_internal_has_sigmap()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.sigmap_);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData SignedTransaction::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    SignedTransaction::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*SignedTransaction::GetClassData() const { return &_class_data_; }


void SignedTransaction::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<SignedTransaction*>(&to_msg);
  auto& from = static_cast<const SignedTransaction&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:proto.SignedTransaction)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_bodybytes().empty()) {
    _this->_internal_set_bodybytes(from._internal_bodybytes());
  }
  if (from._internal_has_sigmap()) {
    _this->_internal_mutable_sigmap()->::proto::SignatureMap::MergeFrom(
        from._internal_sigmap());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void SignedTransaction::CopyFrom(const SignedTransaction& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.SignedTransaction)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SignedTransaction::IsInitialized() const {
  return true;
}

void SignedTransaction::InternalSwap(SignedTransaction* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.bodybytes_, lhs_arena,
      &other->_impl_.bodybytes_, rhs_arena
  );
  swap(_impl_.sigmap_, other->_impl_.sigmap_);
}

::PROTOBUF_NAMESPACE_ID::Metadata SignedTransaction::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_transaction_5fcontents_2eproto_getter, &descriptor_table_transaction_5fcontents_2eproto_once,
      file_level_metadata_transaction_5fcontents_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto::SignedTransaction*
Arena::CreateMaybeMessage< ::proto::SignedTransaction >(Arena* arena) {
  return Arena::CreateMessageInternal< ::proto::SignedTransaction >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>

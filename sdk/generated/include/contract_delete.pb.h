// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: contract_delete.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_contract_5fdelete_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_contract_5fdelete_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021005 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "basic_types.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_contract_5fdelete_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_contract_5fdelete_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_contract_5fdelete_2eproto;
namespace proto {
class ContractDeleteTransactionBody;
struct ContractDeleteTransactionBodyDefaultTypeInternal;
extern ContractDeleteTransactionBodyDefaultTypeInternal _ContractDeleteTransactionBody_default_instance_;
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> ::proto::ContractDeleteTransactionBody* Arena::CreateMaybeMessage<::proto::ContractDeleteTransactionBody>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace proto {

// ===================================================================

class ContractDeleteTransactionBody final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:proto.ContractDeleteTransactionBody) */ {
 public:
  inline ContractDeleteTransactionBody() : ContractDeleteTransactionBody(nullptr) {}
  ~ContractDeleteTransactionBody() override;
  explicit PROTOBUF_CONSTEXPR ContractDeleteTransactionBody(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ContractDeleteTransactionBody(const ContractDeleteTransactionBody& from);
  ContractDeleteTransactionBody(ContractDeleteTransactionBody&& from) noexcept
    : ContractDeleteTransactionBody() {
    *this = ::std::move(from);
  }

  inline ContractDeleteTransactionBody& operator=(const ContractDeleteTransactionBody& from) {
    CopyFrom(from);
    return *this;
  }
  inline ContractDeleteTransactionBody& operator=(ContractDeleteTransactionBody&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const ContractDeleteTransactionBody& default_instance() {
    return *internal_default_instance();
  }
  enum ObtainersCase {
    kTransferAccountID = 2,
    kTransferContractID = 3,
    OBTAINERS_NOT_SET = 0,
  };

  static inline const ContractDeleteTransactionBody* internal_default_instance() {
    return reinterpret_cast<const ContractDeleteTransactionBody*>(
               &_ContractDeleteTransactionBody_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ContractDeleteTransactionBody& a, ContractDeleteTransactionBody& b) {
    a.Swap(&b);
  }
  inline void Swap(ContractDeleteTransactionBody* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ContractDeleteTransactionBody* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ContractDeleteTransactionBody* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ContractDeleteTransactionBody>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ContractDeleteTransactionBody& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const ContractDeleteTransactionBody& from) {
    ContractDeleteTransactionBody::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ContractDeleteTransactionBody* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "proto.ContractDeleteTransactionBody";
  }
  protected:
  explicit ContractDeleteTransactionBody(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kContractIDFieldNumber = 1,
    kPermanentRemovalFieldNumber = 4,
    kTransferAccountIDFieldNumber = 2,
    kTransferContractIDFieldNumber = 3,
  };
  // .proto.ContractID contractID = 1;
  bool has_contractid() const;
  private:
  bool _internal_has_contractid() const;
  public:
  void clear_contractid();
  const ::proto::ContractID& contractid() const;
  PROTOBUF_NODISCARD ::proto::ContractID* release_contractid();
  ::proto::ContractID* mutable_contractid();
  void set_allocated_contractid(::proto::ContractID* contractid);
  private:
  const ::proto::ContractID& _internal_contractid() const;
  ::proto::ContractID* _internal_mutable_contractid();
  public:
  void unsafe_arena_set_allocated_contractid(
      ::proto::ContractID* contractid);
  ::proto::ContractID* unsafe_arena_release_contractid();

  // bool permanent_removal = 4;
  void clear_permanent_removal();
  bool permanent_removal() const;
  void set_permanent_removal(bool value);
  private:
  bool _internal_permanent_removal() const;
  void _internal_set_permanent_removal(bool value);
  public:

  // .proto.AccountID transferAccountID = 2;
  bool has_transferaccountid() const;
  private:
  bool _internal_has_transferaccountid() const;
  public:
  void clear_transferaccountid();
  const ::proto::AccountID& transferaccountid() const;
  PROTOBUF_NODISCARD ::proto::AccountID* release_transferaccountid();
  ::proto::AccountID* mutable_transferaccountid();
  void set_allocated_transferaccountid(::proto::AccountID* transferaccountid);
  private:
  const ::proto::AccountID& _internal_transferaccountid() const;
  ::proto::AccountID* _internal_mutable_transferaccountid();
  public:
  void unsafe_arena_set_allocated_transferaccountid(
      ::proto::AccountID* transferaccountid);
  ::proto::AccountID* unsafe_arena_release_transferaccountid();

  // .proto.ContractID transferContractID = 3;
  bool has_transfercontractid() const;
  private:
  bool _internal_has_transfercontractid() const;
  public:
  void clear_transfercontractid();
  const ::proto::ContractID& transfercontractid() const;
  PROTOBUF_NODISCARD ::proto::ContractID* release_transfercontractid();
  ::proto::ContractID* mutable_transfercontractid();
  void set_allocated_transfercontractid(::proto::ContractID* transfercontractid);
  private:
  const ::proto::ContractID& _internal_transfercontractid() const;
  ::proto::ContractID* _internal_mutable_transfercontractid();
  public:
  void unsafe_arena_set_allocated_transfercontractid(
      ::proto::ContractID* transfercontractid);
  ::proto::ContractID* unsafe_arena_release_transfercontractid();

  void clear_obtainers();
  ObtainersCase obtainers_case() const;
  // @@protoc_insertion_point(class_scope:proto.ContractDeleteTransactionBody)
 private:
  class _Internal;
  void set_has_transferaccountid();
  void set_has_transfercontractid();

  inline bool has_obtainers() const;
  inline void clear_has_obtainers();

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::proto::ContractID* contractid_;
    bool permanent_removal_;
    union ObtainersUnion {
      constexpr ObtainersUnion() : _constinit_{} {}
        ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized _constinit_;
      ::proto::AccountID* transferaccountid_;
      ::proto::ContractID* transfercontractid_;
    } obtainers_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
    uint32_t _oneof_case_[1];

  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_contract_5fdelete_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ContractDeleteTransactionBody

// .proto.ContractID contractID = 1;
inline bool ContractDeleteTransactionBody::_internal_has_contractid() const {
  return this != internal_default_instance() && _impl_.contractid_ != nullptr;
}
inline bool ContractDeleteTransactionBody::has_contractid() const {
  return _internal_has_contractid();
}
inline const ::proto::ContractID& ContractDeleteTransactionBody::_internal_contractid() const {
  const ::proto::ContractID* p = _impl_.contractid_;
  return p != nullptr ? *p : reinterpret_cast<const ::proto::ContractID&>(
      ::proto::_ContractID_default_instance_);
}
inline const ::proto::ContractID& ContractDeleteTransactionBody::contractid() const {
  // @@protoc_insertion_point(field_get:proto.ContractDeleteTransactionBody.contractID)
  return _internal_contractid();
}
inline void ContractDeleteTransactionBody::unsafe_arena_set_allocated_contractid(
    ::proto::ContractID* contractid) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.contractid_);
  }
  _impl_.contractid_ = contractid;
  if (contractid) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:proto.ContractDeleteTransactionBody.contractID)
}
inline ::proto::ContractID* ContractDeleteTransactionBody::release_contractid() {
  
  ::proto::ContractID* temp = _impl_.contractid_;
  _impl_.contractid_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::proto::ContractID* ContractDeleteTransactionBody::unsafe_arena_release_contractid() {
  // @@protoc_insertion_point(field_release:proto.ContractDeleteTransactionBody.contractID)
  
  ::proto::ContractID* temp = _impl_.contractid_;
  _impl_.contractid_ = nullptr;
  return temp;
}
inline ::proto::ContractID* ContractDeleteTransactionBody::_internal_mutable_contractid() {
  
  if (_impl_.contractid_ == nullptr) {
    auto* p = CreateMaybeMessage<::proto::ContractID>(GetArenaForAllocation());
    _impl_.contractid_ = p;
  }
  return _impl_.contractid_;
}
inline ::proto::ContractID* ContractDeleteTransactionBody::mutable_contractid() {
  ::proto::ContractID* _msg = _internal_mutable_contractid();
  // @@protoc_insertion_point(field_mutable:proto.ContractDeleteTransactionBody.contractID)
  return _msg;
}
inline void ContractDeleteTransactionBody::set_allocated_contractid(::proto::ContractID* contractid) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.contractid_);
  }
  if (contractid) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(contractid));
    if (message_arena != submessage_arena) {
      contractid = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, contractid, submessage_arena);
    }
    
  } else {
    
  }
  _impl_.contractid_ = contractid;
  // @@protoc_insertion_point(field_set_allocated:proto.ContractDeleteTransactionBody.contractID)
}

// .proto.AccountID transferAccountID = 2;
inline bool ContractDeleteTransactionBody::_internal_has_transferaccountid() const {
  return obtainers_case() == kTransferAccountID;
}
inline bool ContractDeleteTransactionBody::has_transferaccountid() const {
  return _internal_has_transferaccountid();
}
inline void ContractDeleteTransactionBody::set_has_transferaccountid() {
  _impl_._oneof_case_[0] = kTransferAccountID;
}
inline ::proto::AccountID* ContractDeleteTransactionBody::release_transferaccountid() {
  // @@protoc_insertion_point(field_release:proto.ContractDeleteTransactionBody.transferAccountID)
  if (_internal_has_transferaccountid()) {
    clear_has_obtainers();
    ::proto::AccountID* temp = _impl_.obtainers_.transferaccountid_;
    if (GetArenaForAllocation() != nullptr) {
      temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
    }
    _impl_.obtainers_.transferaccountid_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::proto::AccountID& ContractDeleteTransactionBody::_internal_transferaccountid() const {
  return _internal_has_transferaccountid()
      ? *_impl_.obtainers_.transferaccountid_
      : reinterpret_cast< ::proto::AccountID&>(::proto::_AccountID_default_instance_);
}
inline const ::proto::AccountID& ContractDeleteTransactionBody::transferaccountid() const {
  // @@protoc_insertion_point(field_get:proto.ContractDeleteTransactionBody.transferAccountID)
  return _internal_transferaccountid();
}
inline ::proto::AccountID* ContractDeleteTransactionBody::unsafe_arena_release_transferaccountid() {
  // @@protoc_insertion_point(field_unsafe_arena_release:proto.ContractDeleteTransactionBody.transferAccountID)
  if (_internal_has_transferaccountid()) {
    clear_has_obtainers();
    ::proto::AccountID* temp = _impl_.obtainers_.transferaccountid_;
    _impl_.obtainers_.transferaccountid_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline void ContractDeleteTransactionBody::unsafe_arena_set_allocated_transferaccountid(::proto::AccountID* transferaccountid) {
  clear_obtainers();
  if (transferaccountid) {
    set_has_transferaccountid();
    _impl_.obtainers_.transferaccountid_ = transferaccountid;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:proto.ContractDeleteTransactionBody.transferAccountID)
}
inline ::proto::AccountID* ContractDeleteTransactionBody::_internal_mutable_transferaccountid() {
  if (!_internal_has_transferaccountid()) {
    clear_obtainers();
    set_has_transferaccountid();
    _impl_.obtainers_.transferaccountid_ = CreateMaybeMessage< ::proto::AccountID >(GetArenaForAllocation());
  }
  return _impl_.obtainers_.transferaccountid_;
}
inline ::proto::AccountID* ContractDeleteTransactionBody::mutable_transferaccountid() {
  ::proto::AccountID* _msg = _internal_mutable_transferaccountid();
  // @@protoc_insertion_point(field_mutable:proto.ContractDeleteTransactionBody.transferAccountID)
  return _msg;
}

// .proto.ContractID transferContractID = 3;
inline bool ContractDeleteTransactionBody::_internal_has_transfercontractid() const {
  return obtainers_case() == kTransferContractID;
}
inline bool ContractDeleteTransactionBody::has_transfercontractid() const {
  return _internal_has_transfercontractid();
}
inline void ContractDeleteTransactionBody::set_has_transfercontractid() {
  _impl_._oneof_case_[0] = kTransferContractID;
}
inline ::proto::ContractID* ContractDeleteTransactionBody::release_transfercontractid() {
  // @@protoc_insertion_point(field_release:proto.ContractDeleteTransactionBody.transferContractID)
  if (_internal_has_transfercontractid()) {
    clear_has_obtainers();
    ::proto::ContractID* temp = _impl_.obtainers_.transfercontractid_;
    if (GetArenaForAllocation() != nullptr) {
      temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
    }
    _impl_.obtainers_.transfercontractid_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline const ::proto::ContractID& ContractDeleteTransactionBody::_internal_transfercontractid() const {
  return _internal_has_transfercontractid()
      ? *_impl_.obtainers_.transfercontractid_
      : reinterpret_cast< ::proto::ContractID&>(::proto::_ContractID_default_instance_);
}
inline const ::proto::ContractID& ContractDeleteTransactionBody::transfercontractid() const {
  // @@protoc_insertion_point(field_get:proto.ContractDeleteTransactionBody.transferContractID)
  return _internal_transfercontractid();
}
inline ::proto::ContractID* ContractDeleteTransactionBody::unsafe_arena_release_transfercontractid() {
  // @@protoc_insertion_point(field_unsafe_arena_release:proto.ContractDeleteTransactionBody.transferContractID)
  if (_internal_has_transfercontractid()) {
    clear_has_obtainers();
    ::proto::ContractID* temp = _impl_.obtainers_.transfercontractid_;
    _impl_.obtainers_.transfercontractid_ = nullptr;
    return temp;
  } else {
    return nullptr;
  }
}
inline void ContractDeleteTransactionBody::unsafe_arena_set_allocated_transfercontractid(::proto::ContractID* transfercontractid) {
  clear_obtainers();
  if (transfercontractid) {
    set_has_transfercontractid();
    _impl_.obtainers_.transfercontractid_ = transfercontractid;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:proto.ContractDeleteTransactionBody.transferContractID)
}
inline ::proto::ContractID* ContractDeleteTransactionBody::_internal_mutable_transfercontractid() {
  if (!_internal_has_transfercontractid()) {
    clear_obtainers();
    set_has_transfercontractid();
    _impl_.obtainers_.transfercontractid_ = CreateMaybeMessage< ::proto::ContractID >(GetArenaForAllocation());
  }
  return _impl_.obtainers_.transfercontractid_;
}
inline ::proto::ContractID* ContractDeleteTransactionBody::mutable_transfercontractid() {
  ::proto::ContractID* _msg = _internal_mutable_transfercontractid();
  // @@protoc_insertion_point(field_mutable:proto.ContractDeleteTransactionBody.transferContractID)
  return _msg;
}

// bool permanent_removal = 4;
inline void ContractDeleteTransactionBody::clear_permanent_removal() {
  _impl_.permanent_removal_ = false;
}
inline bool ContractDeleteTransactionBody::_internal_permanent_removal() const {
  return _impl_.permanent_removal_;
}
inline bool ContractDeleteTransactionBody::permanent_removal() const {
  // @@protoc_insertion_point(field_get:proto.ContractDeleteTransactionBody.permanent_removal)
  return _internal_permanent_removal();
}
inline void ContractDeleteTransactionBody::_internal_set_permanent_removal(bool value) {
  
  _impl_.permanent_removal_ = value;
}
inline void ContractDeleteTransactionBody::set_permanent_removal(bool value) {
  _internal_set_permanent_removal(value);
  // @@protoc_insertion_point(field_set:proto.ContractDeleteTransactionBody.permanent_removal)
}

inline bool ContractDeleteTransactionBody::has_obtainers() const {
  return obtainers_case() != OBTAINERS_NOT_SET;
}
inline void ContractDeleteTransactionBody::clear_has_obtainers() {
  _impl_._oneof_case_[0] = OBTAINERS_NOT_SET;
}
inline ContractDeleteTransactionBody::ObtainersCase ContractDeleteTransactionBody::obtainers_case() const {
  return ContractDeleteTransactionBody::ObtainersCase(_impl_._oneof_case_[0]);
}
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace proto

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_contract_5fdelete_2eproto
